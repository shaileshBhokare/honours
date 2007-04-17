#include <iostream>
#include <math.h>
#include "Stroke.h"
#include <string>
#include <vector>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include "convert.h"
#include "Node.h"
#include "Result.h"

namespace rh = redhat;
namespace fs = boost::filesystem;
using namespace std;

namespace redhat{
	class Viterbi{
		public:
			static rh::Result Calculate_path_and_probability(string distributionProbabilityFilePath, string observationFilePath, string transitionProbabilityFilePath);
			static void insertIntoVector(int num, vector<int> pathVector);
	};
	
	rh::Result Viterbi::Calculate_path_and_probability(string distributionProbabilityFilePath, string observationFilePath, string transitionProbabilityFilePath){
		
		//create two 2D array to store the distribtion and transition probability
		double disProb[100][16];
		double tranProb[100][100];
		
		int disRow=0;//used to locate the value in the 2D array
		int disColumn=0;
		int tranRow=0;//tranRow should equal tranColumn, since the transition probability matrix should be a square matrix.
		int tranColumn=0;
		
		vector<int> observation;
		
		string line;//used to retrieve each line in a file
		double num;// used to get double format value of line;
		
		//populate the two 2D arrays
		fs::ifstream disProbFile(distributionProbabilityFilePath);
		if(!disProbFile){
			cout<<"Cannot open file.\n";
		}else{
			while(!disProbFile.eof()){
				getline(disProbFile, line);
				num=rh::convertToDouble(line);
				disProb[disRow][disColumn]=num;
				disColumn++;
				if(disColumn==16) disRow++;
				disColumn = disColumn%16;
			}
		}
		
		//for testing
		//testing start
		/*cout<<"Distribution File:"<<endl<<endl;
		for(int i=0; i<disRow; i++){
			for(int j=0; j<16; j++){
				cout<<disProb[i][j]<<"\t";
			}
			cout<<endl;
		}//testing finish*/
		
		fs::ifstream observationFile(observationFilePath);
		if(!observationFile){
			cout<<"Cannot open file.\n";
		}else{
			while(!observationFile.eof()){
				getline(observationFile, line);
				num=rh::convertToDouble(line);
				observation.push_back(num);
			}
		}
		
		/*cout<<"Observation File:"<<endl<<endl;
		//for testing
		//testing start
		for(int i=0; i<observation.size(); i++){
			cout<<observation.at(i)<<endl;
		}//testing finish*/
	
		
		fs::ifstream tranProbFile(transitionProbabilityFilePath);
		if(!tranProbFile){
			cout<<"Cannot open file.\n";
		}else{
			while(!tranProbFile.eof()){
				getline(tranProbFile, line);
				if(line.compare("newRow")==0){
					tranColumn=0;
					tranRow++;
				}else{
					num=rh::convertToDouble(line);
					tranProb[tranRow][tranColumn]=num;
					tranColumn++;
				}
			}
		}

		/*cout<<"Transition File:"<<endl<<endl;
		//testing start
		for(int i=0; i<=tranRow; i++){
			for(int j=0; j<tranColumn; j++){
				cout<<tranProb[i][j]<<"\t";
			}
			cout<<endl;
		}//testing finish*/
		
		
		
		int matrixColumn = observation.size();
//		int rows = 3;
		rh::Node matrix[100][500];
		
		double maxProbability = 0;
		rh::Node maxNode;
		
		vector <int> mostPossiblePath;
		
		//initialization
//		matrix[0][0].probability = log(distributionProbability.state[0].vector[observation.at[0]]);
		matrix[0][0].probability = log(disProb[0][observation.at(0)]);
		matrix[0][0].path = 0;
		
//		matrix[1][0].probability = 0;
//		matrix[1][0].path = 0;
//		
//		matrix[2][0].probability = 0;
//		matrix[2][0].path = 0;
		
		for(int i=1; i<tranColumn; i++){
			matrix[i][0].probability = 0;
			matrix[i][0].path = 0;
		}
		
		//recursion
		/*for(int i=1; i<columns-1; i++){
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
		}*/
		
		for(int i=1; i<matrixColumn; i++){//calculate column by column
			for(int j=0; j<tranColumn; j++){//calculate each node
				double maxProbAtPresent = 0;
				double maxPathProbAtPresent = 0;
				int maxPath = 0;//default is from the state one.
				for(int k=0; k<tranColumn; k++){//calculate every previous node
					double tempProb = log(matrix[k][i-1].probability)+log(tranProb[k][j])+log(disProb[j][observation.at(i)]);
					double tempPathProb = log(matrix[k][i-1].probability)+log(tranProb[k][j]);
					if (maxProbAtPresent = 0){
						maxProbAtPresent=tempProb;
					}
					if (maxPathProbAtPresent = 0){
						maxPathProbAtPresent = tempPathProb;
					}
					if (tempProb>=maxProbAtPresent){
						maxProbAtPresent=tempProb;
					}
					if (tempPathProb >= maxPathProbAtPresent){
						maxPathProbAtPresent = tempPathProb;
						maxPath = k;	
					}
				}
				matrix[j][i].probability = maxProbAtPresent;
				matrix[j][i].path = maxPath;
			}
		}
		

		cout<<"Viterbi File:"<<endl<<endl;
		//for testing
		//testing start
		for(int i=0; i<tranColumn; i++){
			for(int j=0; j<matrixColumn; j++){
				cout<<matrix[i][j].probability<<"\t";
			}
			cout<<endl;
		}//testing finish*/
		
				//test
		double testProb = log(matrix[0][0].probability)+log(tranProb[0][0])+log(disProb[0][observation.at(2)]);
		cout<<testProb<<endl;
		cout<<log(matrix[0][0].probability)<<"\t"<<log(tranProb[0][0])<<"\t"<<log(disProb[0][observation.at(2)])<<endl;
		//end test
		
		
		//terminiation
		//get the max probability
//		maxProbability = max(matrix[0][columns-1].probability, matrix[1][columns-1].probability, matrix[2][columns-1].probability);
		
		
		//get the ending node at the max probability
//		if(maxProbability=matrix[0][columns-1].probability) maxNode = matrix[0][columns-1];
//		if(maxProbability=matrix[1][columns-1].probability) maxNode = matrix[1][columns-1];
//		else maxNode = matrix[2][columns-1];

		//get the max probability and the max node
		for(int i=0; i<tranColumn; i++){
			if(matrix[i][matrixColumn-1].probability>maxProbability){
				maxProbability = matrix[i][matrixColumn-1].probability;
				maxNode = matrix[i][matrixColumn-1];
			}
		}
		
		//state path backtracking
//		mostPossiblePath.push_back(maxNode.path);
		int previousPath = maxNode.path;
		for(int i = matrixColumn-1; i >= 0; i--){
			Viterbi::insertIntoVector(previousPath, mostPossiblePath);
			previousPath = matrix[previousPath][i].path;
		}
		
		rh::Result result;
		result.probability = maxProbability;
		result.path = mostPossiblePath
		return result;
	}
	
	
	void Viterbi::insertIntoVector(int num, vector<int> pathVector){
		vector<int>::iterator theIterator = pathVector.begin();
		pathVector.insert(theIterator, num);
	}

}