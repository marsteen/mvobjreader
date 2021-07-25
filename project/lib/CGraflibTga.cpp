//------------------------------------------------------------------------------
//
// PROJECT : GLUT test application
//
// FILE    : CGraflibTga.cpp
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
#include <CFileIOMem.h>
#include <SFileReadInfo.h>
#include <CGraflibTga.h>
#include <CPixel24.h>
#include <CPixel32.h>
#include <GSystemFunctions.h>

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Read
//
// Parameter: Filename
//
// Return:
//
//---------------------------------------------------------------------------

template <class TPixelType>
TPixelType* CTgaKompressor<TPixelType>::Decompress(unsigned char *src, void* PixelBuffer, unsigned int psize)
{
 	unsigned int count = 0;
  unsigned char c;
  TPixelType    w;

	//cout  << "Uncompressing Pixelbuffer=" << (int) PixelBuffer << endl;

	if (PixelBuffer == NULL)
	{
  	PixelBuffer = new TPixelType[psize];
	}
  TPixelType* dest = (TPixelType*) PixelBuffer;

  //cout.flags(ios::hex);

  while (count < psize)
  {
    c = *src++;

    if (c & 0x80)  /* komprimierte Daten */
    {
      c &= 0x7F;
				w  = *((TPixelType*) src);
      src += sizeof(TPixelType);

      for (int l = 0; l <= c; l++)
      {
        *(dest++) = w;
      }
      count += c + 1;
    }
    else /* unkomprimierte Daten */
    {
			c += 1;
			memcpy(dest, src, c * sizeof(TPixelType));
			src   += c * sizeof(TPixelType);
			dest  += c;
			count += c;
    }
  }
  //cout.flags(ios::dec);

  return (TPixelType*) PixelBuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CTgaKompressor
// Methode:
//
// CGraphicsTga::Comp8bit(byte *src, byte *dest, int xres, int yres, int LineOffset)
//
//---------------------------------------------------------------------------

template <class TPixelType>
unsigned int CTgaKompressor<TPixelType>::Compress(void* Pixels, unsigned char* dest, int Width, int Height, int LineOffset)
{
	TPixelType* src = (TPixelType*) Pixels;

  int  ycount,xcount;
  byte *sdest;


  sdest = dest; /* Anfang des Zielspeichers */

  for (ycount = 0; ycount < Height; ycount++)
  {
    xcount = 0;
    while (xcount < Width)
    {
      if (src[0] == src[1]) /* Zwei Pixel gleich */
      {

        int  x = 2;
        TPixelType cwert = src[0];

        while ((src[x] == cwert) && (x < 127)) x++;

        if (x + xcount > Width) x = (int) (Width - xcount);

        src    += x;
        xcount += x;

        x |= 0x80;

        *dest++ = (unsigned char) x - 1; /* Zaehler speichern */
        *((TPixelType*) dest) = cwert;   /* Pixelwert speichern */
        dest += sizeof(TPixelType);
      }
      else
      {
        int x = 0;

        while ((src[x] != src[x+1]) && (x < 127))
				{
					x++;
				}

        if (x + xcount > Width)
				{
					x = (int) (Width - xcount);
				}

        *dest++ = (byte) x - 1;

        memcpy(dest, src, sizeof(TPixelType) * x);
        dest += sizeof(TPixelType) * x;

        src    += x;
        xcount += x;
      }
    }
    src += LineOffset;
  }
  return dest - sdest;
}



//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Write
//
// Parameter: Filename - Dateiname
//            SFileReadInfo* fri - Zeiger auf Datei-Informationen (oder NULL)
//
// Return:
//
//---------------------------------------------------------------------------

int CGraflibTga::Write(const char* Filename, bool bCompFlag, CFileIO* fio)
{
	cout << "CGraflibTga::Write " << Filename << endl;

	CFileIO        fioRead;
	CFileIO*       fioPtr;
	STgaHeader     TgaHeader;
	unsigned char* CompressedBuffer;
	int            DataSize;

	if (fio != NULL)
	{
		fioPtr   = fio;
	}
	else
	{
		fioPtr = &fioRead;
		if (!fioPtr->OpenFileWrite(Filename, ios::binary))
		{
			// Datei konnte nicht zum Schreiben ge�ffent werden
			return false;
		}
	}

  memset(&TgaHeader, 0, sizeof(STgaHeader));

  TgaHeader.mWidth  = mWidth;
  TgaHeader.mHeight = mHeight;
  TgaHeader.mPsize  = mBits;
  TgaHeader.mIbyte  = 0x20;

  //cout << "W=" << mWidth << " H=" << mHeight << " Bits=" << mBits << endl;


  if (bCompFlag)
  {
	  CompressedBuffer = new unsigned char[mWidth * mHeight * (mBits/8) * 2];

		CTgaKompbase* TgaComp = NULL;

		switch (mBits)
		{
			case 8:
			{
				TgaComp = new CTgaKompressor<unsigned char>;

				if (mPalette != NULL)
				{
					TgaHeader.mItype   = ETGA_ITYPE_8BIT_COMPRESSED;
					TgaHeader.mCtype   = 1;
					TgaHeader.mCmap[3] = 1;
					TgaHeader.mCmap[4] = 24;
				}
				else
				{
					TgaHeader.mItype  = ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME;
				}
			}
			break;

			case 16:
			{
				TgaComp = new CTgaKompressor<unsigned short>;
				TgaHeader.mItype  = ETGA_ITYPE_RGB_COMPRESSED;
			}
			break;

			case 24:
			{
				TgaComp = new CTgaKompressor<CPixel24>;
				TgaHeader.mItype  = ETGA_ITYPE_RGB_COMPRESSED;
			}
			break;
		}

 		DataSize = TgaComp->Compress(mData, CompressedBuffer, mWidth, mHeight, 0);
 		delete[] TgaComp;
	}
	else
	{
		switch (mBits)
		{
			case 8:

				if (mPalette != NULL)
				{
					TgaHeader.mItype = ETGA_ITYPE_8BIT_UNCOMPRESSED;
					TgaHeader.mCtype   = 1;
					TgaHeader.mCmap[3] = 1;
					TgaHeader.mCmap[4] = 24;
				}
				else
				{
					TgaHeader.mItype = ETGA_ITYPE_8BIT_UNCOMPRESSED_MONOCHROME;
				}
				break;

			case 16:

				TgaHeader.mItype  = ETGA_ITYPE_RGB_UNCOMPRESSED;
				break;

			case 24:

				TgaHeader.mItype  = ETGA_ITYPE_RGB_UNCOMPRESSED;
				break;
		}
		DataSize = mWidth * mHeight * (mBits/8);
	}

	fioPtr->WriteBytes(&TgaHeader, sizeof(STgaHeader));

	if ((mBits == 8) && (mPalette != NULL))
	{
		fioPtr->WriteBytes(mPalette, 256 * 3);
	}

	if (bCompFlag)
	{
		fioPtr->WriteBytes(CompressedBuffer, DataSize);
		delete[] CompressedBuffer;
	}
	else
	{
		fioPtr->WriteBytes(mData, DataSize);
	}


	if (fio == NULL)
	{
		fioPtr->CloseFile();
	}
	return DataSize + sizeof(TgaHeader);
}

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Read
//
// Parameter: Filename - Dateiname
//            SFileReadInfo* fri - Zeiger auf Datei-Informationen (oder NULL)
//
// Return:
//
//---------------------------------------------------------------------------

bool CGraflibTga::Read(const char* Filename, SFileReadInfo* fri)
{

	//cout << "CGraflibTga::Read START " << Filename <<  endl;

	CFileIO      fio;
	CFileIO*     fioPtr;
	unsigned int FileSize;
	STgaHeader   TgaHeader;
	void*        DstData;

	if (fri != NULL)
	{
		//cout << " FRI != NULL!!" << endl;

		FileSize = fri->mFileSize;
		fioPtr   = fri->mFio;
		DstData  = fri->mData;
		fioPtr->SetReadPos(fri->mFilePosition);
	}
	else
	{
		FileSize = fio.GetFileSize(Filename);
		fioPtr = &fio;
		DstData = NULL;
		if (!fioPtr->OpenFileRead(Filename, ios::in | ios::binary))
		{
			// Datei  nicht gefunden
			return false;
		}
		//cout << "Filesize=" << FileSize << endl;
	}
	//cout << "Filesize=" << FileSize << endl;

  fioPtr->ReadBytes(&TgaHeader, sizeof(STgaHeader));

	if (GsysBigEndian())
	{
		GsysSwapWord(&TgaHeader.mWidth);
		GsysSwapWord(&TgaHeader.mHeight);
	}

  //cout << "OK " << FileSize << endl;

	mWidth  = TgaHeader.mWidth;
	mHeight = TgaHeader.mHeight;
	mBits   = TgaHeader.mPsize;


	int PicSize = mWidth * mHeight;


	unsigned int DataLenght = FileSize - sizeof(STgaHeader);

	//cout << " sizeof(STgaHeader)=" << sizeof(STgaHeader) << endl;


/*
	cout   << " CGraflibTga::Read Bits=" << (int) TgaHeader.mPsize << '\n'
	       << " Ident=" << (int) TgaHeader.mIdent << '\n'
	       << " Ctype=" << (int) TgaHeader.mCtype << '\n'
	       << " Itype=" << (int) TgaHeader.mItype << '\n'
	       << " Width=" << mWidth << '\n'
	       << " Height=" << mHeight << '\n'
	       << " Datalength=" << DataLenght << endl;
*/

  if (fri != NULL)
  {
		/*
		cout << " Filepos=" << (int) fri->mFilePosition << '\n'
	       << " Filesize=" << (int) fri->mFileSize << endl;
    */
	}

/*
	Debug3 << " CGraflibTga::Read Bits=" << (int) TgaHeader.mPsize
	       << " Itype=" << (int) TgaHeader.mItype << '\n'
	       << " Width=" << mWidth << '\n'
	       << " Height=" << mHeight << '\n'
	       << " Filepos=" << (int) fri->mFilePosition << '\n'
	       << " Filesize=" << (int) fri->mFileSize << DBLF;
*/

	switch (mBits)
	{
		// 8 Bit Graustufen oder Farbpaletten-Pixeldaten

		case 8:

			//cout << "reading 8 Bit" << endl;

			if (TgaHeader.mCtype == 1) // Farbpalette vorhanden
			{
				short* mCmapOrigin    = (short*) TgaHeader.mCmap;
				short* mCmapLenght    = (short*) (TgaHeader.mCmap + 2);
			  unsigned char* mCmapEntrysize = TgaHeader.mCmap + 4;

				if (GsysBigEndian())
        {
					GsysSwapWord(mCmapOrigin);
					GsysSwapWord(mCmapLenght);
        }
        /*
				cout << "  Colorpal origin: " << *mCmapOrigin << endl;
			  cout << "  Colorpal entries:" << *mCmapLenght << endl;
			  cout << "  Colorpal entry size:" << (int) *mCmapEntrysize << endl;
				*/

			  int CpalSize = *mCmapLenght * (*mCmapEntrysize/8);

			  //cout << "CpalSize=" << CpalSize << endl;

				mPalette = fioPtr->ReadBytes(CpalSize);
				DataLenght -= CpalSize;

				//cout << "Read Palette OK (" << CpalSize << " bytes)" << endl;
			}
			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_8BIT_UNCOMPRESSED:            // Color Map unkomprimiert
				case ETGA_ITYPE_8BIT_UNCOMPRESSED_MONOCHROME: // Monochrom unkomprimiert

					mData = (unsigned char*) fioPtr->ReadBytes(DataLenght);
					break;

				case ETGA_ITYPE_8BIT_COMPRESSED:
				case ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME:
					{
						unsigned char* CompressedData = (unsigned char*) fioPtr->ReadBytes(DataLenght);
						CTgaKompressor<unsigned char> Decomp8;
						mData = Decomp8.Decompress(CompressedData, DstData, PicSize);
						delete[] CompressedData;
					}
					break;
			 }
			 break;

		// 16 Bit Pixeldaten

		case 16:

			//cout << "reading 16 Bit" << endl;

			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_RGB_UNCOMPRESSED:

					mData = (unsigned char*) fioPtr->ReadBytes(DataLenght);
					break;

				case ETGA_ITYPE_RGB_COMPRESSED:
					{
						unsigned char* CompressedData = (unsigned char*) fioPtr->ReadBytes(DataLenght);

						//cout << "CompressedData=" << (int) CompressedData << endl;

						CTgaKompressor<unsigned short> Decomp16;
						mData = (unsigned char*) Decomp16.Decompress(CompressedData, DstData, PicSize);

						//Debug3 << " CGraflibTga::Read mData=" << (int) mData << DBLF;

						delete[] CompressedData;

					}
					break;
			 }
			 break;

		// 24 Bit Pixeldaten

		case 24:

			//cout << "reading 24 Bit" << endl;

			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_RGB_UNCOMPRESSED:

					//cout << "  Reading uncompressed data.." << endl;
					//mData = fioPtr->ReadBytes(DataLenght);
					mData = (unsigned char*) fio.ReadBytes(DataLenght); //

					//memset(mData, 0x7F, DataLenght);
					break;

				case ETGA_ITYPE_RGB_COMPRESSED:
					{
						//cout << "  Decompressing 24 Bit Data" << endl;

						unsigned char* CompressedData = (unsigned char*) fioPtr->ReadBytes(DataLenght);

						CTgaKompressor<CPixel24> Decomp24;
						mData = (unsigned char*) Decomp24.Decompress(CompressedData,  DstData, PicSize);
						delete[] CompressedData;
					}
					break;

				default:

					cout << "***** CGraflibTga::Read - Unsupported Image Type:" << TgaHeader.mItype << endl;
					break;

			 }
			 break;

		// 32 Bit Pixeldaten

		case 32:

			//cout << "reading 32 Bit" << endl;

			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_RGB_UNCOMPRESSED:

					mData = (unsigned char*) fio.ReadBytes(DataLenght); //
					break;

				case ETGA_ITYPE_RGB_COMPRESSED:
					{
						cout << "  Decompressing 32 Bit Data" << endl;

						unsigned char* CompressedData = (unsigned char*) fioPtr->ReadBytes(DataLenght);

						CTgaKompressor<CPixel32> Decomp32;
						mData = (unsigned char*) Decomp32.Decompress(CompressedData,  DstData, PicSize);
						delete[] CompressedData;
					}
					break;

				default:

					cout << "***** CGraflibTga::Read - Unsupported Image Type:" << TgaHeader.mItype << endl;
					break;

			 }
			 break;

	}

  if ((TgaHeader.mIbyte & 0x30) == 0) // Ursprung der Grafik unten links?
  {
    Yflip();
  }


	if (fri == NULL)
	{
		fioPtr->CloseFile();
	}

	//cout << "CGraflibTga::Read OK" << endl;
	return true;
}


