#include <iostream>
#include <string>
#include <math.h>
#include "convert.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include "State.h"
#include "Stroke.h"
#include "Word.h"

namespace fs = boost::filesystem;
namespace rh = redhat;
using namespace std;

const double PI= 4.0*atan(1.0);

int main(){
	
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
		if(fs::is_director(*itr)){
			//handle subdirectory and retrieve the name
			string outFilePath = "./data/probability/"+itr->leaf+".txt";
			rh::Word newWord;
			
			fs::ofstream outFile(outFilePath);
			if(!outFile){
				cout << "Cannot write to file.\n";
				return 1;
			}
			//trversal the subdirecotry
			fs::directory_iterator end_sub_itr;
			for(fs::directory_iterator sub_itr(itr); sub_itr!=end_sub_itr; ++sub_itr){
				if(is_director(*sub_itr)){
					//do nothing	
				}else{
					fs::ifstream trainingFile(*sub_itr);
					bool isFirstFile=true;
					if(!trainingFile){
						cout<<"Cannot open file.\n";
						return 1;
					}else{
						int count;
						int strokeNum=0;

						while(!trainingFile.eof()){
							getline(trainingFile, line);
							if(line.compare("<s>")==0){
								cout<<"Stroke start\n";
								count = 0; //reset the count
								strokeNum++;
								if(isFirstFile){
									newWord.increaseStroke();
									isFirstFile=false;
								}
							}else if(line.compare("</s>")==0){
								cout<<"Stroke end\n";	
								
								rh::Stroke tempStroke = newWord.getStroke(strokeNum-1);
								//TODO: calcualte the overral probability
								//TODO: write word to file
								//first state
								for (int i=0; i<(count/3); i++){
									double deltaX = x[i+1]-x[i];
									double deltaY = y[i+1]-y[i];
									double result = atan2(deltaY, deltaX);
									
									if(result<0){
										result = result + 2*PI;
									}
									
									if(result<(PI/8)) tempStroke.state[0].vector[0]++;
									else if(result<(2*PI/8)) tempStroke.state[0].vector[1]++;
									else if(result<(3*PI/8)) tempStroke.state[0].vector[2]++;
									else if(result<(4*PI/8)) tempStroke.state[0].vector[3]++;
									else if(result<(5*PI/8)) tempStroke.state[0].vector[4]++;
									else if(result<(6*PI/8)) tempStroke.state[0].vector[5]++;
									else if(result<(7*PI/8)) tempStroke.state[0].vector[6]++;
									else if(result<(8*PI/8)) tempStroke.state[0].vector[7]++;
									else if(result<(9*PI/8)) tempStroke.state[0].vector[8]++;
									else if(result<(10*PI/8)) tempStroke.state[0].vector[9]++;
									else if(result<(11*PI/8)) tempStroke.state[0].vector[10]++;
									else if(result<(12*PI/8)) tempStroke.state[0].vector[11]++;
									else if(result<(13*PI/8)) tempStroke.state[0].vector[12]++;
									else if(result<(14*PI/8)) tempStroke.state[0].vector[13]++;
									else if(result<(15*PI/8)) tempStroke.state[0].vector[14]++;
									else tempStroke.state[0].vector[15]++;
								}
								
								//second state
								for(int i=(count/3); i<(2*count/3); i++){
									double deltaX = x[i+1]-x[i];
									double deltaY = y[i+1]-y[i];
									double result = atan2(deltaY, deltaX);
									
									if(result<0){
										result = result + 2*PI;
									}
									
									if(result<(PI/8)) tempStroke.state[1].vector[0]++;
									else if(result<(2*PI/8)) tempStroke.state[1].vector[1]++;
									else if(result<(3*PI/8)) tempStroke.state[1].vector[2]++;
									else if(result<(4*PI/8)) tempStroke.state[1].vector[3]++;
									else if(result<(5*PI/8)) tempStroke.state[1].vector[4]++;
									else if(result<(6*PI/8)) tempStroke.state[1].vector[5]++;
									else if(result<(7*PI/8)) tempStroke.state[1].vector[6]++;
									else if(result<(8*PI/8)) tempStroke.state[1].vector[7]++;
									else if(result<(9*PI/8)) tempStroke.state[1].vector[8]++;
									else if(result<(10*PI/8)) tempStroke.state[1].vector[9]++;
									else if(result<(11*PI/8)) tempStroke.state[1].vector[10]++;
									else if(result<(12*PI/8)) tempStroke.state[1].vector[11]++;
									else if(result<(13*PI/8)) tempStroke.state[1].vector[12]++;
									else if(result<(14*PI/8)) tempStroke.state[1].vector[13]++;
									else if(result<(15*PI/8)) tempStroke.state[1].vector[14]++;
									else tempStroke.state[1].vector[15]++;
								}
								
								//third state
								for(int i=(2*count/3); i<count; i++){
									double deltaX = x[i+1]-x[i];
									double deltaY = y[i+1]-y[i];
									double result = atan2(deltaY, deltaX);
									
									if(result<0){
										result = result + 2*PI;
									}
									
									if(result<(PI/8)) tempStroke.state[2].vector[0]++;
									else if(result<(2*PI/8)) tempStroke.state[2].vector[1]++;
									else if(result<(3*PI/8)) tempStroke.state[2].vector[2]++;
									else if(result<(4*PI/8)) tempStroke.state[2].vector[3]++;
									else if(result<(5*PI/8)) tempStroke.state[2].vector[4]++;
									else if(result<(6*PI/8)) tempStroke.state[2].vector[5]++;
									else if(result<(7*PI/8)) tempStroke.state[2].vector[6]++;
									else if(result<(8*PI/8)) tempStroke.state[2].vector[7]++;
									else if(result<(9*PI/8)) tempStroke.state[2].vector[8]++;
									else if(result<(10*PI/8)) tempStroke.state[2].vector[9]++;
									else if(result<(11*PI/8)) tempStroke.state[2].vector[10]++;
									else if(result<(12*PI/8)) tempStroke.state[2].vector[11]++;
									else if(result<(13*PI/8)) tempStroke.state[2].vector[12]++;
									else if(result<(14*PI/8)) tempStroke.state[2].vector[13]++;
									else if(result<(15*PI/8)) tempStroke.state[2].vector[14]++;
									else tempStroke.state[2].vector[15]++;
								}
								
								newWord.replace(tempStroke, strokeNum-1);
								
							}else{
								//start stroke handling
								int commaPosition = line.find(",");
								
								if(commaPosition != string::npos){
									xtemp = line.substr(0,commaPosition);
									ytemp = line.substr(commaPosition+1);
									x[count] = convertToDouble(xtemp);
									y[count] = convertToDouble(ytemp);
									count++;
								}else cout<<"Wrong file format";
							}
						}
					}
					trainingFile.close();
				}
			}
			newWord.display();
			outFile.close();
		}
	}

	return 0;
}
