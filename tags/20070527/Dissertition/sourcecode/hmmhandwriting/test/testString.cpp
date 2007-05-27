#include <iostream>
#include <string>
using namespace std;

int main(){
	string source = "100,200";
	string dest = "...........";
	
	int position = source.find(",");
	
	if(position != string::npos){
		string firstPart = source.substr(0,position-1);
		string secondPart = source.substr(position+1);
		cout<<firstPart<<endl;
		cout<<secondPart<<endl;
	}else{
		cout<< "wrong format" << endl;
	}
}
	