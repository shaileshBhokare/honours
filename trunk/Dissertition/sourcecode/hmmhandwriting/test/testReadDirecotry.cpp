#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;
using namespace std;

void traverseFile(fs::path dir_path);

int main(){
	string dirPath="../data/tempTrainingData";
	fs::path repository(dirPath);
	traverseFile(repository);
	
	return 0;
}

void traverseFile(fs::path dir_path){
	if(!fs::exists(dir_path)) cout<<"file doesn't exist";
	
//	string line;
	fs::directory_iterator end_itr;
	for(fs::directory_iterator itr(dir_path); itr != end_itr; ++itr){
		if(is_directory(*itr)){
			cout<<itr->leaf()<<endl;
			fs::path tempPath = *itr;
			cout<<tempPath.leaf()<<endl;
			cout<<tempPath.string()<<endl;
			traverseFile(*itr);
		}else {
//			fs::ifstream file(file_path);
//			getline(file, line);

			cout<<itr->leaf()<<endl;
//			cout<<line<<endl;
		}
	}
}