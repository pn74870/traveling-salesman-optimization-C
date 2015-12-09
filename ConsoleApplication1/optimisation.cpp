#include <fstream>
#include<vector>
#include <iomanip>
#include <iostream>
#include <string>
#define PI 3.14159265
using namespace std;

struct place {
	string name, type;
	int population;
	double longitude, latitude;
	place(string values[]) {
		name = values[0];
		type = values[1];
		population = atoi(values[2].c_str());
		latitude = atof(values[3].c_str());
		longitude = atof(values[4].c_str());
		
	}
};
struct boundaryCoordinates {
	double minLatitude, maxLatitude, minLongitude, maxLongitude;
};
place getPlaceFromLine(string line) {
	int end;
	string sub;
	string values[5];
	
	for (int i = 0; i < 5; i++) {
		end = line.find(',', 0);
		sub = line.substr(0, end);
		values[i] = sub;
		line = line.substr(end + 1, line.size() - end - 1);
		
	}
	place newPlace(values);
	return newPlace;
}
double degToRad(double degrees) {
	return degrees / 180 * PI;
}
boundaryCoordinates getBoundaryCoordinates(vector<place> places) {
	double minLat = places[0].latitude, maxLat = minLat, minLong = places[0].longitude, maxLong = minLong;
	for (int i = 1; i < places.size(); i++) {
		if (places[i].latitude < minLat) minLat = places[i].latitude;
		if (places[i].latitude > maxLat)maxLat = places[i].latitude;
		if (places[i].longitude < minLong) minLong = places[i].longitude;
		if (places[i].longitude > maxLong)maxLong = places[i].longitude;
	}
	boundaryCoordinates boundaries;
	boundaries.maxLatitude = maxLat;
	boundaries.minLatitude = minLat;
	boundaries.maxLongitude = maxLong;
	boundaries.minLongitude = minLong;
	return boundaries;
}
double greatCircleDistance(double lat1, double long1, double lat2, double long2) {
	return abs(acos(cos(degToRad(lat1))*cos(degToRad(lat2))*cos(degToRad(long1-long2))+sin(degToRad(lat1))*sin(degToRad(lat2))));
}
double cost(vector<place> data,double lat,double lon) {
	double totalDistance = 0;
	for (int i = 0; i < data.size(); i++) {
		totalDistance += greatCircleDistance(lat, lon, data[i].latitude, data[i].longitude);
	}
	return totalDistance;
}
int main() {
	fstream file("GBplaces.csv");
	string line;
	bool firstLine = true;
	vector<place> data;
	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			if (!firstLine && !line.empty()) {
				place tPlace = getPlaceFromLine(line);
				data.push_back(tPlace);
			}
			else firstLine = false;
		}
		srand(time(NULL));
		file.close();
		boundaryCoordinates boundaries = getBoundaryCoordinates(data);
		double stepLat=(boundaries.maxLatitude-boundaries.minLatitude)/1000;
		double stepLong = (boundaries.maxLongitude - boundaries.minLongitude) / 1000;
		for (int l = 0; l < 50; l++) {
			double lat = boundaries.minLatitude + rand() % (1000 + 1) * (1. / 1000) * (boundaries.maxLatitude - boundaries.minLatitude);
			double lon = boundaries.minLongitude + rand() % (1000 + 1) * (1. / 1000) * (boundaries.maxLongitude - boundaries.minLongitude);
		
			double oldValue = cost(data, lat, lon);
			bool improved;
			do {
				improved = false;
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if (!(i == 0 && j == 0)) {
							double newValue = cost(data, lat + i*stepLat, lon + j*stepLong);
							if (newValue < oldValue) {
								lat += i*stepLat;
								lon += j*stepLong;
								oldValue = newValue;
								improved = true;
							}
						}
					}
				}


			} while (improved);
			cout << lat<<" "<<lon << endl;
		}
		/*FILE *output = fopen("output.txt", "w");
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) fprintf(output, "%10.1f ", atof(data[i][j].c_str()));
			fprintf(output, "\n");
		}
		fclose(output);*/
	}
	else cout << "No file\n";
}