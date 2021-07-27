//------------------------------------------------------------------------------
//
// PROJECT : 3D Globus
//
// FILE    : CStringTool.cpp
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
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "CStringTool.h"

int CStringTool::SplitString(string InputString, vector<string>* ResultVector, char SplitChar)
{
    int OldIndex = 0;
    int NewIndex;

    do
    {
        NewIndex = InputString.find(SplitChar, OldIndex);

        if (NewIndex != string::npos)
        {
            ResultVector->push_back(InputString.substr(OldIndex, NewIndex - OldIndex));

            //cout << "String=" <<  *(--rc.end()) << endl;
            OldIndex = NewIndex + 1;
        }
        else
        {
            ResultVector->push_back(InputString.substr(OldIndex, InputString.size() - OldIndex));
            //cout << "LastString=" <<  *(--rc.end()) << endl;
        }
    } while (NewIndex != string::npos);

    return ResultVector->size();
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   TrimString
//
//
//
//
//---------------------------------------------------------------------------

std::string CStringTool::TrimString(std::string InStr, const char* trimChars)
{
    string OutStr;
    string::size_type StartIndex;
    string::size_type EndIndex;

    StartIndex = InStr.find_first_not_of(trimChars);



    if (StartIndex != string::npos)
    {
        EndIndex = InStr.find_last_not_of(trimChars);
        if (EndIndex != string::npos)
        {
            if (EndIndex > StartIndex)
            {
                OutStr = InStr.substr(StartIndex, EndIndex - StartIndex + 1);
            }
        }
    }

    return OutStr;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binaer-Modus geöffnet sein
//
//---------------------------------------------------------------------------

int CStringTool::ReadLine(ifstream& ifile, string* Line)
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
    } while (!EndLine);
    return true;
}


#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CStringTool
// Methode:   FilterUmlaut
//
// Ersetzt
//
//---------------------------------------------------------------------------

string CStringTool::FilterUmlaut(const string& InStr)
{
    string OutStr;

    for (int i = 0; i < InStr.size(); i++)
    {
        char c = InStr[i];

        switch (c)
        {
            case 'Ä':
                OutStr += "Ae";
                break;

            case 'Ö':
                OutStr += "Oe";
                break;

            case 'Ü':
                OutStr += "Ue";
                break;

            case 'ä':
                OutStr += "ae";
                break;

            case 'ö':
                OutStr += "Oe";
                break;

            case 'ü':
                OutStr += "Ue";
                break;

            default:
                OutStr += c;
                break;
        }
    }
    return OutStr;
}


//---------------------------------------------------------------------------
//
// Klasse:    CStringTool
// Methode:   RemoveUmlautToUpper
//
//
//
//---------------------------------------------------------------------------


void CStringTool::RemoveUmlaut(char* c1, bool ToUpper)
{
    if (ToUpper)
    {
        switch (*c1)
        {
            case 'Ä':
                *c1 = 'A';
                break;

            case 'Ö':
                *c1 = 'O';
                break;

            case 'Ü':
                *c1 = 'U';
                break;

            case 'ä':
                *c1 = 'A';
                break;

            case 'ö':
                *c1 = 'O';
                break;

            case 'ü':
                *c1 = 'U';
                break;

            default:
                *c1 = toupper(*c1);
                break;
        }
    }
    else
    {
        switch (*c1)
        {
            case 'Ä':
                *c1 = 'A';
                break;

            case 'Ö':
                *c1 = 'O';
                break;

            case 'Ü':
                *c1 = 'U';
                break;

            case 'ä':
                *c1 = 'a';
                break;

            case 'ö':
                *c1 = 'o';
                break;

            case 'ü':
                *c1 = 'u';
                break;
        }
    }
}


#endif

//---------------------------------------------------------------------------
//
// Klasse:    CStringTool
// Methode:   CompareStrings
//
// IgnoreCase = true  : Groß/Kleinschreibung ignorieren
// IgnoreCase = false : Groß/Kleinschreibung berücksichtigen
//
// Rückgabe: Es wird "true" zurückgegeben, wenn str1 kleiner als str2 ist.
//           Sonst "false".
//
//
//---------------------------------------------------------------------------


bool CStringTool::CompareStrings(const string& str1, const string& str2, bool IgnoreCase)
{
    bool EndString = false;
    bool r = false;
    int i = 0;

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
    } while (!EndString);

    return r;
}
