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
	
	fs::path repository_path("./data/trainingData/localRawData");//point to the trainning data direcotry

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
	string distributionProbabilityFilePath = "./data/trainingData/localInitialData/"+repository_path.leaf()+"_dis.txt";
	string transitionProbabilityFilePath = 	"./data/trainingData/localInitialData/"+repository_path.leaf()+"_tran.txt";
	string featureDirectoryPath = "./data/trainingData/localInitialData/"+repository_path.leaf();
	fs::create_directory(featureDirectoryPath);
	rh::Word newWord;
	bool isFirstFile=true;
	
	fs::ofstream distributionProbabilityFile(distributionProbabilityFilePath);
	fs::ofstream transitionProbabilityFile(transitionProbabilityFilePath);
	if(!distributionProbabilityFile||!transitionProbabilityFile){
		cout << "Cannot write to file.\n";
	}
	
	try{
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
					try{
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
											featureFile<<0<<endl;
										}
										else if(result<(2*PI/8)) {
											tempStroke.state[i].vector[1]++;
											featureFile<<1<<endl;
										}
										else if(result<(3*PI/8)) {
											tempStroke.state[i].vector[2]++;
											featureFile<<2<<endl;
										}
										else if(result<(4*PI/8)) {
											tempStroke.state[i].vector[3]++;
											featureFile<<3<<endl;
										}
										else if(result<(5*PI/8)) {
											tempStroke.state[i].vector[4]++;
											featureFile<<4<<endl;
										}
										else if(result<(6*PI/8)) {
											tempStroke.state[i].vector[5]++;
											featureFile<<5<<endl;
										}
										else if(result<(7*PI/8)) {
											tempStroke.state[i].vector[6]++;
											featureFile<<6<<endl;
										}
										else if(result<(8*PI/8)) {
											tempStroke.state[i].vector[7]++;
											featureFile<<7<<endl;
										}
										else if(result<(9*PI/8)) {
											tempStroke.state[i].vector[8]++;
											featureFile<<8<<endl;
										}
										else if(result<(10*PI/8)) {
											tempStroke.state[i].vector[9]++;
											featureFile<<9<<endl;
										}
										else if(result<(11*PI/8)) {
											tempStroke.state[i].vector[10]++;
											featureFile<<10<<endl;
										}
										else if(result<(12*PI/8)) {
											tempStroke.state[i].vector[11]++;
											featureFile<<11<<endl;
										}
										else if(result<(13*PI/8)) {
											tempStroke.state[i].vector[12]++;
											featureFile<<12<<endl;
										}
										else if(result<(14*PI/8)) {
											tempStroke.state[i].vector[13]++;
											featureFile<<13<<endl;
										}
										else if(result<(15*PI/8)) {
											tempStroke.state[i].vector[14]++;
											featureFile<<14<<endl;
										}
										else {
											tempStroke.state[i].vector[15]++;
											featureFile<<15<<endl;
										}
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
								}else cout<<"Wrong file format: "+sub_itr->leaf()+"\n";
							}
						}
					}catch(...){
						cout<<"Exception when processing file "+sub_itr->leaf()+"\n";
					}
					isFirstFile=false;
				}
				trainingFile.close();
				featureFile.close();
			}
		}
	}catch(...){
		cout<<"Exception when quantilising\n";
	}
	
//	try{
		int featureNumInStroke[50];//store the average number of features in each stroke
		double tranMatrixSource[100];//store the source number of each state
		int featureArrayEnd=0;
		int matrixArrayEnd=0;
		double transitionMatrix[300][300];
		double transitionMatrixNormolisation[300];
