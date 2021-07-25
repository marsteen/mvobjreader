//------------------------------------------------------------------------------
//
// PROJECT : 3D Globus
//
// FILE    : NStringTool.cpp
//
// VERSION : 1.0
//
// AUTOR   : Martin Steen
//           Imagon GmbH
//
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Implementation von CStringTool
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 02.11.2007 erstellt - Martin Steen
// 20.07.2010 erweitert - Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>


#include <NStringTool.h>

namespace NStringTool
{
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Split
	//
	//---------------------------------------------------------------------------

	int Split(std::string InputString, stringvector* ResultVector, char SplitChar)
	{
		int OldIndex = 0;
		int NewIndex;

		if (strchr(InputString.c_str(), SplitChar) != NULL)
		{
			do
			{
				NewIndex = InputString.find(SplitChar, OldIndex);

				if (NewIndex != std::string::npos)
				{
					ResultVector->push_back(InputString.substr(OldIndex, NewIndex - OldIndex));
					OldIndex = NewIndex + 1;
				}
				else
				{
					ResultVector->push_back(InputString.substr(OldIndex, InputString.size() - OldIndex));
				}
			}
			while (NewIndex != std::string::npos);
		}

		return ResultVector->size();
	}

	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Split
	//
	//---------------------------------------------------------------------------

	int Split(std::string InputString, stringvector* ResultVector, char SplitChar, const char b0, const char b1)
	{
		int BrackedOpen = 0;
		std::string InputStringTrimmed;

		for (int i = 0; i < InputString.size(); i++)
		{
			char c = InputString.at(i);
			if (c == b0)
			{
				BrackedOpen += 1;
			}
			else
			if (c == b1)
			{
				BrackedOpen -= 1;
			}
			else
			{
				if ((BrackedOpen > 0) && (c == SplitChar))
				{
					InputStringTrimmed += '*';
				}
				else
				{
					InputStringTrimmed += c;
				}
			}
		}

		//std::cout << "InputStringTrimmed=" << InputStringTrimmed << std::endl;
		int r = Split(InputStringTrimmed, ResultVector, SplitChar);

		for (stringvector::iterator it = ResultVector->begin(); it != ResultVector->end(); ++it)
		{
			for (int i = 0; i < it->size(); i++)
			{
				if ((*it)[i] == '*')
				{
					(*it)[i] = SplitChar;
				}
			}

		}
		return r;
	}



	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Trim
	//
	//---------------------------------------------------------------------------

	std::string Trim(std::string InStr, const char* trimChars)
	{
		std::string OutStr;
		std::string::size_type StartIndex;
		std::string::size_type EndIndex;

		StartIndex = InStr.find_first_not_of(trimChars);



		if (StartIndex != std::string::npos)
		{
			EndIndex = InStr.find_last_not_of(trimChars);
			if (EndIndex != std::string::npos)
			{
				if (EndIndex >= StartIndex)
				{
					OutStr = InStr.substr(StartIndex, EndIndex - StartIndex + 1);
				}
			}
		}

		return OutStr;
	}

	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    ReadLine
	//
	//---------------------------------------------------------------------------

	int ReadLine(std::ifstream& ifile, std::string* Line)
	{
		char ch;
		bool EndLine = false;

		Line->clear();
		do
		{
			ifile.get(ch);
			if (!ifile.eof())
			{
				if (ch != 0x0D)
				{
					if (ch != 0x0A)
					{
						Line->push_back(ch);
					}
					else
					{
						EndLine = true;
					}
				}
			}
			else
			{
				EndLine = true;
			}
		}
		while (!EndLine);
		return true;
	}

#if 0
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:
	//
	//---------------------------------------------------------------------------

	std::string FilterUmlaut(const std::string& InStr)
	{
		std::string OutStr;

		for (int i = 0; i < InStr.size(); i++)
		{
			char c = InStr[i];

			switch (c)
			{
				case '?': OutStr += "Ae"; break;
				case '?': OutStr += "Oe"; break;
				case '?': OutStr += "Ue"; break;
				case '?': OutStr += "ae"; break;
				case '?': OutStr += "Oe"; break;
				case '?': OutStr += "Ue"; break;

				default: OutStr += c; break;
			}
		}
		return OutStr;
	}
#endif

#if 0
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:
	//
	//---------------------------------------------------------------------------


