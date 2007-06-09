#include <iostream>
#include <math.h>
#include "Stroke.h"

namespace rh = redhat;
using namespace std;

namespace redhat{
	class Viterbi{
		public:
			Calculate_path_and_probability(rh::Word distributionProbability, vector<int> observation, double transitionProbability[3][3]);
			double max(double a, double b);
			double max(double a, double b, double c);
			void insertIntoVector(int num, vector<int> pathVector);
	};
	
	Viterbi::Calculate_path_and_probability(rh::Word distributionProbability, vector<int> observation, double transitionProbability[3][3]){
		int columns = observation.size();
		int rows = 3;
		Node matrix[3][columns];
		
		double maxProbability;
		Node maxNode;
		
		vector <int> mostPossiblePath;
		
		//initialization
		matrix[0][0].probability = log(distributionProbability.state[0].vector[observation.at[0]]);
		matrix[0][0].path = 0;
		
		matrix[1][0].probability = 0;
		matrix[1][0].path = 0;
		
		matrix[2][0].probability = 0;
		matrix[2][0].path = 0;
		//recursion
		for(int i=1; i<columns-1; i++){
			for(int j=0; j<rows; j++){
				double tempProb1 = log(matrix[i-1][j].probability)+log(transitionProbability[0][0])
										+log(distributionProbability.state[0].vector[observation.at[i]]);
				double tempProb2 = log(matrix[i-1][j+1].probability)+log(transitionProbability[1][0])
										+log(distributionProbability.state[1].vector[observation.at[i]]);
				double tempProb3 = log(matrix[i-1][j+2].probability)+log(transitionProbability[2][0])
										+log(distributionProbability.state[2].vector[observation.at[i]]);
				matrix[i][j].probability = max(tempProb1, tempProb2, tempProb3);
				
				double tempPath1 = log(matrix[i-1][j].probability)+log(transitionProbability[0][0]);
				double tempPath2 = log(matrix[i-1][j+1].probability)+log(transitionProbability[1][0]);
				double tempPath3 = log(matrix[i-1][j+2].probability)+log(transitionProbability[2][0]);
				
				if((max(tempPath1, tempPath2, tempPath3)=tempPath1) matrix[i][j].path = 0;
				else if((max(tempPath1, tempPath2, tempPath3)=tempPath2) matrix[i][j].path = 1;
				else matrix[i][j].path = 2;
			}
		}
		//terminiation
		//get the max probability
		maxProbability = max(matrix[0][columns-1].probability, matrix[1][columns-1].probability, matrix[2][columns-1].probability);
		
		//get the ending node at the max probability
		if(maxProbability=matrix[0][columns-1].probability) maxNode = matrix[0][columns-1];
		if(maxProbability=matrix[1][columns-1].probability) maxNode = matrix[1][columns-1];
		else maxNode = matrix[2][columns-1];
		
		//state path backtracking
//		mostPossiblePath.push_back(maxNode.path);
		int previousPath = maxNode.path;
		for(int i=columns-1; i>=0; i--){
			insertIntoVector(previousPath, mostPossiblePath);
			previousPath = matrix[i][previousPath].path
		}
	}
	
	double Viterbi::max(double a, double b){
		if(a>=b) return a;
		else return b
	}
	
	double Viterbi::max(double a, double b, double c){
		double temp = Viterbi::max(a,b);
		Viterbi::max(temp,c);
	}
	
	void Viterbi::insertIntoVector(int num, vector<int> pathVector){
		vector<int>::iterator theIterator = pathVector.begin();
		pathVector.insert(theIterator, num);
	}
	
	class Node{
		public:
			double probability;
			int path;
	};
}