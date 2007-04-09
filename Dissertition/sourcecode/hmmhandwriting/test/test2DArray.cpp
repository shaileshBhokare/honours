#include <iostream>
#include <fstream>
#include "../convert.h"

namespace rh = redhat;
using namespace std;

int main(){
	string line;
	int row=10;
	int column = 16;
	double num;
	
	ifstream infoFile("../data/trainingData/exampleInfo.txt");
	if(infoFile.good()){
		while(!infoFile.eof()){
			infoFile>>row;
		}
	}
	double matrix[row];
	
	for(int i=0; i<row; i++){
		for(int j=0; j<16; j++){
			cout<<matrix[i][j]<<endl;
		}
	}
}