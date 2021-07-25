#ifndef CSTRINGTOOL_HPP
#define CSTRINGTOOL_HPP

#include <string>
#include <sstream>

#include <CStringTool.h>

template <typename T>
T CStringTool::StringTo(const std::string& str)
{
	T Value;
	std::stringstream istr(str);
	istr >> Value;
	return Value;
}

template <typename T>
T CStringTool::StringTo(const std::string& str, int Precision)
{
	T Value;
	std::stringstream istr(str);
	istr.precision(Precision);
	istr >> Value;
	return Value;
}


#endif
