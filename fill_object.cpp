#include <iostream>
#include <cstdio>
#include <vector>
#include <climits>
#include <cfloat>
#include <cstdlib>
using namespace std;

#define CELL_LENGTH 1.0		// 0.1 mm


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


// this function is extreamly slow, better to use kd-tree
bool point_exist_in_cell(float lowerleft_x, float lowerleft_y) {
	for (int i = 0; i < v.size(); i++) {
		Point p = v[i];
		if (p.x > lowerleft_x &&
			p.x < lowerleft_x + CELL_LENGTH &&
			p.y > lowerleft_y &&
			p.y < lowerleft_y + CELL_LENGTH) {
			return true;
		}
	}
	return false;
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
			if (point_exist_in_cell(min_x + i * CELL_LENGTH, min_y + j * CELL_LENGTH)) {
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

	for (int i = 0; i < 100; i++) {
		grid = grow(grid, 5, true);
	}

	for (int i = 0; i < 30; i++) {
		grid = grow(grid, 3, false);
	}

	//for (int i = 0; i < 10; i++) {
	//	grid = shrink(grid);
	//}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if ((grid[i][j] & INSIDE) == INSIDE) {
				cout << min_x + i * CELL_LENGTH << " " << min_y + j * CELL_LENGTH << endl;
			}
		}
	}
	
	return 0;
}
