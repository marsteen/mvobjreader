#ifndef CFILEIO_H
#define CFILEIO_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "EFileTypes.h"



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <CStringTool.h>

#define OPENMODE	std::ios_base::openmode
#define SEEKDIR		std::ios_base::seekdir

enum EFileAttribut
{
    EFA_NOT_FOUND,
    EFA_FILE,
    EFA_FOLDER
};

class CFileIO
{
    public:

        CFileIO()
        {
            mOpenRead = false;
            mOpenWrite = false;
            mFs = NULL;
        }


        ~CFileIO()
        {
            delete mFs;
        }


        void*  ReadFile(const char* Filename, int* nBytes);
        bool OpenFileRead(const char* Filename, OPENMODE type);
        bool OpenFileWrite(const char* Filename, OPENMODE type);
        bool OpenFileAppend(const char* Filename, OPENMODE type);
        bool OpenFileChange(const char* Filename, int Position);
        bool ReadBytes(void* buffer, int nBytes);

        void*    ReadBytes(int nBytes);
        bool WriteBytes(const void* buffer, int nBytes);
        bool WriteString(const char* str);
        bool WriteFile(const char* Filename, const void* Buffer, int nBytes);
        char*  ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count);
        int GetChar(void);
        bool IsEof(void);
        void Clear(void);
        void SkipBytes(int nBytes);
        int GuessFileFormat(const char* Filename);
        char*  ReplaceApp(const char* Filename, const char* neuApp);
        bool ReadLine(std::vector<char>* Line);
        bool ReadLine(std::string* Line);
        bool ReadSplitLine(stringvector* SplitVector, char SplitChar);
        bool AppendData(const char* File1, const void* Data, int DataLen);
        void ReadAllLines(std::vector<std::string>& LineVec, char CommentSign);

        std::string StripExtension(const char* FullPathName);


        char** ResolveWildCards(const char* WildCardString, int* nFiles);
        char** ResolveWildCard(const char* Dir, const char* WildCard, int* DirCount);
        bool ResolveWildCard(const char* Dir, const char* WildCard, std::vector<std::string>* FileList);

        void DeleteStringList(char** StrList);
        char*  Fill0(int i, int n);
        char*  Fill0(char* strbuffer, int i, int n);
        std::string Fill0str(int i, int n);
        bool CreateDir(const char* NewFolder);
        bool CreateDir(const std::string& NewFolder);
        char*  NewString(const char* OldStr);

        char*  ExtractFilename(const char* FullPathName);
        const char*  ExtractPath(const char* FullPathName);
        bool Strcmpx(const char* s1, const char* s2);
        int GetInt(const char* str);

        static int FileType(const char* Filename);
        static bool FileExists(const char* Filename);


        bool Good(void)
        {
            return (int)mFs->good();
        }


        void CloseFile(void);

        bool SetReadPos(int pos);
        bool SetReadPos2(int pos, SEEKDIR where);
        int GetReadPos(void);

        int GetWritePos(void);
        void SetWritePos(unsigned int pos);
        void SetWritePosEnd(void);


        //std::streampos GetFileSize(const char* Filename, bool* FileExists=NULL);
        int GetFileSize(const char* Filename);

        template<typename Tobj>
        bool ReadObject(Tobj& obj);

        template<typename Tobj>
        bool ReadObjects(Tobj* obj, int n);

/*
 *      template <typename Tobj>
 *      bool WriteObject(const Tobj* obj);
 */

        template<typename Tobj>
        bool WriteObjects(const Tobj* obj, int n);


        template<typename Tobj>
        bool WriteObject(const Tobj& obj);

    protected:

        std::fstream* mFs;
        bool mOpenWrite;
        bool mOpenRead;
};

#include <CFileIO.hpp>

#endif