//	}catch(...){
//		cout<<"Exception when allocating memory\n";
//	}
	
	try{
		//generate the distribution matrix --begin
		//change the feature data to the probability fomat and output
		for(int i=0; i<newWord.strokes.size(); i++){
			rh::Stroke probabilityStroke = newWord.getStroke(i);
	//		transitionProbabilityFile<<probabilityStroke.getAverageNumOfFeatures()<<endl;
			featureNumInStroke[featureArrayEnd]=probabilityStroke.getAverageNumOfFeatures();
			featureArrayEnd++;
			for(int j=0; j<rh::STATENO; j++){
				double sum = 0;
				int numOfZero = 0;//used to change all the zero to 1/(80*numberOfZero)
				for(int k=0; k<16; k++){
					sum += probabilityStroke.state[j].vector[k];
					if(probabilityStroke.state[j].vector[k]==0){
						numOfZero++;	
					}
				}
				for(int k=0; k<16; k++){
					if(probabilityStroke.state[j].vector[k]==0){
						probabilityStroke.state[j].vector[k] = 1.0/(80*numOfZero);
					}else{
						probabilityStroke.state[j].vector[k] = probabilityStroke.state[j].vector[k]/sum;
					}
					distributionProbabilityFile<<probabilityStroke.state[j].vector[k]<<endl;
				}
			}	
			newWord.replace(probabilityStroke, i);
		}
		//generate the distribution matrix -- end
	}catch(...){
		cout<<"Exception when generating distribution probability"<<endl;	
	}
	//generate the transition matrix -- start
	try{
		for(int i=0; i<featureArrayEnd; i++){
			for(int j=0; j<rh::STATENO-1; j++){
				tranMatrixSource[matrixArrayEnd]=featureNumInStroke[i]/rh::STATENO;
	//			cout<<featureNumInStroke[i]<<"\t";
	//			cout<<tranMatrixSource[matrixArrayEnd]<<endl;
				matrixArrayEnd++;
			}
			tranMatrixSource[matrixArrayEnd]=featureNumInStroke[i]/rh::STATENO+featureNumInStroke[i]%rh::STATENO;
	//		cout<<featureNumInStroke[i]<<"\t";
	//			cout<<tranMatrixSource[matrixArrayEnd]<<endl;
			matrixArrayEnd++;
		}
		
		//initialise transitionMatrixNormolisation
		for(int i=0; i<matrixArrayEnd-1; i++){
			transitionMatrixNormolisation[i]=0;
		}
	
		for(int i=0; i<matrixArrayEnd-1; i++){
	//		cout<<tranMatrixSource[i]<<endl;
			if(tranMatrixSource[i]==0){
				transitionMatrix[i][i]=0;//can be removed, since the default value is zero
			}else if(tranMatrixSource[i]==1){
				transitionMatrix[i][i]=0;
				transitionMatrix[i][i+1]=1;
				transitionMatrixNormolisation[i]+=transitionMatrix[i][i+1];//calculate the normalisation value.
				for(int j=2; j<=rh::JUMPNO; j++){
					transitionMatrix[i][i+j]=0.5*transitionMatrix[i][i+j-1];
					transitionMatrixNormolisation[i]+=transitionMatrix[i][i+j];
				}
			}else{
				transitionMatrix[i][i]=(tranMatrixSource[i]-1)/tranMatrixSource[i];		
				transitionMatrixNormolisation[i]+=transitionMatrix[i][i];
				for(int j=1; j<=rh::JUMPNO; j++){
					transitionMatrix[i][i+j]=0.5*transitionMatrix[i][i+j-1];
					transitionMatrixNormolisation[i]+=transitionMatrix[i][i+j];
				}
			}
//			cout<<i<<"\t"<<transitionMatrix[i][i]<<"\t"<<transitionMatrix[i][i+1]<<"\t"<<tranMatrixSource[i]<<endl;
		}
		
		//normalise the transition matrix
		for(int i=0; i<matrixArrayEnd-1; i++){
			for(int j=0; j<=rh::JUMPNO; j++){
				if(transitionMatrixNormolisation[i]!=0){
					transitionMatrix[i][i+j]*=(1.0/transitionMatrixNormolisation[i]);
				}
			}
		}
		
		transitionMatrix[matrixArrayEnd-1][matrixArrayEnd-1]=1;
		
		for(int i=0; i<matrixArrayEnd; i++){
			for(int j=0; j<matrixArrayEnd; j++){
				transitionProbabilityFile<<transitionMatrix[i][j]<<endl;
	//			cout<<transitionMatrix[i][j]<<endl;
			}	
			if(i!=matrixArrayEnd-1){
				transitionProbabilityFile<<"newRow"<<endl;
			}
		}
		//generate the transition matrix -- end
	}catch(...){
		cout<<"exception when generating transition probability"<<endl;
	}
	
	
	
	
////	distributionProbabilityFile<<"<word>\n";
//	for(int i=0; i<newWord.strokes.size(); i++){
//		rh::Stroke stroke = newWord.strokes.at(i);
////		transitionProbabilityFile<<stroke.featureNum<<endl;
////		transitionProbabilityFile<<stroke.trainingTimes<<endl;
//		transitionProbabilityFile<<stroke.getAverageNumOfFeatures()<<endl;
////		distributionProbabilityFile<<"<stroke>\n";
//		for(int j=0; j<3; j++){
////			distributionProbabilityFile<<"<state>\n";
//
//			for(int k=0; k<16; k++){
//				distributionProbabilityFile<<stroke.state[j].vector[k]<<endl;
//			}	
////			distributionProbabilityFile<<"</state>\n";
//		}
////		distributionProbabilityFile<<"</stroke>\n";
//	}
////	distributionProbabilityFile<<"</word>\n";
	
	distributionProbabilityFile.close();
	transitionProbabilityFile.close();
}