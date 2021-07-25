#ifndef CSTRINGTOOL
#define CSTRINGTOOL

#include <vector>
#include <string>
#include <fstream>

typedef std::vector<std::string> stringvector;

class CStringTool
{
	public:

		int SplitString(std::string InputString, stringvector* ResultVector, char SplitChar);
		int ReadLine(std::ifstream& ifile, std::string* Line);


		template <typename T>
		T StringTo(const std::string& str);

		template <typename T>
		T StringTo(const std::string& str, int Precision);

		bool CompareStrings(const std::string& str1, const std::string& str2, bool IgnoreCase=false);
		std::string TrimString(std::string str, const char* trimChars);
    std::string FilterUmlaut(const std::string& InStr);


	protected:

		void RemoveUmlaut(char* c1, bool ToUpper);


};

#include <CStringTool.hpp>

#endif

