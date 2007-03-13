#include <iostream>
#include <string>
//#include <erron.h>
//#include <dirent.h>
//#include <stdio.h>
#include <stlib.h>

using namespace std;

int main(){
	DIR *repository;
	struct dirent *pent;
	
	repository = opendir("./data/trainingData");
	if(!repository){
		cout<<"Opendir() failure; terminating");
		exit(1);
	}
	erron=0;
	while((pent=readdir(repository))){
		cout<<pent->d_name;
	}
	if(errno){
		cout<<"readdir() failure; terminating");
		exit(1);
	}
	closedir(repository);
}