//reading files

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
struct point {
	double x, y, e;
};
struct fittedParameters {
	double m, dm, c, dc, chiSquare
};
double interpolate(double x,point points[],int length) {
	double deltaMin1 = points[0].x - x, deltaMin2 = points[1].x - x;
	int numberOfX1=0,numberOfX2=1;
	for (int i =2; i < length; i++) {
		double delta = points[i].x - x;
		if (pow(deltaMin1, 2) > pow(delta, 2)) {
			deltaMin1 = delta;
			numberOfX1 = i;
		}
		else if (pow(deltaMin2, 2) > pow(delta, 2)) {
			deltaMin2 = delta;
			numberOfX2 = i;
		}
	}

	return points[numberOfX1].y - (points[numberOfX2].y - points[numberOfX1].y) / (points[numberOfX2].x - points[numberOfX1].x)*deltaMin1;

}

fittedParameters fittedCurve(point points[],int size) {
	double sw = 0;
	for (int i = 0; i < size; i++) {
		sw = sw + points[i].x / pow(points[i].e, 2);
	}
}


int main() {
	ifstream dataFile("xy_data.txt");
	point points[5];
	double x;
	if (dataFile.is_open()) {
		int i = 0;
		while (!dataFile.eof()) {
			dataFile >> points[i].x >> points[i].y;
			i++;
		}
		dataFile.close();
	
		for (int j = 0; j < i; j++) {
			printf("x= %1.3f, y= %4.3f\n", points[j].x, points[j].y);
			//cout << points[j].x << " " << points[j].y << endl;
		}
		cin >> x;
		
		double y = interpolate(x, points, i);
		printf("y at x %.2f is %.5f", x, y);
	}
	else cout << "no file";
	
	return 0;
}