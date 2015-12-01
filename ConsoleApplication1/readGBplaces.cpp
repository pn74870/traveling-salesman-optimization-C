#include <fstream>
#include<vector>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

vector<string> getVectorFromLine(string line) {
	int end;
	string sub;
	vector<string> vec;
	do  {
		end = line.find(',', 0);
		sub = line.substr(0, end );
		vec.push_back(sub);
		line = line.substr(end + 1, line.size() - end - 1);
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
		FILE *output=fopen ("output.txt","w");
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) fprintf(output,"%10.1f ",atof(data[i][j].c_str()));
			fprintf(output,"\n");
		}
		fclose(output);
	}
	else cout << "No file\n";
}