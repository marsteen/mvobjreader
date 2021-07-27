#ifndef CGRAFLIBJPEG
#define CGRAFLIBJPEG

#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CDataRect.h>
#include <CGraflib.h>

class CGraflibJpeg : public CGraflib
{
    public:

        CGraflibJpeg()
        {
            mJpegInterface = NULL;
            mJpegQuality = 80;
        }


        bool Read(const char* Filename, SFileReadInfo* fri = NULL);
        int Write(const char* Filename, bool bCompFlag, CFileIO* fio = NULL);

        bool OpenReadLine(const char* Filename);
        void ReadLine(void* LineBuffer);
        void CloseReadLine(void);

        bool OpenWriteLine(const char* Filename);
        void WriteLine(void* LineBuffer);
        void CloseWriteLine(void);

        void SetAppendMode(bool b);

        int mJpegQuality;

    protected:

        void* mJpegInterface;
};

#endif
