#ifndef __STATE__
#define __STATE__

#include <iostream>

using namespace std;

namespace redhat{
	class State{
	public:
		double vector[16];//used to store features
		
		State();
		~State();

		void display();
	};
	
	State::State(){
		for(int i=0; i<16; i++){
			vector[i] = 0;
		}
	}
	
	State::~State(){
//		delete [] vector;
//		cout<<"deleting vector"<<endl;
	}
	
	void State::display(){
		for(int i=0; i<16; i++){
			cout<<vector[i]<<endl;
		}
	}
}

#endif //__STATE__