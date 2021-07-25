#ifndef CFILEIOMEM_H
#define CFILEIOMEM_H


enum
{
	EFILEREADMODE_FILE,
	EFILEREADMODE_MEM,
};

class CFileIOMem : public CFileIO
{
	public:

		CFileIOMem()
		{
			mMemPtr = NULL;
			mReadMode = EFILEREADMODE_FILE;
		}

		void OpenFileMem(const char* Mem, int FileSize)
		{
			mReadMode = EFILEREADMODE_MEM;
			mMemBase = Mem;
			mMemPtr  = Mem;
			mOpenRead = true;
		};

		void CloseFile(void);

		bool 	ReadBytes(void* buffer, int nBytes);
		void*	ReadBytes(int nBytes);

	protected:

		int         mReadMode;
		const char* mMemPtr;
		const char* mMemBase;

};

#endif



