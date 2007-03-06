#include <iostream>
#include <string>
#include <fstream>
#include "convert.h"

using namespace std;

int main(){
	//create eight input probability files
	char *inFile1 = "./data/probability/4.1.txt";
	char *inFile2 = "./data/probability/4.2.txt";
	char *inFile3 = "./data/probability/4.3.txt";
	char *inFile4 = "./data/probability/4.4.txt";
	char *inFile5 = "./data/probability/4.5.txt";
	char *inFile6 = "./data/probability/4.6.txt";
	char *inFile7 = "./data/probability/4.7.txt";
	char *inFile8 = "./data/probability/4.8.txt";
	//create the output file
	char *outFile = "./data/probability/4.txt";
	
	ifstream probFile1(inFile1);
	ifstream probFile2(inFile2);
	ifstream probFile3(inFile3);
	ifstream probFile4(inFile4);
	ifstream probFile5(inFile5);
	ifstream probFile6(inFile6);
	ifstream probFile7(inFile7);
	ifstream probFile8(inFile8);
	ofstream aveFile(outFile);
	
	
	string line;
	
	
	return 0;
}
	