#include <iostream>
#include <cstdio>
#include <vector>
#include <climits>
#include <cfloat>
#include <cstdlib>
using namespace std;

#define CELL_LENGTH 3		// 1 mm
#define SMALL_CELL_LENGTH 0.1


#define UNLABELED 0
#define BOUNDARY 1
#define INSIDE 2


#define EIGHT_DIRECTION

struct Point {
	float x;
	float y;
	Point(float _x, float _y) : x(_x), y(_y) {}
};

vector<Point> v;

void init() {
	srand (time(NULL));
}


// this function is extreamly and naive slow, better to use kd-tree
bool point_exist_in_cell(float lowerleft_x, float lowerleft_y, int all, float length) {
	int count = 0;

	for (int i = 0; i < v.size(); i++) {
		Point p = v[i];
		if (p.x > lowerleft_x &&
			p.x < lowerleft_x + length &&
			p.y > lowerleft_y &&
			p.y < lowerleft_y + length ) {
			count++;
		}
	}
	return count >= all ? true : false;
}

// inside points and boundary are both neighbors
int get_neighbor_count(const vector<vector<unsigned int> > &v, int x, int y) {
	int count = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y)
				continue;
			if ( ((v[i][j] & BOUNDARY) == BOUNDARY) || ((v[i][j] & INSIDE) == INSIDE) )
				count++;
		}
	}
	//cout << count << endl;
	return count;
}


int get_neighbor_count_no_boundary(const vector<vector<unsigned int> > &v, int x, int y) {
	int count = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y)
				continue;
			if ( (v[i][j] & BOUNDARY) == BOUNDARY )
				return 0;
			if ( (v[i][j] & INSIDE) == INSIDE )
				count++;
		}
	}
	//cout << count << endl;
	return count;
}


vector<vector<unsigned int> > grow(const vector<vector<unsigned int> > &v, int bound, bool count_boundary = true) {
	int m = v.size();
	int n = v[0].size();
	vector<vector<unsigned int> > ret = v;
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			if (count_boundary) {
				if (get_neighbor_count(v, i, j) >= bound) {
					ret[i][j] |= INSIDE;
				}
			} else {
				if (get_neighbor_count_no_boundary(v, i, j) >= bound) {
					ret[i][j] |= INSIDE;
				}
			}
		}
	}
	return ret;
}

vector<vector<unsigned int> > shrink(vector<vector<unsigned int> > &v, int bound) {
	int m = v.size();
	int n = v[0].size();
	vector<vector<unsigned int> > ret = v;
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			if (get_neighbor_count(v, i, j) <= bound) {
				ret[i][j] &= ~INSIDE;
			}
		}
	}
	return ret;
}

