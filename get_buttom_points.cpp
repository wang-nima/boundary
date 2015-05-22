#include <iostream>
#include <cstdio>
using namespace std;

#define THRESHOLD 4.5

//struct Point {
//	float x;
//	float y;
//	float z;
//};

int main() {
	freopen("all_points_without_noise.txt", "r", stdin);
	freopen("buttom_points.txt", "w", stdout);
	float x, y, z;
	while (cin >> x >> y >> z) {
		if (z < THRESHOLD) {
			cout << x << " " << y << endl;
		}
	}
	return 0;
}
