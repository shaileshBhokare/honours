#ifndef __ViterbiResult__
#define __ViterbiResult__

#include <iostream>
#include <vector>

using namespace std;

namespace redhat{
	class ViterbiResult{
		public:
			double probability;
			vector<int> path;
	};
}

#endif