#ifndef __WORD__
#define __WORD__

#include <iostream>
#include <vector>
#include "State.h"
#include "Stroke.h"

namespace rh = redhat;
using namespace std;

namespace redhat{
	class Word{
	public:
		vector<rh::Stroke> strokes;
		
		Word();
		~Word();
		void setStroke(rh::Stroke ob);
		rh::Stroke getStroke(int i);
		void display();
	};
	
	Word::Word(){
//		strokes = new vector<stroke>;
	}
	
	Word::~Word(){
		delete [] strokes;
	}
	
	void setStroke(rh::Stroke ob){
		strokes.push_back(ob);
	}
	
	rh::Stroke Word::getStroke(int i){
		return strokes.at(i);
	}
	
	void Word::display(){
		for(int i; i<strokes.size; i++){
			
		}
	}
}

#endif