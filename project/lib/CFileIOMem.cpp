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
#include <fstream>

using namespace std;

#include <CFileIO.h>
#include <CFileIOMem.h>

// ---------------------------------------------------------------------------
//
// KLASSE        : CFileIOMem
// METHODE       : ReadBytes
//
//
//
// ---------------------------------------------------------------------------

bool CFileIOMem::ReadBytes(void* buffer, int nBytes)
{
	if (mReadMode == EFILEREADMODE_FILE)
	{
		return CFileIO::ReadBytes(buffer, nBytes);
	}
	memcpy(buffer, mMemPtr, nBytes);
	mMemPtr += nBytes;
	return true;
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CFileIOMem
// METHODE       : ReadBytes
//
//
//
// ---------------------------------------------------------------------------

void* CFileIOMem::ReadBytes(int nBytes)
{
	if (mReadMode == EFILEREADMODE_FILE)
	{
		return CFileIO::ReadBytes(nBytes);
	}

	//cout << "CFileIOMem ReadBytes;" << nBytes << endl;
	char* ReadBuffer = new char[nBytes];
	memcpy(ReadBuffer, mMemPtr, nBytes);
	mMemPtr += nBytes;
	return ReadBuffer;

}

// ---------------------------------------------------------------------------
//
// KLASSE        : CFileIOMem
// METHODE       : CloseFile
//
//
//
// ---------------------------------------------------------------------------

void CFileIOMem::CloseFile()
{
	if (mReadMode == EFILEREADMODE_FILE)
	{
		CFileIO::CloseFile();
		return;
	}
	mOpenRead = false;
}
