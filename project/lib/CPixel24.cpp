//---------------------------------------------------------------------------
//
// PROJECT : Die Planeten
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include <iostream>

using namespace std;

#include <CPixel24.h>



//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : operator ==
//
//
//
//---------------------------------------------------------------------------

bool CPixel24::operator==(CPixel24& p2)
{
	return ((rgb[0] == p2.rgb[0]) &&
					(rgb[1] == p2.rgb[1]) &&
					(rgb[2] == p2.rgb[2]));
}

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       :
//
//
//
//---------------------------------------------------------------------------

bool CPixel24::operator!=(CPixel24& p2)
{
	return ((rgb[0] != p2.rgb[0]) ||
					(rgb[1] != p2.rgb[1]) ||
					(rgb[2] != p2.rgb[2]));
}

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       :
//
//
//
//---------------------------------------------------------------------------

void CPixel24::SwapRedBlue()
{
	byte swap = rgb[0];

	rgb[0] = rgb[2];
	rgb[2] = swap;
}

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       :
//
//
//
//---------------------------------------------------------------------------

void CPixel24::Set(byte r, byte g, byte b)
{
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       :
//
//
//
//---------------------------------------------------------------------------

void CPixel24::Set(unsigned int b)
{
	unsigned char* c = (unsigned char*) &b;

	rgb[0] = c[0];
	rgb[1] = c[1];
	rgb[2] = c[2];
}

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       :
//
//
//
//---------------------------------------------------------------------------

int CPixel24::Brightness()
{
	return rgb[0] + rgb[1] + rgb[2];
}

