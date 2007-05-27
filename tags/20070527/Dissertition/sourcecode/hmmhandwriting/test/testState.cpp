#include <iostream>
#include "../State.h"

namespace rh = redhat;
using namespace std;

int main(){
	rh::State tempState[2];
	
	for(int i=0; i<16; i++){
		tempState[0].vector[i] = i;
	}
	
	for(int i=0; i<16; i++){
		tempState[1].vector[i] = i+100;
	}
	
	tempState[0].display();
	tempState[1].display();
	
	return 0;
}