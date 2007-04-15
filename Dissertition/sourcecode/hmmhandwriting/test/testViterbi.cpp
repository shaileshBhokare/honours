#include <iostream>
#include <string>
#include "../Viterbi.h"
#include <vector>

namespace rh = redhat;
using namespace std;

int main(){
	string disPath = "../data/probability/four.txt";
	string obePath = "../data/featureData/four/4.1.txt";
	vector <int> result;
	string tranPath="../data/transitionProbability/four.txt";
	result = rh::Viterbi::Calculate_path_and_probability(disPath,obePath,tranPath);
//	result.push_back(1.0);
//	result.push_back(2);
	for(int i=0; i<result.size(); i++){
		cout<<result.at(i)<<endl;
	}
}