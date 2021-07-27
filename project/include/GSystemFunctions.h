#ifndef GLOBAL_SYSTEM_FUNCTIONS_H
#define GLOBAL_SYSTEM_FUNCTIONS_H

#ifdef _WIN32
#define PATHDIFF	'\\'
#else
#define PATHDIFF	'/'
#endif


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		SwapWord
//
//---------------------------------------------------------------------------

inline static void GsysSwapWord(short* w)
{
    char s;
    char* c = (char*)w;

    s = c[0];
    c[0] = c[1];
    c[1] = s;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		GsysSwapWord
//
//---------------------------------------------------------------------------

inline static void GsysSwapWord(unsigned short* w)
{
    char s;
    char* c = (char*)w;

    s = c[0];
    c[0] = c[1];
    c[1] = s;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		SwapWord
//
//---------------------------------------------------------------------------

inline static short GsysSwapWord(short w)
{
    char s;
    char* c = (char*)&w;

    s = c[0];
    c[0] = c[1];
    c[1] = s;
    return w;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		GsysSwapDword
//
//---------------------------------------------------------------------------

inline static void GsysSwapDword(int* dw)
{
    int s = *dw;
    char* c1 = (char*)dw;
    char* c2 = (char*)&s;

    c1[0] = c2[3];
    c1[1] = c2[2];
    c1[2] = c2[1];
    c1[3] = c2[0];
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		GsysSwapFloat
//
//---------------------------------------------------------------------------

inline static void GsysSwapFloat(float* dw)
{
    float s = *dw;
    char* c1 = (char*)dw;
    char* c2 = (char*)&s;

    c1[0] = c2[3];
    c1[1] = c2[2];
    c1[2] = c2[1];
    c1[3] = c2[0];
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		BigEndian
//
// Rückgabe:  true - Prozessor ist Big-Endian (MIPS, MC68000, PowerPC)
//            false - Prozessor ist Little-Endian (Intel x86)

//
//---------------------------------------------------------------------------

inline static bool GsysBigEndian()
{
    short test = 0x1234;

    return *((char*)&test) == 0x12;
}


#endif
