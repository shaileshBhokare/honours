#include <iostream>
#include <string>
#include "../Viterbi.h"
#include <vector>
#include "../ViterbiResult.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

namespace rh = redhat;
namespace fs = boost::filesystem;
using namespace std;

int main(){
	string disPath = "../data/featureData/four/four_dis.txt";
	string obePath = "../data/featureData/four/feature/4.1.txt";
	rh::ViterbiResult result;
	string tranPath="../data/featureData/four/four_tran.txt";
	
	string outViterbiPath = "../data/temp/viterbiPath.txt";
	fs::ofstream outViterbiFile(outViterbiPath);
	if(!outViterbiFile){
		cout << "Cannot write to file.\n";
	}
	
	result = rh::Viterbi::Calculate_path_and_probability(disPath,obePath,tranPath);
	for(int i=0; i<result.path.size(); i++){
		outViterbiFile<<result.path.at(i)<<endl;
	}
	outViterbiFile.close();
}