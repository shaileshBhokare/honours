#include <iostream>
#include <math.h>
#include "Stroke.h"

namespace rh = redhat;
using namespace std;

namespace redhat{
	class Viterbi{
		public:
			static Calculate_path_and_probability(rh::Stroke distributionProbability, vector<int> observation, double transitionProbability[3][3]);
			double max(double a, double b);
			double max(double a, double b, double c);
	};
	
	static Viterbi::Calculate_path_and_probability(rh::Stroke distributionProbability, vector<int> observation, double transitionProbability[3][3]){
		int columns = observation.size();
		int rows = 3;
		Node matrix[3][columns];
		//initialization
		matrix[0][0].probability = log(distributionProbability.state[0].vector[observation.at[0]]);
		matrix[0][0].path = 0;
		
		matrix[1][0].probability = 0;
		matrix[1][0].path = 0;
		
		matrix[2][0].probability = 0;
		matrix[2][0].path = 0;
		//recursion
		for(int i=1; i<columns; i++){
			for(int j=0; j<rows; j++){
				double tempProb1 = log(matrix[i-1][j].probability)+log(transitionProbability[0][0])+
				double tempProb2 = log(matrix[i-1][j+1].probability)+log(transitionProbability[1][0])+
				double tempProb3 = log(matrix[i-1][j+2].probability)+log(transitionProbability[2][0])+
			}
		}
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
	
	class Node{
		public:
			double probability;
			int path;
	};
}