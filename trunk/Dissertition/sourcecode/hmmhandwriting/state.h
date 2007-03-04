#include <iostream>
using namespace std;

class State{
public:
	double vector[16];
	
	State();
};

State::State(){
	for(int i=0; i<16; i++){
		vector[i] = 0;
	}
}