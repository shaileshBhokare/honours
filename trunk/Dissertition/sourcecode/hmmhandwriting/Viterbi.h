#include <iostream>
#include <math.h>

using namespace std;

namespace redhat{
	class Viterbi{
		public:
			Calculate_path_and_probability();
			double max(double a, double b);
			double max(double a, double b, double c);
	};
	
	Viterbi::Calculate_path_and_probability(vector<double> observation){
		//initialization
		double prob1 = log();
		//recursion
		
		//terminiation
		
		//state path backtracking
	}
	
	double Viterbi::max(double a, double b){
		if(a>=b) return a;
		else return b
	}
	
	double Viterbi::max(double a, double b, double c){
		double temp = Viterbi::max(a,b);
		Viterbi::max(temp,c);
	}
}