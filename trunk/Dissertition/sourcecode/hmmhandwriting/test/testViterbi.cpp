#include <iostream>
#include <string>
#include "../Viterbi.h"
#include <vector>

namespace rh = redhat;
using namespace std;

int main(){
	string disPath = "../data/trainingData/example.txt";
	vector <int> obervation;
	string tranPath="../data/trainingData/exampleInfo.txt";
	rh::Viterbi::Calculate_path_and_probability(disPath,obervation,tranPath);
}