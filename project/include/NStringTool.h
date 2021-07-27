#ifndef NSTRINGTOOL_H
#define NSTRINGTOOL_H

#include <sstream>
#include <string>
#include <vector>

#ifndef stringvector
typedef std::vector<std::string> stringvector;
#endif

namespace NStringTool
{
    int Split(std::string InputString, stringvector* ResultVector, char SplitChar);
    int Split(std::string InputString, stringvector* ResultVector, char SplitChar, const char b0, const char b1);

    int ReadLine(std::ifstream& ifile, std::string* Line);

    bool Compare(const std::string& str1, const std::string& str2, bool IgnoreCase = false);
    std::string Trim(std::string str, const char* trimChars);
    std::string FilterUmlaut(const std::string& InStr);

    std::string ReplaceExtension(const char* Filename, const char* NewExtension);
    std::string Pathname(const std::string& Filename, char PathDiff);
    std::string Cut(const std::string& name, char diff);



    template<typename T>
    T Cast(const std::string& str)
    {
        T Value;
        std::stringstream istr(str);

        istr >> Value;
        return Value;
    }


    template<typename T>
    T Cast(const std::string& str, int Precision)
    {
        T Value;
        std::stringstream istr(str);

        istr.precision(Precision);
        istr >> Value;
        return Value;
    }
}


#endif
