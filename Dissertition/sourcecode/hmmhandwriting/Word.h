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
		void increaseStroke();
		void replace(rh::Stroke ob, int i);
		void display();
	};
	
	Word::Word(){
//		strokes = new vector<stroke>;
	}
	
	Word::~Word(){
//		vector<rh::Stroke>().swap(strokes);
	}
	
	void Word::setStroke(rh::Stroke ob){
		strokes.push_back(ob);
	}
	
	rh::Stroke Word::getStroke(int i){
		return strokes.at(i);
	}
	
	void Word::increaseStroke(){
		rh::Stroke newStroke;
		Word::setStroke(newStroke);
	}
	
	void Word::replace(rh::Stroke ob, int i){
		vector<rh::Stroke>::iterator indexIterator;
		indexIterator=strokes.begin()+i;
		strokes.erase(indexIterator);
		strokes.insert(indexIterator, ob);
	}
	
	void Word::display(){
		for(int i=0; i<strokes.size(); i++){
			rh::Stroke tempStroke = strokes.at(i);
			tempStroke.display();
		}
	}
}

#endif