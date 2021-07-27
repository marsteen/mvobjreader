//------------------------------------------------------------------------------
//
// PROJECT : Diercke Globus Online
//
// FILE    : CPixel24.cpp
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
// Inhalt: Deklaration der Klasse CPixel32
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 07.10.2009 erweitert - Martin Steen
//
//---------------------------------------------------------------------------

#include <CPixel32.h>

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:		operator==
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CPixel32::operator==(CPixel32& p2)
{
    return *((int*)rgba) == *((int*)p2.rgba);
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CPixel32::operator!=(CPixel32& p2)
{
    return *((int*)rgba) != *((int*)p2.rgba);
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:   SwapRedBlue
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CPixel32::SwapRedBlue()
{
    unsigned char swap = rgba[0];

    rgba[0] = rgba[2];
    rgba[2] = swap;
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:   Set
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CPixel32::Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:   Brightness
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CPixel32::Brightness()
{
    return (int)rgba[0] + rgba[1] + rgba[2];
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:		GetInt
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CPixel32::GetInt()
{
    return *((int*)rgba);
}
