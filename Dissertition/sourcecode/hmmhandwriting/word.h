#include <iostream>
#include <vector>
#include "state.h"
#include "stroke.h"

class Word{
public:
	vector<stroke> strokes;
	
	Word();
	~Word();
}

Word::Word(){
	strokes = new vector<stroke>;
}

Word::~Word(){
	delete [] strokes;
}