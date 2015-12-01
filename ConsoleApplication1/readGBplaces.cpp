#include <fstream>
#include<vector>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

vector<string> getVectorFromLine(string line) {
	int end;
	string sub,newLine=line;
	vector<string> vec;
	do  {
		end = newLine.find(',', 0);
		sub = newLine.substr(0, end );
		vec.push_back(sub);
		newLine = newLine.substr(end + 1, newLine.size() - end - 1);
	} while (end != -1);
	return vec;
}

int main() {
	fstream file("GBplaces.csv");
	string line;
	
	vector<vector<string>> data;
	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			vector<string> tVector=getVectorFromLine(line);
			
			
			data.push_back(tVector);
		}

		file.close();
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) cout << data[i][j] << " ";
			cout << "\n";
		}
	}
	else cout << "No file\n";
}