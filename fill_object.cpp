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

	vector<vector<int> > grid(m, vector<int>(n, UNLABELED));
	for (float i = 0; i < m; i++) {
		for (float j = 0; j < n; j++) {
			if (point_exist_in_cell(min_x + i * CELL_LENGTH, min_y + j * CELL_LENGTH)) {
				grid[i][j] = BOUNDARY;
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
			if (grid[x][i] == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}
		hit = false;
		// down
		for (int i = y; i >= 0; i--) {
			if (grid[x][i] == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}
		hit = false;
		// right 
		for (int i = x; i < m; i++) {
			if (grid[i][y] == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}
		hit = false;
		// left 
		for (int i = x; i >= 0; i--) {
			if (grid[i][y] == BOUNDARY) {
				hit = true;
				break;
			}
		}
		if (!hit) {
			continue;
		}

		count++;
		cout << min_x + x * CELL_LENGTH << " " << min_y + y * CELL_LENGTH << endl;
	}
	return 0;
}
