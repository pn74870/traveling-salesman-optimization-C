/*optimisation
Program reads the file GBplaces.csv and finds the optimal place for the logistics hub.
Assumptions: the cost of travelling to a place is proportional to the GreatCircle
distance between the hub and that place and the population of that place.
*/
#include <fstream>
#include<vector>
#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#define PI 3.14159265
using namespace std;


struct place {
	string name, type;
	int population;
	double longitude, latitude;
	bool visited = false;
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

	boundaryCoordinates(vector<place> places) {
		double minLat = places[0].latitude, maxLat = minLat, minLong = places[0].longitude, maxLong = minLong;
		for (int i = 1; i < places.size(); i++) {
			if (places[i].latitude < minLat) minLat = places[i].latitude;
			if (places[i].latitude > maxLat)maxLat = places[i].latitude;
			if (places[i].longitude < minLong) minLong = places[i].longitude;
			if (places[i].longitude > maxLong)maxLong = places[i].longitude;
		}

		maxLatitude = maxLat;
		minLatitude = minLat;
		maxLongitude = maxLong;
		minLongitude = minLong;

	}
};
struct coordinates {
	double latitude, longitude;
	vector<place> nearestPlaces;
	coordinates(double lat, double lon) {
		latitude = lat;
		longitude = lon;
	}
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
double degToRad(double& degrees) {
	return degrees / 180 * PI;
}

double greatCircleDistance(double& lat1, double& long1, double& lat2, double& long2) {
	double deltaLong = long1 - long2;
	return abs(acos(cos(degToRad(lat1))*cos(degToRad(lat2))*cos(degToRad(deltaLong)) + sin(degToRad(lat1))*sin(degToRad(lat2))));
}
double cost(vector<place> data, double lat, double lon) {
	double totalDistance = 0;
	for (int i = 0; i < data.size(); i++) {
		//	totalDistance += data[i].population*greatCircleDistance(lat, lon, data[i].latitude, data[i].longitude);
		totalDistance += greatCircleDistance(lat, lon, data[i].latitude, data[i].longitude);

	}
	return totalDistance;
}
int indexOfNN(vector<place> &data, int &index) {
	int inOfNearest = -1;
	for (int i = 0; i < data.size(); i++) {
		if (i != index && !data[i].visited) {
			inOfNearest = i;
		}
	}
	if (inOfNearest > -1) {
		double minDist = greatCircleDistance(data[inOfNearest].latitude, data[inOfNearest].longitude, data[index].latitude, data[index].longitude);
		for (int i = 1; i < data.size(); i++) {
			if (i != index) {
				double dist = greatCircleDistance(data[i].latitude, data[i].longitude, data[index].latitude, data[index].longitude);
				if (dist < minDist&& !data[i].visited) {
					inOfNearest = i;
					minDist = dist;
				}
			}
		}
	}

	return inOfNearest;
}
double calcDist(vector<place> data, int i, int j) {
	return greatCircleDistance(data[i].latitude, data[i].longitude, data[j].latitude, data[j].longitude);
}
double cost1(vector<place> &data, double& latitude, double &longitude) {
	double distance = 0;
	for (int i = 0; i < data.size(); i++) {
		if (!data[i].visited) {
			data[i].visited = true;
			int nearest = indexOfNN(data, i);
			if (nearest != -1 && greatCircleDistance(data[nearest].latitude, data[nearest].longitude, data[i].latitude, data[i].longitude)
				< greatCircleDistance(data[nearest].latitude, data[nearest].longitude, latitude, longitude)) {
				data[nearest].visited = true;
				if (data[nearest].population > data[i].population) {
					distance += data[i].population*(greatCircleDistance(data[nearest].latitude, data[nearest].longitude, data[i].latitude, data[i].longitude)+
					 greatCircleDistance(latitude, longitude, data[i].latitude, data[i].longitude));
					distance += (2 * data[nearest].population - data[i].population)*greatCircleDistance(latitude, longitude, data[nearest].latitude, data[nearest].longitude);
				}
				else {
						distance += data[nearest].population*(greatCircleDistance(data[nearest].latitude, data[nearest].longitude, data[i].latitude, data[i].longitude) +
							greatCircleDistance(latitude, longitude, data[nearest].latitude, data[nearest].longitude));
						distance += (2 * data[i].population - data[nearest].population)*greatCircleDistance(latitude, longitude, data[i].latitude, data[i].longitude);
					}
				}
			}
			else {
				distance += 2*greatCircleDistance(latitude, longitude, data[i].latitude, data[i].longitude);
			}
		
	}


	return distance;
}
double cost2(vector<place> &data, vector<coordinates> &coordOfHubs, bool useSimpleCost) {
	double distance = 0;
	for (int i = 0; i < data.size(); i++) {
		data[i].visited = false;
		double shortestDistToHub = greatCircleDistance(data[i].latitude, data[i].longitude, coordOfHubs[0].latitude, coordOfHubs[0].longitude);
		int indexOfNearest = 0;
		for (int j = 1; j < coordOfHubs.size(); j++) {
			double distToHub =  greatCircleDistance(data[i].latitude, data[i].longitude, coordOfHubs[j].latitude, coordOfHubs[j].longitude);
			if (distToHub < shortestDistToHub) {
				shortestDistToHub = distToHub;
				indexOfNearest = i;
			}
		}
		coordOfHubs[indexOfNearest].nearestPlaces.push_back(data[i]);
		
	}
	for (int i = 0; i < coordOfHubs.size(); i++) {
		distance +=useSimpleCost?cost(coordOfHubs[i].nearestPlaces,coordOfHubs[i].latitude, coordOfHubs[i].longitude):
			cost1(coordOfHubs[i].nearestPlaces, coordOfHubs[i].latitude, coordOfHubs[i].longitude);
	}
	return distance;
}
double acceptanceProbability(double &energy, double &newEnergy, double &temperature) {
	// If the new solution is better, accept it

	if (newEnergy < energy) {
		return 1.0;
	}
	// If the new solution is worse, calculate a probability
	//   return 0;
	return exp((energy - newEnergy) / temperature);
}
int main() {
	const int n = 5;
	const double rate = 0.05;
	bool useSimpleCost = true;
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
		boundaryCoordinates boundaries(data);
		double stepLat = (boundaries.maxLatitude - boundaries.minLatitude) / 1000;
		double stepLong = (boundaries.maxLongitude - boundaries.minLongitude) / 1000;
		double bestValue = -1;
		for (int l = 0; l < 1; l++) {
			double temperature = 10000;
			vector<coordinates> coordinatesOfHubs;
			for (int o = 0; o < n; o++) {
				double lat = boundaries.minLatitude + rand() % (1000 + 1) * (1. / 1000) * (boundaries.maxLatitude - boundaries.minLatitude);
				double lon = boundaries.minLongitude + rand() % (1000 + 1) * (1. / 1000) * (boundaries.maxLongitude - boundaries.minLongitude);
				coordinates hubCoord(lat, lon);
				coordinatesOfHubs.push_back(hubCoord);
			}
			double oldValue = cost2(data, coordinatesOfHubs, useSimpleCost);
			if (bestValue == -1) bestValue = oldValue;
			bool improved;
			do {
				random_shuffle(begin(data), end(data));
				improved = false;
				for (int m = 0; m < n; m++) {
					for (int i = -1; i < 2; i++) {
						for (int j = -1; j < 2; j++) {
							if (!(i == 0 && j == 0)) {
								coordinates newHub(coordinatesOfHubs[m].latitude + i*stepLat, coordinatesOfHubs[m].longitude + j*stepLong);
								vector <coordinates> tempVector = coordinatesOfHubs;
								tempVector[m] = newHub;
								double newValue = cost2(data, tempVector,useSimpleCost);
								if (acceptanceProbability(oldValue, newValue, temperature)>(double)rand() / RAND_MAX) {
									coordinatesOfHubs = tempVector;
									oldValue = newValue;
								}
							/*	if (newValue < oldValue) {
									coordinatesOfHubs = tempVector;
									oldValue = newValue;
									improved = true;
								}*/
							}
						}
					}
				}
				
				if (bestValue > oldValue) bestValue = oldValue;
				for (int i = 0; i < n; i++) cout << coordinatesOfHubs[i].latitude << " " << coordinatesOfHubs[i].longitude << " " << oldValue << " t="<<temperature<< endl;
				temperature *= 1 - rate;
			//	cout << "-----------" << endl;
			} while (temperature>0.0000001);
			cout << bestValue << endl;
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