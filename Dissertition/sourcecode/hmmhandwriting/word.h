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
	
}

Word::~Word(){
	delete [] strokes;
}