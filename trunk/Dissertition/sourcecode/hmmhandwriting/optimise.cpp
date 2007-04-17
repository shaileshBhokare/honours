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
			string disPath = "./data/trainingData/localInitialData/"+itr.leaf()+"_dis.txt";
			string tranPath = "./data/trainingData/localInitialData/"+itr.leaf()+"_tran.txt";
			parseFile(*itr, disPath, tranPath);
		}
	}
	
	return 0;
}

void parseFile(fs::path repository_path, string disProbFilePath, string tranProbFilePath){
	//trversal the subdirecotry
	fs::directory_iterator end_sub_itr;
	for(fs::directory_iterator sub_itr(repository_path); sub_itr!=end_sub_itr; ++sub_itr){
		if(is_directory(*sub_itr)){
			//do nothing	
		}else{
			string ovservationPath = repository_path+"/"+sub_itr->leaf();
			rh::ViterbiResult result = rh::Viterbi::Calculate_path_and_probability(disProbFilePath, tranProbFilePath, ovservationPath);
			string stateSequencePath = "./data/trainingData/localOptimisedData/"+sub_itr->leaf();
			fs::ofstream stateSequence(stateSequencePath);
			for(int i=0; i<result.path.size(); i++){
				stateSequence<<result.path.at(i)<<endl;
			}
			stateSequence.close();
		}
	}
}