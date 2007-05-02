#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include "State.h"
#include "Stroke.h"
#include "Viterbi.h"
#include "ViterbiResult.h"
#include <vector>

namespace fs = boost::filesystem;
namespace rh = redhat;
using namespace std;

vector<rh::ViterbiResult> insert(vector<rh::ViterbiResult> resultSequence, rh::ViterbiResult item, int i);

int main(){
	fs::path configFilePath("./data/recognitionData/path.txt");
	fs::ifstream configFile(configFilePath);
	string line;
	string line1;
	string line2;
//	while(!configFile.eof()){
//		getline(configFile, line);
//	}
	getline(configFile, line1);
	getline(configFile, line2);
	line=line1+"/"+line2+".txt";
	
	string recognitionData_path="./data/recognitionData/localFeatureData/"+line;
	fs::path optimisedData_path("./data/trainingData/localOptimisedData/");
	vector<rh::ViterbiResult> recognitionResult;
	
	if(!fs::exists(optimisedData_path)){
		cout<<"Cannot read the direcotry"<<endl;
	}
	
	fs::directory_iterator end_itr;
	
	for(fs::directory_iterator itr(optimisedData_path); itr!=end_itr; ++itr){	//each directory represent one character
		if(fs::is_directory(*itr)){
			//TEST
			string disPath = "./data/trainingData/localOptimisedData/"+itr->leaf()+"_dis.txt";
			string tranPath = "./data/trainingData/localInitialData/"+itr->leaf()+"_tran.txt";//tempararily use initial transition probability
			
			rh::ViterbiResult result = rh::Viterbi::Calculate_path_and_probability(disPath, recognitionData_path, tranPath);
			result.character=itr->leaf();
			
//			cout<<result.probability<<endl;
//			cout<<recognitionResult.size()<<endl;
			if(recognitionResult.size()==0){
				recognitionResult.push_back(result);
//				cout<<recognitionResult.size()<<endl;
			}else{
				int i=0;
				bool keepGoing=true;
				while(i<recognitionResult.size()&&keepGoing){
					if(recognitionResult.at(i).probability<result.probability){
						recognitionResult = insert(recognitionResult, result, i);
						keepGoing=false;
					}else if(i==(recognitionResult.size()-1)){
						recognitionResult.push_back(result);
						keepGoing=false;
					}
					i++;
				}
			}
		}
	}
//	for(int i=0; i<recognitionResult.size(); i++){
//		cout<<recognitionResult.at(i).probability<<endl;
//	}
	
	//output results
	fs::path resultFilePath("./data/recognitionData/results/"+line2+".txt");
	fs::ofstream resultFile(resultFilePath);
	
	resultFile<<"For character: "<<line<<endl;
	for(int i=0; i<recognitionResult.size(); i++){
		resultFile<<i+1<<"\t"<<recognitionResult.at(i).character<<endl;
	}
	
	resultFile.close();
	
	return 0;
}

vector<rh::ViterbiResult> insert(vector<rh::ViterbiResult> resultSequence, rh::ViterbiResult item, int i){
	vector<rh::ViterbiResult>::iterator indexIterator;
	indexIterator=resultSequence.begin()+i;
	resultSequence.insert(indexIterator, item);
	return resultSequence;
} 