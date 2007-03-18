#include <iostream>
#include <string>
#include "../State.h"
#include "../Stroke.h"
#include "../Word.h"

using namespace std;

int main(){
	Word word = new Word();
	Stroke stroke = new Stroke();
	State state[] = new State[3];
	
	for(int i=0; i<16; i++){
		state[0].vector[i] = i;
	}
	
	for(int i=100; i<116; i++){
		state[1].vector[i] = i;
	}
	
	for(int i=200; i<216; i++){
		state[2].vector[i] = i;
	}
	
	for(int i=0; i<3; i++){
		stroke.state[i] = state[i];	
	}
	
	word.strokes.push_back(stroke);
	
	//print out the word content;
	for(int i=0; i<word.strokes.size(); i++){
		Stroke tempStroke = word.strokes.at(i);
		for(int j=0; i<3; i++){
			State tempState = tempStroke.state[i];
			for(k=0; k<3; k++){
				cout<tempState.vector[k]<<endl;
			}
		}
	}
	
	return 0;
}