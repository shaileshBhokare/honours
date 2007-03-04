#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include "convert.h"


using namespace std;

int vector[16];
const double PI= 4.0*atan(1.0);

int main(){
	for(int i=0; i<16; i++){
		vector[i] = 0;
//		cout<<vector[i]<<endl;
	}

//	double x1,y1,x2,y2 = -1;
	double x[100], y[100];//use array to stroe the data read from the file.
						  //Using this way, we can just read the file once and use many times.
	string xtemp, ytemp;

	string line;
	char *file = "./data/data.txt";
	
	ifstream dataFile(file);
	if(!dataFile){
		cout << "Cannot open file.\n";
		return 1;
	}else{
		int i = 0;
		while(!dataFile.eof()){
			getline(dataFile, line);
			
			//string handling
			if(line.compare("<s>")==0){
				cout<<"Stroke start\n";
			}else if(line.compare("</s>")==0){
				cout<<"Stroke end\n";
				
			}else{
				int commaPosition = line.find(",");
				
				if(commaPosition != string::npos){
					xtemp = line.substr(0,commaPosition);
					ytemp = line.substr(commaPosition+1);
					
					//cast string to double
					/*if(x1==-1){
						x1 = convertToDouble(xtemp);
						y1 = convertToDouble(ytemp);
					}else{
					x1 = x2;
					y1 = y2;
					x2 = convertToDouble(xtemp);
					y2 = convertToDouble(ytemp);*/
					x[i] = convertToDouble(xtemp);
					y[i] = convertToDouble(ytemp);
					i++;
				}else cout<<"Wrong file format";
			}
					//quantilise the features
					double deltaX = x2-x1;
					double deltaY = y2-y1;
					double result = atan2(deltaY, deltaX);
					
					if(result<0){
						result = result + 2*PI;
					}
					
					if(result<(PI/8)) vector[0]++;
					else if(result<(2*PI/8)) vector[1]++;
					else if(result<(3*PI/8)) vector[2]++;
					else if(result<(4*PI/8)) vector[3]++;
					else if(result<(5*PI/8)) vector[4]++;
					else if(result<(6*PI/8)) vector[5]++;
					else if(result<(7*PI/8)) vector[6]++;
					else if(result<(8*PI/8)) vector[7]++;
					else if(result<(9*PI/8)) vector[8]++;
					else if(result<(10*PI/8)) vector[9]++;
					else if(result<(11*PI/8)) vector[10]++;
					else if(result<(12*PI/8)) vector[11]++;
					else if(result<(13*PI/8)) vector[12]++;
					else if(result<(14*PI/8)) vector[13]++;
					else if(result<(15*PI/8)) vector[14]++;
					else vector[15]++;
//					}
				
			}
		}
		dataFile.close();
		for(int i=0 ; i<16; i++){
			cout<<vector[i]<<endl;
		}
	}
	
	return 0;
}
