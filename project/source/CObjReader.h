#ifndef COBJREADER_H
#define COBJREADER_H

#include <vector>
#include <set>
#include <CVector3T.h>
#include <fstream>
#include "S3dfMaterial.h"

enum
{
	OBJ_FLAG_TEXT = 1,
	OBJ_FLAG_NORM = 2,
	OBJ_FLAG_MAT  = 4    // Material
};


struct SFaceIndex
{
	CVector3<int> mIndex[3];
};


struct SObjGroup
{
	SObjGroup()
	{
		mFlags = 0;
	}

	std::string               mName;
	std::string               mMatName;  // Material-Name
	unsigned int              mMatIndex; // Material-Index
	std::vector<SFaceIndex*>  mFaceVec;
	unsigned int              mFlags;
};


struct STextureFilename
{
	bool operator<(const STextureFilename& k) const
	{
		return mFilename < k.mFilename;
	}

	std::string mFilename;
	std::string mFullname;
};


class CObjReader
{
  public:

    CObjReader();
    ~CObjReader();

		int ReadMtl(const char* Filename);
    int ReadObj(const char* Filename);
		void Write3df(const char* Filename);
		void WriteJS(const std::string& Filename);

  protected:



		void WriteGroups(CFileIO& fio);
		void WriteTextures(CFileIO& fio);
		void WriteMaterials(CFileIO& fio);
		void WriteString(CFileIO& fio, const std::string& str);

		void WriteGroupsJS(std::ofstream& outf);
		void WriteMaterialsJS(std::ofstream& outf);


		void SortMaterialSetIndex(std::vector<S3dfMaterial>& MaterialVec);
		void WriteMaterial(CFileIO& fio, const S3dfMaterial& mat);
    std::vector<CVector3<float> >* mVertexVec;
		std::vector<CVector2<float> >* mTexCoordVec;
    std::vector<CVector3<float> >* mNormalVec;

		std::string                mObjNameJS;
		std::string                mMtlFilename;
		std::string                mObjFilename;
		std::vector<SObjGroup*>    mGroupVec;
		std::set<S3dfMaterial>     mMaterialSet;
		std::set<STextureFilename> mTextureFilesSet;

};

#endif