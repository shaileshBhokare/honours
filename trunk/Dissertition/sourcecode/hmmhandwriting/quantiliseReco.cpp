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

void parseFile(fs::path repository_path);

int main(){
	fs::path repository_path("./data/recognitionData/localRawData");//point to the trainning data direcotry

	//open file exception handling
	if(!fs::exists(repository_path)){
		cout<<"Cannot read the direcotry"<<endl;
	}
	
	fs::directory_iterator end_itr;//find out the end of direcotry
	
	for(fs::directory_iterator itr(repository_path); itr!=end_itr; ++itr){	//each directory represent one character
		if(fs::is_directory(*itr)){
			//TEST
			parseFile(*itr);
		}
	}

	return 0;
}

void parseFile(fs::path repository_path){//handle subdirectory and retrieve the name
	string featureDirectoryPath = "./data/recognitionData/localFeatureData/"+repository_path.leaf();
	fs::create_directory(featureDirectoryPath);
	rh::Word newWord;
	bool isFirstFile=true;
	
	//trversal the subdirecotry
	fs::directory_iterator end_sub_itr;
	for(fs::directory_iterator sub_itr(repository_path); sub_itr!=end_sub_itr; ++sub_itr){	//each file represent a training data file
		if(is_directory(*sub_itr)){
			//do nothing	
		}else{//parse each file
			double x[100], y[100];/* use array to stroe the data read from the file.
								   * Using this way, we can just read the file once and use many times.
								   */
			string xtemp, ytemp;
	
			string line;//used to retrieve each line in a file
			
			int count; //used to divide each stroke into several states
			int strokeNum=0; //used to retrieve specific stroke from the word
			
			fs::ifstream trainingFile(*sub_itr);
			string featureFilePath = featureDirectoryPath+"/"+sub_itr->leaf();
			fs::ofstream featureFile(featureFilePath);
			
			if(!trainingFile||!featureFile){
				cout<<"Cannot open file.\n";
			}else{
				while(!trainingFile.eof()){
					getline(trainingFile, line);
					if(line.compare("<s>")==0){
						count = 0; //reset the count
						strokeNum++;
						if(isFirstFile){
							newWord.increaseStroke();
						}
//						featureFile<<"<s>"<<endl;
					}else if(line.compare("</s>")==0){
						rh::Stroke tempStroke = newWord.getStroke(strokeNum-1);
						
						for(int i=0; i<rh::STATENO; i++){
							for (int j=((i*count)/rh::STATENO); j<(((i+1)*count)/rh::STATENO); j++){
								double deltaX = x[j+1]-x[j];
								double deltaY = y[j+1]-y[j];
								double result = atan2(deltaY, deltaX);
								
								if(result<0){
									result = result + 2*PI;
								}
								
								if(result<(PI/8)) {
									tempStroke.state[i].vector[0]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<16<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-16<<endl;
									}else featureFile<<0<<endl;
								}
								else if(result<(2*PI/8)) {
									tempStroke.state[i].vector[1]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<17<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-15<<endl;
									}else featureFile<<1<<endl;
								}
								else if(result<(3*PI/8)) {
									tempStroke.state[i].vector[2]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<18<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-14<<endl;
									}else featureFile<<2<<endl;
								}
								else if(result<(4*PI/8)) {
									tempStroke.state[i].vector[3]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<19<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-13<<endl;
									}else featureFile<<3<<endl;
								}
								else if(result<(5*PI/8)) {
									tempStroke.state[i].vector[4]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<20<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-12<<endl;
									}else featureFile<<4<<endl;
								}
								else if(result<(6*PI/8)) {
									tempStroke.state[i].vector[5]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<21<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-11<<endl;
									}else featureFile<<5<<endl;
								}
								else if(result<(7*PI/8)) {
									tempStroke.state[i].vector[6]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<22<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-10<<endl;
									}else featureFile<<6<<endl;
								}
								else if(result<(8*PI/8)) {
									tempStroke.state[i].vector[7]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<23<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-9<<endl;
									}else featureFile<<7<<endl;
								}
								else if(result<(9*PI/8)) {
									tempStroke.state[i].vector[8]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<24<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-8<<endl;
									}else featureFile<<8<<endl;
								}
								else if(result<(10*PI/8)) {
									tempStroke.state[i].vector[9]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<25<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-7<<endl;
									}else featureFile<<9<<endl;
								}
								else if(result<(11*PI/8)) {
									tempStroke.state[i].vector[10]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<26<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-6<<endl;
									}else featureFile<<10<<endl;
								}
								else if(result<(12*PI/8)) {
									tempStroke.state[i].vector[11]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<27<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-5<<endl;
									}else featureFile<<11<<endl;
								}
								else if(result<(13*PI/8)) {
									tempStroke.state[i].vector[12]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<28<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-4<<endl;
									}else featureFile<<12<<endl;
								}
								else if(result<(14*PI/8)) {
									tempStroke.state[i].vector[13]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<29<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-3<<endl;
									}else featureFile<<13<<endl;
								}
								else if(result<(15*PI/8)) {
									tempStroke.state[i].vector[14]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<30<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-2<<endl;
									}else featureFile<<14<<endl;
								}
								else {
									tempStroke.state[i].vector[15]++;
									if(i==0&&j==(i*count)/rh::STATENO){
										featureFile<<31<<endl;
									}else if(i==(rh::STATENO-1)&&j==((i+1)*count)/rh::STATENO-1){
										featureFile<<-1<<endl;
									}else featureFile<<15<<endl;
								}
							}
						}
						
						newWord.replace(tempStroke, strokeNum-1);
					}else{
						//start stroke handling
						int commaPosition = line.find(",");
						
						if(commaPosition != string::npos){
							xtemp = line.substr(0,commaPosition);
							ytemp = line.substr(commaPosition+1);
							x[count] = rh::convertToDouble(xtemp);
							y[count] = rh::convertToDouble(ytemp);
							count++;
						}else cout<<"Wrong file format";
					}
				}
				isFirstFile=false;
			}
			trainingFile.close();
			featureFile.close();
		}
	}
}