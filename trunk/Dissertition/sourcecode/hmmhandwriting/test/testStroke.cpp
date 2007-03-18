#include <iostream>
#include "../State.h"
#include "../Stroke.h"

namespace rh = redhat;
using namespace std;

int main(){
	rh::Stroke tempStroke;
	rh::State tempState[3];
	
	for(int i=0; i<16; i++){
		tempState[0].vector[i]=i;
	}
	
	for(int i=0; i<16; i++){
		tempState[1].vector[i]=i+100;
	}
	
	for(int i=0; i<16; i++){
		tempState[2].vector[i]=i+200;
	}
	
	tempStroke.setState(0,tempState[0]);
	tempStroke.setState(1,tempState[1]);
	tempStroke.setState(2,tempState[2]);
	
	tempStroke.display();
	
	cout<<"Test get"<<endl;
	
	rh::State newState = tempStroke.getState(1);
	newState.display();
	
	return 0;
}