#if 1

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibTga
// Methode:   Read
//
// Parameter: Filename - Dateiname
//            SFileReadInfo* fri - Zeiger auf Datei-Informationen (oder NULL)
//
// Return:
//
//---------------------------------------------------------------------------


bool CGraflibTga::ReadMem(const char* MemPtr, unsigned int FileSizeMem)
{
	//cout << "CGraflibTga::ReadMem START" << endl;

	CFileIOMem   fim;
	STgaHeader   TgaHeader;
	void*        DstData = NULL;

	fim.OpenFileMem(MemPtr, FileSizeMem);
  fim.ReadBytes(&TgaHeader, sizeof(STgaHeader));

	if (GsysBigEndian())
	{
		GsysSwapWord(&TgaHeader.mWidth);
		GsysSwapWord(&TgaHeader.mHeight);
	}

	mWidth  = TgaHeader.mWidth;
	mHeight = TgaHeader.mHeight;
	mBits   = TgaHeader.mPsize;

	/*
	cout << "Width=" << mWidth << endl;
	cout << "Height=" << mHeight << endl;
	cout << "FileSizeMem=" << FileSizeMem << endl;
	*/

	int PicSize = mWidth * mHeight;

	unsigned int DataLenght = FileSizeMem - sizeof(STgaHeader);

	switch (mBits)
	{
		// 8 Bit Graustufen oder Farbpaletten-Pixeldaten

		case 8:

			if (TgaHeader.mCtype == 1) // Farbpalette vorhanden
			{
				short* mCmapOrigin    = (short*) TgaHeader.mCmap;
				short* mCmapLenght    = (short*) (TgaHeader.mCmap + 2);
			  unsigned char* mCmapEntrysize = TgaHeader.mCmap + 4;

				if (GsysBigEndian())
        {
					GsysSwapWord(mCmapOrigin);
					GsysSwapWord(mCmapLenght);
        }
        /*
				cout << "  Colorpal origin: " << *mCmapOrigin << endl;
			  cout << "  Colorpal entries:" << *mCmapLenght << endl;
			  cout << "  Colorpal entry size:" << (int) *mCmapEntrysize << endl;
				*/

			  int CpalSize = *mCmapLenght * (*mCmapEntrysize/8);

			  //cout << "CpalSize=" << CpalSize << endl;

				mPalette = fim.ReadBytes(CpalSize);
				DataLenght -= CpalSize;

				//cout << "Read Palette OK (" << CpalSize << " bytes)" << endl;
			}
			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_8BIT_UNCOMPRESSED:            // Color Map unkomprimiert
				case ETGA_ITYPE_8BIT_UNCOMPRESSED_MONOCHROME: // Monochrom unkomprimiert

					mData = (unsigned char*) fim.ReadBytes(DataLenght);
					break;

				case ETGA_ITYPE_8BIT_COMPRESSED:
				case ETGA_ITYPE_8BIT_COMPRESSED_MONOCHROME:
					{
						unsigned char* CompressedData = (unsigned char*) fim.ReadBytes(DataLenght);
						CTgaKompressor<unsigned char> Decomp8;
						mData = Decomp8.Decompress(CompressedData, DstData, PicSize);
						delete[] CompressedData;
					}
					break;
			 }
			 break;

		// 16 Bit Pixeldaten

		case 16:

			//cout << "reading 16 Bit" << endl;

			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_RGB_UNCOMPRESSED:

					mData = (unsigned char*) fim.ReadBytes(DataLenght);
					break;

				case ETGA_ITYPE_RGB_COMPRESSED:
					{
						unsigned char* CompressedData = (unsigned char*) fim.ReadBytes(DataLenght);

						//cout << "CompressedData=" << (int) CompressedData << endl;

						CTgaKompressor<unsigned short> Decomp16;
						mData = (unsigned char*) Decomp16.Decompress(CompressedData, DstData, PicSize);

						//Debug3 << " CGraflibTga::Read mData=" << (int) mData << DBLF;

						delete[] CompressedData;

					}
					break;
			 }
			 break;

		// 24 Bit Pixeldaten

		case 24:

			//cout << "reading 24 Bit" << endl;

			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_RGB_UNCOMPRESSED:

					mData = (unsigned char*) fim.ReadBytes(DataLenght); //
					break;

				case ETGA_ITYPE_RGB_COMPRESSED:
					{
						unsigned char* CompressedData = (unsigned char*) fim.ReadBytes(DataLenght);

						CTgaKompressor<CPixel24> Decomp24;

						mData = (unsigned char*) Decomp24.Decompress(CompressedData,  DstData, PicSize);
						delete[] CompressedData;

					}
					break;

				default:

					cout << "***** CGraflibTga::Read - Unsupported Image Type:" << TgaHeader.mItype << endl;
					break;

			 }
			 break;

		// 32 Bit Pixeldaten

		case 32:

			//cout << "reading 32 Bit" << endl;

			switch (TgaHeader.mItype)
			{
				case ETGA_ITYPE_RGB_UNCOMPRESSED:

					mData = (unsigned char*) fim.ReadBytes(DataLenght); //
					break;

				case ETGA_ITYPE_RGB_COMPRESSED:
					{
						unsigned char* CompressedData = (unsigned char*) fim.ReadBytes(DataLenght);

						CTgaKompressor<CPixel32> Decomp32;

						mData = (unsigned char*) Decomp32.Decompress(CompressedData,  DstData, PicSize);
						delete[] CompressedData;

					}
					break;

				default:

					cout << "***** CGraflibTga::Read - Unsupported Image Type:" << TgaHeader.mItype << endl;
					break;

			 }
			 break;

	}

  if ((TgaHeader.mIbyte & 0x30) == 0) // Ursprung der Grafik unten links?
  {
    Yflip();
  }
	fim.CloseFile();

	return true;
}

#endif
