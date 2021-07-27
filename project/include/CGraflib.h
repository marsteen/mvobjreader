#ifndef CGRAFLIB
#define CGRAFLIB

#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CDataRect.h>

enum EGraflibExceptionNr
{
    EGLIB_VOID,
    EGLIB_OPENREAD,
    EGLIB_OPENWRITE
};


class CGraflibException
{
    public:

        CGraflibException(EGraflibExceptionNr er, std::string fn) : mErrnum(er), mFilename(fn)
        {
        }


        EGraflibExceptionNr mErrnum;
        std::string mFilename;
};

class CGraflib : public CDataRect
{
    public:

        virtual bool Read(const char* Filename, SFileReadInfo* fri = NULL) = 0;
        virtual int Write(const char* Filename, bool bCompFlag, CFileIO* fio) = 0;

        virtual bool OpenReadLine(const char* Filename) = 0;
        virtual void ReadLine(void* LineBuffer) = 0;
        virtual void CloseReadLine(void) = 0;

        virtual bool OpenWriteLine(const char* Filename) = 0;
        virtual void WriteLine(void* LineBuffer) = 0;
        virtual void CloseWriteLine(void) = 0;

        void Yflip(void);
        void SwapRedBlue(void);
        bool InterleaveAlpha(CGraflib* Alpha);
        void Scale(int NewWidth, int NewHeight);

        void DownScaleLine2(void* LineBuffer, void* ErgBuffer, int LineOffset);
        void DownScaleLine2(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int b);

        void GreyScale24bit(void);

    protected:
};
#endif
