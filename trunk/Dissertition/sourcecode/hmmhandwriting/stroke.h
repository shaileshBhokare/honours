#include <iostream>
#include "state.h"
using namespace std;

class Stroke{
public:
	State state[3];
	
	Stroke();
	~Stroke();
}

Stroke::Stroke(){
	for(int i=0; i<16; i++){
		state[i] = NULL;
	}
}

Stroke::~Stroke(){
	delete [] state;	
}