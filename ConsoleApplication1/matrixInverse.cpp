/*InverseOfMatrix
program reads a matrix from the file matrix.txt, stores data into two
dimensinonal vector and writes the inverse of matrix to the file output.txt
can also compute the determinant
values in the input file should be separated by space
separation character can be changed by changing the variable named separator
Justinas Rumbutis 01/12/2015*/


//includes and headers
#include <fstream>
#include<vector>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

vector<double> getVectorFromLine(string line, char separator) {
	/*getVectorFromLine
	function finds values of double type separated by some character and puts them into the vector
	arguments: the string from which data is obtained and the character which separates values
	function returns a vector<double> of data*/

	int end; //index of the end of the substring
	string sub; //substring contains one value
	vector<double> vec;
	do {
		end = line.find(separator, 0);
		sub = line.substr(0, end);
		vec.push_back(atof(sub.c_str())); //puts data into vector
		line = line.substr(end + 1, line.size() - end - 1); //reduces the size of the line
	} while (end != -1); //if separator was not found the while loop stops
	return vec;
}

vector<vector<double>> identityMatrix(int size) {
	/*identityMatrix
	function creates an identity matrix of a given size (size *size)*/
	vector<vector<double>> matrix;
	for (int i = 0; i < size; i++) {
		vector<double> temp;
		for (int j = 0; j < size; j++) {
			if (i == j) temp.push_back(1.);
			else temp.push_back(0.);
		}
		matrix.push_back(temp);
	}
	return matrix;
}
double determinant(vector<vector<double>> matrix) {
	/*determinant
	function calculates the determinant of a given matrix
	*/
	for (int i = 0; i < matrix.size(); i++) {

		for (int k = i + 1; k < matrix.size(); k++) {

			double multiplier = matrix[k][i] / matrix[i][i];
			for (int j = 0; j < matrix[0].size(); j++) {
				matrix[k][j] -= multiplier* matrix[i][j];


			}
		}
	}
	double d = 1;
	for (int i = 0; i < matrix.size() && d!=0; i++) d *= matrix[i][i];
	return d;
}

vector<vector<double>> inverseMatrix(vector<vector<double>> matrix)  {
	/*inverseMatrix
	function calculates the inverse of a given matrix
	*/
	vector<vector<double>> inverse=identityMatrix(matrix.size()); //identity matrix which is initially on the RHS of the augmented matrix

	for (int i = 0; i < matrix.size(); i++) { // this loops through all rows
		double divider = matrix[i][i]; //variable which stores diagonal element
		for (int j = 0; j < matrix.size(); j++) {//loops through all columns of a given row
			matrix[i][j] /=divider; //divides equation by the diagonal element
			inverse[i][j] /= divider;
			
		}
		//now the diagonal element of this row is 1
		for (int k = 0; k < matrix.size(); k++) { //loops through other rows and eliminates i th column
			if (k != i) {
				double multiplier = matrix[k][i]; //variable stores the number by which the i th row has to be multiplied in order to eliminate i th element of k th row
			for (int j = 0; j < matrix.size(); j++) { //subtracts multiplier*i th row from k th row
				
					matrix[k][j] -= multiplier* matrix[i][j];
					inverse[k][j] -= multiplier * inverse[i][j];
					
				}
		
			}
		}
	
	}
	return inverse; //now LHS of the augmented matrix should become identity matrix and RHS (initial identity matrix) should become the inverse of a matrix
}
//main program
int main() {
	fstream file("matrix.txt"); //opening the file
	string line;  //variable in which recently read line is stored
	bool wrongInput = false; //variable is used to check if the input is correct
	vector<vector<double>> data; //vector containing data
	char separator = ' '; //this specifies what character is used for the separation of values
	if (file.is_open()) {
		while (!file.eof()) { //loops through all lines
			getline(file, line);
			if (!line.empty()) { //checks if line is empty
				vector<double> tVector = getVectorFromLine(line, separator); //temporary vector containing one line

				data.push_back(tVector);

				if (tVector.size() != data[0].size()) {//checks if all rows are of the same size
					wrongInput = true;
					break;
				}
			}
		}
		file.close();
		if (wrongInput || data[0].size() != data.size()) cout << "Wrong input\n"; //also checks if matrix is square
		else {
			double det = determinant(data);
			cout << "determinant = " << det << endl;

			if (det != 0) {
				FILE *output = fopen("output.txt", "w"); //output file is named output.txt
				vector<vector<double>> inverse = inverseMatrix(data);//vector contains the inverse matrix


				for (int i = 0; i < data.size(); i++) { //the first loop goes through the all rows 

					for (int j = 0; j < data.size(); j++) { //the second loop goes through the all columns of a given row

						fprintf(output, "%6.4f ", inverse[i][j]); //prints the [i][j] element of the inverse


					}
					fprintf(output, "\n"); //goes to a new line after printing the row


				}
				fclose(output);
			}
			else cout << "matrix doesnt have inverse!\n";
		}
	}
	else {
		cout << "No file\n";
		exit(1);
	}
	return 0;
}