int main() {
	freopen("buttom_points.txt", "r", stdin);
	
	// save points into vector
	float x, y;
	while (cin >> x >> y) {
		v.push_back(Point(x,y));
	}
	//fclose(stdin);
	cout << "total points: " << v.size() << endl;

	// find the points with max/min x/y
	float min_x = FLT_MAX;
	float max_x = FLT_MIN;
	float min_y = FLT_MAX;
	float max_y = FLT_MIN;
	for (int i = 0; i < v.size(); i++) {
		if (v[i].x < min_x) {
			min_x = v[i].x;
		}
		if (v[i].x > max_x) {
			max_x = v[i].x;
		}
		if (v[i].y < min_y) {
			min_y = v[i].y;
		}
		if (v[i].y > max_y) {
			max_y = v[i].y;
		}
	}
	cout << "min x: " << min_x << "\nmax x: " << max_x << "\nmin y: " << min_y << "\nmax y: " << max_y  << endl;
	// cut into grid
	int m = ( max_x - min_x ) / CELL_LENGTH;
	int n = ( max_y - min_y ) / CELL_LENGTH;

	m++;
	n++;

	cout << "m = " << m << "\nn = " << n << endl;

	freopen("grid_points.txt", "w", stdout);

	vector<vector<unsigned int> > grid(m, vector<unsigned int>(n, UNLABELED));
	for (float i = 0; i < m; i++) {
		for (float j = 0; j < n; j++) {
			if (point_exist_in_cell(min_x + i * CELL_LENGTH, min_y + j * CELL_LENGTH, 10, CELL_LENGTH)) {
				grid[i][j] |= BOUNDARY;
				cout << min_x + i * CELL_LENGTH << " " << min_y + j * CELL_LENGTH << endl;
			}
		}
	}

	//fclose(stdout);
	//cout << "grid cell count: " << grid.size() << endl;
	freopen("random_inside_points.txt", "w", stdout);

	int count = 0;
	const int threshold = 3000;

	while (count < threshold) {
		int x = rand() % m;
		int y = rand() % n;

		// check 4 direction
		// up
		bool hit = false;
		for (int i = y; i < n; i++) {
			if ((grid[x][i] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

		// down
		hit = false;
		for (int i = y; i >= 0; i--) {
			if ((grid[x][i] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

		// right 
		hit = false;
		for (int i = x; i < m; i++) {
			if ((grid[i][y] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

		// left 
		hit = false;
		for (int i = x; i >= 0; i--) {
			if ((grid[i][y] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

#ifdef EIGHT_DIRECTION
		// upper right
		hit = false;
		for (int i = x, j = y; i < m && j < n; i++, j++) {
			if ((grid[i][j] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

		// lower right
		hit = false;
		for (int i = x, j = y; i < m && j >= 0; i++, j--) {
			if ((grid[i][j] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}		

		// upper left
		hit = false;
		for (int i = x, j = y; i >= 0 && j < n; i--, j++) {
			if ((grid[i][j] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}	

		// lower left
		hit = false;
		for (int i = x, j = y; i >= 0 && j >= 0; i--, j--) {
			if ((grid[i][j] & BOUNDARY) == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

#endif
		count++;
		grid[x][y] |= INSIDE;
		//cout << min_x + x * CELL_LENGTH << " " << min_y + y * CELL_LENGTH << endl;
	}

	//grid = grow(grid);

	for (int i = 0; i < 1000; i++) {
		grid = grow(grid, 5, true);
	}

	//for (int i = 0; i < 1000; i++) {
	//	grid = shrink(grid, 5);
	//}


	for (int i = 0; i < 300; i++) {
		grid = grow(grid, 3, false);
	}

// if a grid is boundary it can't be inside
 	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if ((grid[i][j] & BOUNDARY) == BOUNDARY) {
				grid[i][j] &= ~INSIDE;
			}
		}
	}

// print random inside points
	
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if ((grid[i][j] & INSIDE) == INSIDE) {
				cout << min_x + i * CELL_LENGTH << " " << min_y + j * CELL_LENGTH << endl;
			}
		}
	}

	
	freopen("final.txt", "w", stdout);


// print fine boundary

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if ((grid[i][j] & BOUNDARY) == BOUNDARY) {
				// look for inside grid
				// 1 up 2 right 3 down 4 left 0 not found
				int direction = 0;
				if (j + 1 < n && (grid[i][j+1] & INSIDE) == INSIDE) {
					direction = 1;
				} else if (i + 1 < m && (grid[i+1][j] & INSIDE) == INSIDE) {
					direction = 2;
				} else if (j - 1 >= 0 && (grid[i][j-1] & INSIDE) == INSIDE) {
					direction = 3;
				} else if (i - 1 >= 0 && (grid[i-1][j] & INSIDE) == INSIDE) {
					direction = 4;
				}
				// inside grid near me not found
				if (direction == 0) {
					continue;
				}
				int size = CELL_LENGTH / SMALL_CELL_LENGTH;
				vector<vector<bool> > exist(size, vector<bool>(size, false));
				// cut big cell into smaller
				for (int p = 0; p < size; p++) {
					for (int q = 0; q < size; q++) {
						if (point_exist_in_cell(min_x + i * CELL_LENGTH + p * SMALL_CELL_LENGTH,
												min_y + j * CELL_LENGTH + q * SMALL_CELL_LENGTH,
												2,
												SMALL_CELL_LENGTH)) {
							exist[p][q] = true;
						}
					}
				}

				if (direction == 1) {
					// from top to buttom
					for (int p = 0; p < size; p++) {
						for (int q = size - 1; q >= 0; q--) {
							if (exist[p][q] == false) {
								cout << min_x + i * CELL_LENGTH + p * SMALL_CELL_LENGTH << " "\
									 << min_y + j * CELL_LENGTH + q * SMALL_CELL_LENGTH << endl;
							} else {
								break;
							}
						}
					}
				} else if (direction == 2) {
					// from right to left
					for (int q = size - 1; q >= 0; q--) {
						for (int p = size - 1; p >= 0; p--) {
							if (exist[p][q] == false) {
								cout << min_x + i * CELL_LENGTH + p * SMALL_CELL_LENGTH << " "\
									 << min_y + j * CELL_LENGTH + q * SMALL_CELL_LENGTH << endl;
							} else {
								break;
							}
						}
					}

				} else if (direction == 3) {
					// from buttom to top
					for (int p = 0; p < size; p++) {
						for (int q = 0; q < size; q++) {
							if (exist[p][q] == false) {
								cout << min_x + i * CELL_LENGTH + p * SMALL_CELL_LENGTH << " "\
									 << min_y + j * CELL_LENGTH + q * SMALL_CELL_LENGTH << endl;
							} else {
								break;
							}
						}
					}
					
				} else {	// direction == 4      0 is impossable here
					for (int q = size - 1; q >= 0; q--) {
						for (int p = 0; p < size; p++) {
							if (exist[p][q] == false) {
								cout << min_x + i * CELL_LENGTH + p * SMALL_CELL_LENGTH << " "\
									 << min_y + j * CELL_LENGTH + q * SMALL_CELL_LENGTH << endl;
							} else {
								break;
							}
						}
					}
				}

				// look for furthest point from inside to boundary
			}
		}
	}

	int size = CELL_LENGTH / SMALL_CELL_LENGTH;

	// fill in side point
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if ((grid[i][j] & INSIDE) == INSIDE) {
				for (int p = 0; p < size; p++) {
					for (int q = 0; q < size; q++) {
						cout << min_x + i * CELL_LENGTH + p * SMALL_CELL_LENGTH << " "\
							 << min_y + j * CELL_LENGTH + q * SMALL_CELL_LENGTH << endl;
					}
				}
			}
		}
	}



	
	return 0;
}
