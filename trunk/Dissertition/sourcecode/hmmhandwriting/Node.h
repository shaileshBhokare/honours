#include <iostream>

using namespace std;

namespace redhat{
	class Node{
		public:
			double probability;
			int path;
			int currentPath;
			
			Node();
	};
	
	Node::Node(){
		probability=log(0.0);
		path=-1;
		currentPath=-1;	
	}
}