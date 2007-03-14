#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include "convert.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
//#include "state.h"
//#include "stroke.h"

namespace fs = boost::filesystem;
using namespace std;

double vector[16];
const double PI= 4.0*atan(1.0);

int main(){
	//initialise the array
	for(int i=0; i<16; i++){
		vector[i] = 0;
	}
	
	fs::path repository_path("./data/trainingData");//point to the trainning data direcotry
	fs::path trainingFile_path;//current working file

	double x[100], y[100];//use array to stroe the data read from the file.
//						  //Using this way, we can just read the file once and use many times.
	string xtemp, ytemp;
	
	string line;//used to retrieve each in a file
		
	//open file exception handling
	if(!fs::exists(repository_path)){
		cout<<"Cannot read the direcotry"<<endl;
		return 1;
	}
	
	fs::directory_iterator end_itr;//find out the end of direcotry
	for(fs::directory_iterator itr(repository_path); itr!=end_itr; ++itr){
		if(is_director(*itr)){
			//handle subdirectory and retrieve the name
			string outFilePath = "./data/probability/"+itr->leaf+".txt";
			fs::ofstream outFile(outFilePath);
			if(!outFile){
				cout << "Cannot write to file.\n";
				return 1;
			}
			//trversal the subdirecotry
			fs::directory_iterator end_sub_itr;
			for(fs::direcotry_iterator sub_itr(itr); sub_itr!=end_sub_itr; ++sub_itr){
				if(is_director(*sub_itr)){
					//do nothing	
				}else{
					fs::ifstream trainingFile(*sub_itr);
					if(!trainingFile){
						cout<<"Cannot open file.\n";
						return 1;
					}else{
						int count;
					}
				}
			}
		}
	}
	
//	for(int i=0; i<8; i++){

//		char *inFile = "./data/trainingData/4.8.txt";
//		char *outFile = "./data/probability/4.8.txt";
		
//		ifstream dataFile(inFile);
//		ofstream probabilityFile(outFile);
//		if(!dataFile){
//			cout << "Cannot open file.\n";
//			return 1;
//		}else if(!probabilityFile){
//			cout << "Cannot write to file.\n";
//			return 1;
		}else{
			int count; //use to count the number of the lines in each stroke
			
			while(!dataFile.eof()){
				getline(dataFile, line);
				
				//string handling
				if(line.compare("<s>")==0){
					cout<<"Stroke start\n";
					count = 0; //reset the count
				}else if(line.compare("</s>")==0){
					cout<<"Stroke end\n";
					//start calculate the probability
	//				State state[3];
	//				Stroke stroke;
					double sum[3] = {0,0,0};
					//process three states in each stroke
					
					//write starting tags
					probabilityFile << "<s>\n";
					probabilityFile << "<state>\n";
					
					//first state
					for (int i=0; i<(count/3); i++){
						double deltaX = x[i+1]-x[i];
						double deltaY = y[i+1]-y[i];
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
					}
					
					for(int i=0; i<16; i++){
						sum[0] += vector[i];
					}
					
					for(int i=0; i<16; i++){
						double prob = vector[i]/sum[0];
	//					state[0].vector[i] = prob;
						probabilityFile << i << "," << prob << endl;
					}
					
					for(int i=0; i<16; i++){
							vector[i] = 0;
					}
					probabilityFile << "</state>\n";
					probabilityFile << "<state>\n";
					//second state
					for(int i=(count/3); i<(2*count/3); i++){
						double deltaX = x[i+1]-x[i];
						double deltaY = y[i+1]-y[i];
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
					}
					
					for(int i=0; i<16; i++){
						sum[1] += vector[i];
					}
	//				
					for(int i=0; i<16; i++){
						double prob = vector[i]/sum[1];
	//					state[1].vector[i] = prob;
						probabilityFile << i << "," << prob << endl;
					}
	//				
					for(int i=0; i<16; i++){
							vector[i] = 0;
					}
					probabilityFile << "</state>\n";
					probabilityFile << "<state>\n";
					//third state
					for(int i=(2*count/3); i<count; i++){
						double deltaX = x[i+1]-x[i];
						double deltaY = y[i+1]-y[i];
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
					}
					
					for(int i=0; i<16; i++){
						sum[2] += vector[i];
					}
					
					for(int i=0; i<16; i++){
						double prob = vector[i]/sum[2];
	//					state[2].vector[i] = prob;
						probabilityFile << i << "," << prob << endl;
					}
					
					for(int i=0; i<16; i++){
							vector[i] = 0;
					}
	//				
	//				stroke.state[0] = state[0];
	//				stroke.state[1] = state[1];
	//				stroke.state[2] = state[2];
					
					//store the stroke object in files.
					
					
					//write end tags
					probabilityFile << "</state>\n";
					probabilityFile << "</s>\n";
				}else{
					//start stroke handling
					
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
						x[count] = convertToDouble(xtemp);
						y[count] = convertToDouble(ytemp);
						count++;
					}else cout<<"Wrong file format";
				}	
			}
			dataFile.close();
			probabilityFile.close();
		}
		
//	}
	
	
	
	return 0;
}
