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
	string recognitionData_path("./data/recognitionData/localFeatureData/1.1/1.1.9.txt");
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
			
			if(recognitionResult.size()==0){
				recognitionResult.push_back(result);
			}else{
				for(int i=0; i<recognitionResult.size(); i++){
					if(recognitionResult.at(i).probability<result.probability){
						recognitionResult = insert(recognitionResult, result, i);
					}
				}
			}
		}
	}
	
	for(int i=0; i<recognitionResult.size(); i++){
		cout<<recognitionResult.at(i).character<<endl;
	}
	return 0;
}

vector<rh::ViterbiResult> insert(vector<rh::ViterbiResult> resultSequence, rh::ViterbiResult item, int i){
	vector<rh::ViterbiResult>::iterator indexIterator;
	indexIterator=resultSequence.begin()+i;
	resultSequence.insert(indexIterator, item);
	return resultSequence;
} 