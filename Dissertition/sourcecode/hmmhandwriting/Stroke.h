#ifndef __STROKE__
#define __STROKE__

#include <iostream>
#include "State.h"

namespace rh = redhat;
using namespace std;

namespace redhat{
	class Stroke{
	public:
		rh::State state[3];
		
		Stroke();
		~Stroke();
		void setState(int i, rh::State ob);
		rh::State getState(int i);
		void display();
	};
	
	Stroke::Stroke(){
//		for(int i=0; i<3; i++){
//			state[i] = new State();
//		}
	}
	
	Stroke::~Stroke(){
		delete [] state;	
	}
	
	void Stroke::setState(int i, rh::State ob){
		state[i] = ob;
	}
	
	rh::State Stroke::getState(int i){
		return state[i];
	}
	
	void Stroke::display(){
		for(int i=0; i<3; i++){
			state[i].display();	
		}	
	}
}

#endif