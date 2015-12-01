/*TransposeOfMatrix
program reads a matrix from the file matrix.txt, stores data into two
dimensinonal vector and writes the transpose of matrix to the file output.txt
Justinas Rumbutis 01/12/2015*/


//includes and headers
#include <fstream>
#include<vector>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

vector<double> getVectorFromLine(string line) {
	/*getVectorFromLine
	function finds values of double type separated by comma and puts them into the vector
	argument is the string from which data is obtained
	function returns a vector<double> of data*/

	int end; //index of the end of the substring
	string sub; //substring contains value separated by comma
	vector<double> vec;
	do  {
		end = line.find(',', 0);
		sub = line.substr(0, end);
		vec.push_back(atof(sub.c_str())); //puts data into vector
		line = line.substr(end + 1, line.size() - end - 1); //reduces the size of the line
	} while (end != -1); //if comma was not found the while loop stops
	return vec;
}

//main program
int main() {
	fstream file("matrix.txt"); //opening the file
	string line;  //variable in which recently read line is stored
	bool wrongInput = false; //variable checks if all rows are of the same size
	vector<vector<double>> data; //vector containing data

	if (file.is_open()) { 
		while (!file.eof()) { //loops through all lines
			getline(file, line);
			if (!line.empty()){ //checks if line is empty
				vector<double> tVector = getVectorFromLine(line); //temporary vector containing one line
				data.push_back(tVector);

				if (tVector.size() != data[0].size()){//checks if all rows are of the same size
				wrongInput = true;
				cout << "Wrong input" << endl;
				break;
			}
			}
		}

		file.close();
		FILE *output = fopen("output.txt", "w"); //output file is named output.txt
	

		for (int i = 0; i < data[0].size() && !wrongInput; i++) { //the first loop goes through the all columns 
			
				for (int j = 0; j < data.size(); j++){ //the second loop goes through the all rows of a given column
				
						fprintf(output, "%6.1f ", data[j][i]); //prints the column of the matrix to the row of the output file
					
				
				}
				fprintf(output, "\n"); //goes to a new line after printing the row
			
			
		}
		fclose(output);
	}
	else {
		cout << "No file\n";
		exit(1);
	}
	return 0;
}