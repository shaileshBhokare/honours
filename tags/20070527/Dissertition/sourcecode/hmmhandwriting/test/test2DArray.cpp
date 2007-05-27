#include <iostream>
#include <fstream>
#include <string>
#include "../convert.h"
#include "../Node.h"

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

	
	cout<<"Test case one"<<endl;
	for(int i=0; i<row; i++){
		for(int j=0; j<16; j++){
			cout<<matrix[i][j]<<endl;
		}
	}
	
	rh::Node NodeMatrix[2][2];
	NodeMatrix[0][0].probability = 0.9;
	NodeMatrix[0][0].path = 2;
	NodeMatrix[1][1] = NodeMatrix[0][0];
	cout<<"Test case two"<<endl;
	cout<<NodeMatrix[1][1].probability<<endl;
	cout<<NodeMatrix[1][1].path<<endl;
}