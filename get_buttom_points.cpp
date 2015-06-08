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
	freopen("objectASC.txt", "r", stdin);
	freopen("buttom_points.txt", "w", stdout);
	float x, y, z;
	float a, b, c, d;
	while (cin >> x >> y >> z >> a >> b >> c >> d) {
		if (z < THRESHOLD) {
			cout << x << " " << y << endl;
		}
	}
	return 0;
}
