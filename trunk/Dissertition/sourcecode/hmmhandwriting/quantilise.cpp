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
	
	fs::path repository_path("./data/tempTrainingData");//point to the trainning data direcotry



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
	string outFilePath = "./data/probability/"+repository_path.leaf()+".xml";
	rh::Word newWord;
	bool isFirstFile=true;
	
	fs::ofstream outFile(outFilePath);
	if(!outFile){
		cout << "Cannot write to file.\n";
	}
	
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
			
			int count; //used to divide each stroke into three states
			int strokeNum=0; //used to retrieve specific stroke from the word
			
			fs::ifstream trainingFile(*sub_itr);

			if(!trainingFile){
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
					}else if(line.compare("</s>")==0){
						
						rh::Stroke tempStroke = newWord.getStroke(strokeNum-1);
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
							x[count] = rh::convertToDouble(xtemp);
							y[count] = rh::convertToDouble(ytemp);
							count++;
						}else cout<<"Wrong file format";
					}
				}
				isFirstFile=false;
			}
			trainingFile.close();
		}
	}
	
	//change the count number to the probability fomat
	for(int i=0; i<newWord.strokes.size(); i++){
		rh::Stroke probabilityStroke = newWord.getStroke(i);
		for(int j=0; j<3; j++){
			double sum = 0;
			for(int k=0; k<16; k++){
				sum += probabilityStroke.state[j].vector[k];
			}
			for(int k=0; k<16; k++){
				probabilityStroke.state[j].vector[k] = probabilityStroke.state[j].vector[k]/sum;
			}
		}	
		newWord.replace(probabilityStroke, i);
	}
	outFile<<"<word>\n";

	for(int i=0; i<newWord.strokes.size(); i++){
		rh::Stroke stroke = newWord.strokes.at(i);
		outFile<<"<stroke>\n";
		
		for(int j=0; j<3; j++){
			outFile<<"<state>\n";

			for(int k=0; k<16; k++){
				outFile<<stroke.state[j].vector[k]<<endl;
			}
			
			outFile<<"</state>\n";
		}
		
		outFile<<"</stroke>\n";
	}
	
	outFile<<"</word>\n";
	
	outFile.close();
}
