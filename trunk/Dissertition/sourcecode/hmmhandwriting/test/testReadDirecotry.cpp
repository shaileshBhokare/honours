#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;
using namespace std;

int main(){
	fs::path repository("./data/trainingData");
	fs::path file_path;
	
	if(!fs::exists(repository)) return 1;
	
	string line;
	fs::directory_iterator end_itr;
	for(fs::directory_iterator itr(repository); itr != end_itr; ++itr){
		if(is_directory(*itr)){
			cout<<"Found one diretory"<<endl;
		}else {
			file_path = *itr;
			fs::ifstream file(file_path);
			getline(file, line);

//			cout<<"Found one file"<<endl;
			cout<<line<<endl;
		}
	}
}