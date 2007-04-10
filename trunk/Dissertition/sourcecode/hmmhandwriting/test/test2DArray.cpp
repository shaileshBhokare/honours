#include <iostream>
#include <fstream>
#include <string>
#include "../convert.h"

namespace rh = redhat;
using namespace std;

int main(){
	string line;
	double num;
	double matrix[100][16];
	int row=0;
	int column=0;
	
	ifstream infoFile("../data/trainingData/example.txt");
	if(infoFile.is_open()){
		while(!infoFile.eof()){
			//infoFile>>num;
			getline(infoFile, line);
			num = rh::convertToDouble(line);
//			cout<<num<<endl;
			matrix[row][column]=num;
			column++;
			if(column==16){
				row++;
			}
			column = column%16;
		}
	}

	
	
	for(int i=0; i<row; i++){
		for(int j=0; j<16; j++){
			cout<<matrix[i][j]<<endl;
		}
	}
}