#include <iostream>
#include <string>
#include "boost\filesystem\operations.hpp"
#include "boost\filesystem\fstream.hpp"
#include "boost\filesystem\path.hpp"

using namespace std;

int main(){
	path repository("./data/trainingData");
	
	if(!exists(repository)) return 1;
	
	string line;
	directory_iterator end_itr;
	for(directory_iterator itr(dir_path); itr != end_itr; ++itr){
		if(is_directory(*itr)){
			cout<<"Found one diretory"<<endl;
		}else {
			file_path = *itr;
			ifstream file(file_path);
			getline(file, line);
			
			cout<<line<<endl;
		}
	}
}