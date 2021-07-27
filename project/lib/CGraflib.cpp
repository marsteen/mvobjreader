//------------------------------------------------------------------------------
//
// PROJECT : GLUT test application
//
// FILE    : CGraflib.cpp
//
// VERSION : 1.0
//
// AUTHOR  : Martin Steen
//
//
//
//
//
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>

using namespace std;

#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CPixel24.h>
#include <CPixel32.h>
#include <CDataRect.h>
#include <CGraflib.h>

//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		Yflip
//
//---------------------------------------------------------------------------

void CGraflib::SwapRedBlue()
{
    if (mBits == 24)
    {
        CPixel24* PixelPtr = (CPixel24*)mData;

        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                PixelPtr->SwapRedBlue();
                PixelPtr++;
            }
        }
    }
    else
    if (mBits == 32)
    {
        CPixel32* PixelPtr = (CPixel32*)mData;

        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                PixelPtr->SwapRedBlue();
                PixelPtr++;
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		Yflip
//
//---------------------------------------------------------------------------

void CGraflib::Yflip()
{
    unsigned char* PixelData = (unsigned char*)mData;
    int LineWidth = mWidth * (mBits / 8); // Breite des Bildes in Byte
    unsigned char* LineBuffer = new unsigned char[LineWidth];

    for (int y = (mHeight / 2) - 1; y >= 0; y--)
    {
        int off1 = y * LineWidth;
        int off2 = ((mHeight-1) - y) * LineWidth;

        memcpy(LineBuffer, PixelData + off2, LineWidth);
        memcpy(PixelData + off2, PixelData + off1, LineWidth);
        memcpy(PixelData + off1, LineBuffer, LineWidth);
    }
    delete[] LineBuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:   InterleaveAlpha
//
// Falls die Quelldaten 24 Pixel haben, wird ein neuer 32-Bit
// Datenblock erzeugt.
//
// Falls die Daten 32 Bit haben, wird der alte Alphakanal durch
// die neuen Alpha-Daten ersetzt.
//
//
//---------------------------------------------------------------------------

bool CGraflib::InterleaveAlpha(CGraflib* Alpha)
{
    bool r = false;

    if (Alpha->mBits == 8)
    {
        if ((mWidth == Alpha->mWidth) && (mHeight == Alpha->mHeight))
        {
            unsigned char* AlphaDataPtr = (unsigned char*)Alpha->mData;
            int Size = mWidth * mHeight;

            if (mBits == 24)
            {
                unsigned char* PixelDataPtr = (unsigned char*)mData;
                unsigned char* C32Data = new unsigned char[Size * 4];
                unsigned char* C32DataPtr = C32Data;

                for (int i = 0; i < Size; i++)
                {
                    C32DataPtr[0] = PixelDataPtr[0];
                    C32DataPtr[1] = PixelDataPtr[1];
                    C32DataPtr[2] = PixelDataPtr[2];
                    C32DataPtr[3] = *AlphaDataPtr;

                    PixelDataPtr += 3;
                    AlphaDataPtr += 1;
                    C32DataPtr += 4;
                }
                delete[] mData;
                mData = C32Data;
                mBits = 32;
                r = true;
            }
            else
            if (mBits == 32)
            {
                unsigned char* C32DataPtr = (unsigned char*)mData;
                for (int i = 0; i < Size; i++)
                {
                    C32DataPtr[3] = *AlphaDataPtr;
                    AlphaDataPtr += 1;
                    C32DataPtr += 4;
                }
                r = true;
            }
            else
            {
                // Fehler
            }
        }
    }
    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleLine2
//
// Skalieren einer Zeile um den Faktor 2
//
// LineOffset = Offset zur nächsten Zeile in Bytes
//
//---------------------------------------------------------------------------

void CGraflib::DownScaleLine2(void* LineBuffer, void* ErgBuffer, int LineOffset)
{
/*
 * cout << "Width=" << mWidth << endl;
 * cout << "Height=" << mHeight << endl;
 *  cout << "LineOffset=" << LineOffset << endl;
 */
    int k = (mBits == 24) ? 3 : 1;

    for (int b = 0; b < k; b++)
    {
        //cout << "b=" << b << endl;

        DownScaleLine2(LineBuffer, ErgBuffer, mWidth, LineOffset, b);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		DownScaleLine2
//
// Skalieren einer Zeile um den Faktor 2
//
// LineOffset = Offset zur nächsten Zeile in Bytes
//
//---------------------------------------------------------------------------

void CGraflib::DownScaleLine2(void* LineBuffer, void* ErgBuffer, int Width, int LineOffset, int b)
{
    int w2 = Width / 2;

    switch (mBits)
    {
        case 8:
        {
            unsigned char* LinePtr0 = (unsigned char*)LineBuffer;
            unsigned char* LinePtr1 = ((unsigned char*)LineBuffer) + LineOffset;
            unsigned char* ErgPtr = (unsigned char*)ErgBuffer;

            for (int i = 0; i < w2; i++)
            {
                *(ErgPtr++) = (LinePtr0[0] + LinePtr0[1] + LinePtr1[0] + LinePtr1[1]) >> 2;
                LinePtr0 += 2;
                LinePtr1 += 2;
            }
        }
        break;

        case 16:
        {
            unsigned short* LinePtr0 = (unsigned short*)LineBuffer;
            unsigned short* LinePtr1 = ((unsigned short*)LineBuffer) + LineOffset;
            unsigned short* ErgPtr = (unsigned short*)ErgBuffer;

            for (int i = 0; i < w2; i++)
            {
                *(ErgPtr++) = (LinePtr0[0] + LinePtr0[1] + LinePtr1[0] + LinePtr1[1]) >> 2;
                LinePtr0 += 2;
                LinePtr1 += 2;
            }
        }
        break;

        case 24:
        {
            unsigned char* LinePtr0 = (unsigned char*)LineBuffer + b;
            unsigned char* LinePtr1 = ((unsigned char*)LineBuffer) + LineOffset + b;
            unsigned char* ErgPtr = (unsigned char*)ErgBuffer + b;

            for (int i = 0; i < w2; i++)
            {
                //cout << "  i=" << i << endl;

                *ErgPtr = (LinePtr0[0] + LinePtr0[3] + LinePtr1[0] + LinePtr1[3]) >> 2;
                LinePtr0 += 6;
                LinePtr1 += 6;
                ErgPtr += 3;
            }
        }
        break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		GreyScale
//
//
//---------------------------------------------------------------------------

void CGraflib::GreyScale24bit()
{
    unsigned char* DataPtr = (unsigned char*)mData;

    if (mBits == 24)
    {
        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                unsigned char GreyVal = (DataPtr[0] + DataPtr[1] + DataPtr[2]) / 3;
                DataPtr[0] = DataPtr[1] = DataPtr[2] = GreyVal;
                DataPtr += 3;
            }
        }
    }
}
