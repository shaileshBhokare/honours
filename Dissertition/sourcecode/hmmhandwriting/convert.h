#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

namespace redhat{

	class Conversion: public std::runtime_error{
		public:
			Conversion(const std::string& s): std::runtime_error(s){}
	};
	
	inline double convertToDouble(const std::string& s, bool failIfLeftoverChars=true){
		std::istringstream i(s);
		double x;
		char c;
		if(!(i >> x) || (failIfLeftoverChars && i.get(c)))
			 throw Conversion("convertToDouble(\""+ s + "\")");
		return x;
	}
}