	void RemoveUmlaut(char* c1, bool ToUpper)
	{
		if (ToUpper)
		{
			switch (*c1)
			{
				case '?': *c1 = 'A'; break;
				case '?': *c1 = 'O'; break;
				case '?': *c1 = 'U'; break;
				case '?': *c1 = 'A'; break;
				case '?': *c1 = 'O'; break;
				case '?': *c1 = 'U'; break;

				default: *c1 = toupper(*c1); break;
			}
		}
		else
		{
			switch (*c1)
			{
				case '?': *c1 = 'A'; break;
				case '?': *c1 = 'O'; break;
				case '?': *c1 = 'U'; break;
				case '?': *c1 = 'a'; break;
				case '?': *c1 = 'o'; break;
				case '?': *c1 = 'u'; break;
			}
		}
	}
#endif

	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:
	//
	// IgnoreCase = true  : Gross/Kleinschreibung ignorieren
	// IgnoreCase = false : Gross/Kleinschreibung beruecksichtigen
	//
	// Rueckgabe: Es wird "true" zurueckgegeben, wenn str1 kleiner als str2 ist.
	//            Sonst "false".
	//
	//
	//---------------------------------------------------------------------------


	bool CompareStrings(const std::string& str1, const std::string& str2, bool IgnoreCase)
	{
		bool EndString = false;
		bool r = false;
		int  i = 0;

		const char* cstr1 = str1.c_str();
		const char* cstr2 = str2.c_str();

		do
		{
			char c1 = cstr1[i];
			char c2 = cstr2[i];

			i++;

			if ((c1 == 0) && (c2 == 0))
			{
				EndString = true;
				r = true; // gleich
			}
			else
			{
				if (c1 == 0)
				{
					EndString = true;
				}
				if (c2 == 0)
				{
					EndString = true;
					r = true;
				}
			}

			if (!EndString)
			{
				//RemoveUmlaut(&c1, IgnoreCase);
				//RemoveUmlaut(&c2, IgnoreCase);

				if (c1 < c2)
				{
					r = true;
					EndString = true;
				}
				if (c1 > c2)
				{
					EndString = true;
				}
			}
		}
		while (!EndString);

		return r;
	}

	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    ReplaceExtension
	//
	//---------------------------------------------------------------------------

	std::string ReplaceExtension(const char* Filename, const char* NewExtension)
	{
		std::string FilenameStr = Filename;

		int PointIndex = FilenameStr.find_last_of('.');
		if (PointIndex != std::string::npos)
		{
			FilenameStr = FilenameStr.substr(0, PointIndex) + NewExtension;
		}
		else
		{
			FilenameStr += std::string(".") + NewExtension;
		}

		return FilenameStr;
	}

  //---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Pathname
  //
  // Rueckgabe:  Pfadname des Dateiamens
	//
	//---------------------------------------------------------------------------

	std::string Pathname(const std::string& FilenameStr, char PathDiff)
	{
		std::string PathStr;

		int SlashIndex = FilenameStr.find_last_of(PathDiff);
		if (SlashIndex != std::string::npos)
		{
			PathStr = FilenameStr.substr(0, SlashIndex);
		}
		return PathStr;
	}
	
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Cut
  //
  // Rueckgabe:  Schneidet alles hinter einem Zeichen ab.
  // Beispiel:   aus "mantis.js" wird "mantis" ('.' als Parameter) 
	//
	//---------------------------------------------------------------------------

	std::string Cut(const std::string& Name, char d)
	{
		std::string cstr;

		int pIndex = Name.find_last_of(d);
		if (pIndex != std::string::npos)
		{
		  cstr = Name.substr(0, pIndex);
		}
		else
		{
		  cstr = Name;
		}
	  
		return cstr;
	}


  //
	// End of namespace NStringTool
	//
}