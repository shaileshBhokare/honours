#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include "State.h"
#include "Stroke.h"
#include "Viterbi.h"
#include "ViterbiResult.h"

namespace fs = boost::filesystem;
namespace rh = redhat;
using namespace std;

void parseFile(fs::path repository_path, string disProbFilePath, string tranProbFilePath);

int main(){
	fs::path repository_path("./data/trainingData/localInitialData/");
	
	if(!fs::exists(repository_path)){
		cout<<"Cannot read the direcotry"<<endl;
	}
	
	fs::directory_iterator end_itr;
	
	for(fs::directory_iterator itr(repository_path); itr!=end_itr; ++itr){	//each directory represent one character
		if(fs::is_directory(*itr)){
			//TEST
			string disPath = "./data/trainingData/localInitialData/"+itr->leaf()+"_dis.txt";
			string tranPath = "./data/trainingData/localInitialData/"+itr->leaf()+"_tran.txt";
			parseFile(*itr, disPath, tranPath);
		}
	}
	
	return 0;
}

void parseFile(fs::path repository_path, string disProbFilePath, string tranProbFilePath){
	rh::State state[150];
	int feature[300];
	int numOfState = 0;
	int numOfFeature = 0;
	int tranState[150]; //used to calculate transition probabilityy
	int trainingTimes = 0; //used to calculate transition probability
	double optimisedTranMatrixSource[100]; 
	double optimisedTransitionMatrix[100][100];// = new double[100][100];
	
//	for(int i=0; i<15; i++){
//		for(int j=0; j<15; j++){
//			cout<<optimisedTransitionMatrix[i][j]<<"\t";
//		}
//		cout<<endl;
//	}
	
	//trversal the subdirecotry
	fs::directory_iterator end_sub_itr;
	for(fs::directory_iterator sub_itr(repository_path); sub_itr!=end_sub_itr; ++sub_itr){
		if(is_directory(*sub_itr)){
			//do nothing
		}else{
			string observationPath = "./data/trainingData/localInitialData/"+repository_path.leaf()+"/"+sub_itr->leaf();
			
			string line;
			fs::ifstream observationFile(observationPath);
			if(!observationFile){
				cout<<"Cannot open file.\n";
			}else{
				while(!observationFile.eof()){
					getline(observationFile, line);
					if(line.compare("")==0){//do nothing
					}else{
						feature[numOfFeature]=rh::convertToInt(line);
						numOfFeature++;
					}
				}
			}
						
			rh::ViterbiResult result = rh::Viterbi::Calculate_path_and_probability(disProbFilePath, observationPath, tranProbFilePath);
			
			//intermedia value -- start
			string stateSequanceDirectoryPath = "./data/trainingData/localOptimisedData/"+repository_path.leaf();
			fs::create_directory(stateSequanceDirectoryPath);
			string stateSequencePath = stateSequanceDirectoryPath+"/"+sub_itr->leaf();
			fs::ofstream stateSequence(stateSequencePath);
			if(!stateSequence){
				cout<<"Cannot open file!"<<endl;
			}
			for(int i=0; i<result.path.size(); i++){
				stateSequence<<result.path.at(i)<<endl;
				tranState[result.path.at(i)]++;//calculate the total number of the feature in each state
			}
			stateSequence.close();//intermedia value -- end
			
			for(int i=0; i<result.path.size(); i++){
				int stateIndex = result.path.at(i);
				if(stateIndex > numOfState){ 
					numOfState = stateIndex;
				}
				state[stateIndex].vector[feature[stateIndex]]++;
			}
		}
		trainingTimes++;
//		//tst
//		cout<<trainingTimes<<endl;
//		for(int i=0; i<15; i++){
//			for(int j=0; j<15; j++){
//				cout<<optimisedTransitionMatrix[i][j]<<"\t";
//			}
//							cout<<endl;
//		}
//		//tst end
	}
	
	fs::ofstream optimisedDistributionFile("./data/trainingData/localOptimisedData/"+repository_path.leaf()+"_dis.txt");
	//calculate distribution probability
	for(int i=0; i<=numOfState; i++){
		double sum = 0;
		for(int k=0; k<16; k++){
			sum += state[i].vector[k];
		}
		for(int k=0; k<16; k++){
			state[i].vector[k] = state[i].vector[k]/sum;
			optimisedDistributionFile<<state[i].vector[k]<<endl;
		}
	}
	
	//calculate the transition probability
	for(int i=0; i<=numOfState; i++){
		optimisedTranMatrixSource[i] = tranState[i]/trainingTimes;
//		cout<<"feature: "<<tranState[i]<<endl;
//		cout<<"training times: "<<trainingTimes<<endl;
//		cout<<"average :"<<optimisedTranMatrixSource[i]<<endl;
	}

	//initilised optimisedTransitionMatrix
	for(int i=0; i<100; i++){
		for(int j=0; j<100; j++){
			optimisedTransitionMatrix[i][j]=0;
		}
	}
	for(int i=0; i<=numOfState-1; i++){
		optimisedTransitionMatrix[i][i]=(optimisedTranMatrixSource[i]-1)/optimisedTranMatrixSource[i];
		optimisedTransitionMatrix[i][i+1]=1/optimisedTranMatrixSource[i];
	}
	
	optimisedTransitionMatrix[numOfState][numOfState]=1;
	//output to file
	fs::ofstream optimisedTransitionFile("./data/trainingData/localOptimisedData/"+repository_path.leaf()+"_tran.txt");
	for(int i=0; i<=numOfState; i++){
		for(int j=0; j<=numOfState; j++){
			optimisedTransitionFile<<optimisedTransitionMatrix[i][j]<<endl;
		}
		if(i!=numOfState){
			optimisedTransitionFile<<"newRow"<<endl;
		}
	}
}