#include <iostream>
#include <cstdio>
#include <vector>
#include <climits>
#include <cfloat>
using namespace std;

#define CELL_LENGTH 0.1		// 0.1 mm


#define UNLABELED 0
#define BOUNDARY 1

struct Point {
	float x;
	float y;
	Point(float _x, float _y) : x(_x), y(_y) {}
};

vector<Point> v;

// this function is extreamly slow, better to use kd-tree
void point_in_cell(float lowerleft_x, float lowerleft_y) {

}

int main() {
	freopen("buttom_points.txt", "r", stdin);
	
	// save points into vector
	float x, y;
	while (cin >> x >> y) {
		v.push_back(Point(x,y));
	}
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
	vector<vector<unsigned char> > grid(n, vector<unsigned char>(m, UNLABELED));
	for (float i = min_x; i < max_x; i += CELL_LENGTH) {
		for (float j = min_y; j < max_y; j += CELL_LENGTH) {
			
		}
	}
	
	return 0;
}
