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
	string outFilePath = "./data/probability/"+repository_path.leaf()+".txt";
	string transitionProbabilityFilePath = "./data/transitionProbability/"+repository_path.leaf()+".txt";
	string featureDirectoryPath = "./data/featureData/"+repository_path.leaf();
	fs::create_directory(featureDirectoryPath);
	rh::Word newWord;
	bool isFirstFile=true;
	
	fs::ofstream outFile(outFilePath);
	fs::ofstream transitionProbabilityFile(transitionProbabilityFilePath);
	if(!outFile||!transitionProbabilityFile){
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
						tempStroke.featureNum += count; // calculate trasition probabilityy
						tempStroke.trainingTimes += 1;
						//first state
						for (int i=0; i<(count/3); i++){
							double deltaX = x[i+1]-x[i];
							double deltaY = y[i+1]-y[i];
							double result = atan2(deltaY, deltaX);
							
							if(result<0){
								result = result + 2*PI;
							}
							
							if(result<(PI/8)) {
								tempStroke.state[0].vector[0]++;
								featureFile<<0<<endl;
							}
							else if(result<(2*PI/8)) {
								tempStroke.state[0].vector[1]++;
								featureFile<<1<<endl;
							}
							else if(result<(3*PI/8)) {
								tempStroke.state[0].vector[2]++;
								featureFile<<2<<endl;
							}
							else if(result<(4*PI/8)) {
								tempStroke.state[0].vector[3]++;
								featureFile<<3<<endl;
							}
							else if(result<(5*PI/8)) {
								tempStroke.state[0].vector[4]++;
								featureFile<<4<<endl;
							}
							else if(result<(6*PI/8)) {
								tempStroke.state[0].vector[5]++;
								featureFile<<5<<endl;
							}
							else if(result<(7*PI/8)) {
								tempStroke.state[0].vector[6]++;
								featureFile<<6<<endl;
							}
							else if(result<(8*PI/8)) {
								tempStroke.state[0].vector[7]++;
								featureFile<<7<<endl;
							}
							else if(result<(9*PI/8)) {
								tempStroke.state[0].vector[8]++;
								featureFile<<8<<endl;
							}
							else if(result<(10*PI/8)) {
								tempStroke.state[0].vector[9]++;
								featureFile<<9<<endl;
							}
							else if(result<(11*PI/8)) {
								tempStroke.state[0].vector[10]++;
								featureFile<<10<<endl;
							}
							else if(result<(12*PI/8)) {
								tempStroke.state[0].vector[11]++;
								featureFile<<11<<endl;
							}
							else if(result<(13*PI/8)) {
								tempStroke.state[0].vector[12]++;
								featureFile<<12<<endl;
							}
							else if(result<(14*PI/8)) {
								tempStroke.state[0].vector[13]++;
								featureFile<<13<<endl;
							}
							else if(result<(15*PI/8)) {
								tempStroke.state[0].vector[14]++;
								featureFile<<14<<endl;
							}
							else {
								tempStroke.state[0].vector[15]++;
								featureFile<<15<<endl;
							}
						}
						
						//second state
						for(int i=(count/3); i<(2*count/3); i++){
							double deltaX = x[i+1]-x[i];
							double deltaY = y[i+1]-y[i];
							double result = atan2(deltaY, deltaX);
							
							if(result<0){
								result = result + 2*PI;
							}
							
							if(result<(PI/8)) {
								tempStroke.state[1].vector[0]++;
								featureFile<<0<<endl;
							}
							else if(result<(2*PI/8)) {
								tempStroke.state[1].vector[1]++;
								featureFile<<1<<endl;
							}
							else if(result<(3*PI/8)) {
								tempStroke.state[1].vector[2]++;
								featureFile<<2<<endl;
							}
							else if(result<(4*PI/8)) {
								tempStroke.state[1].vector[3]++;
								featureFile<<3<<endl;
							}
							else if(result<(5*PI/8)) {
								tempStroke.state[1].vector[4]++;
								featureFile<<4<<endl;
							}
							else if(result<(6*PI/8)) {
								tempStroke.state[1].vector[5]++;
								featureFile<<5<<endl;
							}
							else if(result<(7*PI/8)) {
								tempStroke.state[1].vector[6]++;
								featureFile<<6<<endl;
							}
							else if(result<(8*PI/8)) {
								tempStroke.state[1].vector[7]++;
								featureFile<<7<<endl;
							}
							else if(result<(9*PI/8)) {
								tempStroke.state[1].vector[8]++;
								featureFile<<8<<endl;
							}
							else if(result<(10*PI/8)) {
								tempStroke.state[1].vector[9]++;
								featureFile<<9<<endl;
							}
							else if(result<(11*PI/8)) {
								tempStroke.state[1].vector[10]++;
								featureFile<<10<<endl;
							}
							else if(result<(12*PI/8)) {
								tempStroke.state[1].vector[11]++;
								featureFile<<11<<endl;
							}
							else if(result<(13*PI/8)) {
								tempStroke.state[1].vector[12]++;
								featureFile<<12<<endl;
							}
							else if(result<(14*PI/8)) {
								tempStroke.state[1].vector[13]++;
								featureFile<<13<<endl;
							}
							else if(result<(15*PI/8)) {
								tempStroke.state[1].vector[14]++;
								featureFile<<14<<endl;
							}
							else {
								tempStroke.state[1].vector[15]++;
								featureFile<<15<<endl;
							}
						}
						
						//third state
						for(int i=(2*count/3); i<count; i++){
							double deltaX = x[i+1]-x[i];
							double deltaY = y[i+1]-y[i];
							double result = atan2(deltaY, deltaX);
							
							if(result<0){
								result = result + 2*PI;
							}
							
							if(result<(PI/8)) {
								tempStroke.state[2].vector[0]++;
								featureFile<<0<<endl;
							}
							else if(result<(2*PI/8)) {
								tempStroke.state[2].vector[1]++;
								featureFile<<1<<endl;
							}
							else if(result<(3*PI/8)) {
								tempStroke.state[2].vector[2]++;
								featureFile<<2<<endl;
							}
							else if(result<(4*PI/8)) {
								tempStroke.state[2].vector[3]++;
								featureFile<<3<<endl;
							}
							else if(result<(5*PI/8)) {
								tempStroke.state[2].vector[4]++;
								featureFile<<4<<endl;
							}
							else if(result<(6*PI/8)) {
								tempStroke.state[2].vector[5]++;
								featureFile<<5<<endl;
							}
							else if(result<(7*PI/8)) {
								tempStroke.state[2].vector[6]++;
								featureFile<<6<<endl;
							}
							else if(result<(8*PI/8)) {
								tempStroke.state[2].vector[7]++;
								featureFile<<7<<endl;
							}
							else if(result<(9*PI/8)) {
								tempStroke.state[2].vector[8]++;
								featureFile<<8<<endl;
							}
							else if(result<(10*PI/8)) {
								tempStroke.state[2].vector[9]++;
								featureFile<<9<<endl;
							}
							else if(result<(11*PI/8)) {
								tempStroke.state[2].vector[10]++;
								featureFile<<10<<endl;
							}
							else if(result<(12*PI/8)) {
								tempStroke.state[2].vector[11]++;
								featureFile<<11<<endl;
							}
							else if(result<(13*PI/8)) {
								tempStroke.state[2].vector[12]++;
								featureFile<<12<<endl;
							}
							else if(result<(14*PI/8)) {
								tempStroke.state[2].vector[13]++;
								featureFile<<13<<endl;
							}
							else if(result<(15*PI/8)) {
								tempStroke.state[2].vector[14]++;
								featureFile<<14<<endl;
							}
							else {
								tempStroke.state[2].vector[15]++;
								featureFile<<15<<endl;
							}
						}
						newWord.replace(tempStroke, strokeNum-1);
//						featureFile<<"</s>"<<endl;
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
	
	int featureNumInStroke[50];//store the average number of features in each stroke
	double matrixSource[100];//store the source number of each state
	int featureArrayEnd=0;
	int matrixArrayEnd=0;
	double transitionMatrix[100][100];
	//change the feature data to the probability fomat and output
	for(int i=0; i<newWord.strokes.size(); i++){
		rh::Stroke probabilityStroke = newWord.getStroke(i);
//		transitionProbabilityFile<<probabilityStroke.getAverageNumOfFeatures()<<endl;
		featureNumInStroke[featureArrayEnd]=probabilityStroke.getAverageNumOfFeatures();
		featureArrayEnd++;
		for(int j=0; j<3; j++){
			double sum = 0;
			for(int k=0; k<16; k++){
				sum += probabilityStroke.state[j].vector[k];
			}
			for(int k=0; k<16; k++){
				probabilityStroke.state[j].vector[k] = probabilityStroke.state[j].vector[k]/sum;
				outFile<<probabilityStroke.state[j].vector[k]<<endl;
			}
		}	
		newWord.replace(probabilityStroke, i);
	}
	
	for(int i=0; i<featureArrayEnd; i++){
		matrixSource[matrixArrayEnd]=featureNumInStroke[i]/3;
		matrixArrayEnd++;
		matrixSource[matrixArrayEnd]=featureNumInStroke[i]/3;
		matrixArrayEnd++;
		matrixSource[matrixArrayEnd]=featureNumInStroke[i]/3+featureNumInStroke[i]%3;
		matrixArrayEnd++;
	}
	
	//generate the transition matrix -- start
	for(int i=0; i<matrixArrayEnd-1; i++){
		cout<<matrixSource[i]<<endl;
		transitionMatrix[i][i]=(matrixSource[i]-1)/matrixSource[i];
		transitionMatrix[i][i+1]=1/matrixSource[i];
	}
	transitionMatrix[matrixArrayEnd-1][matrixArrayEnd-1]=1;
	
	for(int i=0; i<matrixArrayEnd; i++){
		for(int j=0; j<matrixArrayEnd; j++){
			transitionProbabilityFile<<transitionMatrix[i][j]<<endl;
			cout<<transitionMatrix[i][j]<<endl;
		}	
		if(i!=matrixArrayEnd-1){
			transitionProbabilityFile<<"newRow"<<endl;
		}
	}
	//generate the transition matrix -- end
	
	
	
	
////	outFile<<"<word>\n";
//	for(int i=0; i<newWord.strokes.size(); i++){
//		rh::Stroke stroke = newWord.strokes.at(i);
////		transitionProbabilityFile<<stroke.featureNum<<endl;
////		transitionProbabilityFile<<stroke.trainingTimes<<endl;
//		transitionProbabilityFile<<stroke.getAverageNumOfFeatures()<<endl;
////		outFile<<"<stroke>\n";
//		for(int j=0; j<3; j++){
////			outFile<<"<state>\n";
//
//			for(int k=0; k<16; k++){
//				outFile<<stroke.state[j].vector[k]<<endl;
//			}	
////			outFile<<"</state>\n";
//		}
////		outFile<<"</stroke>\n";
//	}
////	outFile<<"</word>\n";
	
	outFile.close();
	transitionProbabilityFile.close();
}
