//
// PROJECT : $project$
//
// FILE    : $filename$
//
// VERSION : $version$
//
// AUTOR   : $autor$
//
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Implementation der Klasse CObjReader
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 10.10.2010 erstellt - Martin Steen
//
//------------------------------------------------------------------------------


#include <CFileIO.h>
#include <NStringTool.h>
#include <CVector3T.h>
#include <algorithm>
#include <GSystemFunctions.h>
#include "CObjReader.h"

using namespace std;

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   CObjReader
//
//
//---------------------------------------------------------------------------

CObjReader::CObjReader()
{
  mVertexVec = NULL;
  mNormalVec = NULL;
  mTexCoordVec = NULL;
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   ~CObjReader
//
//
//---------------------------------------------------------------------------

CObjReader::~CObjReader()
{
  cout << "delete" << endl;

  if (mVertexVec != NULL)
  {
    mVertexVec->clear();
    delete mVertexVec;
  }
  if (mNormalVec != NULL)
  {
    mNormalVec->clear();
    delete mNormalVec;
  }
  if (mTexCoordVec != NULL)
  {
    mTexCoordVec->clear();
    delete mTexCoordVec;
  }
  cout << "delete OK" << endl;
}

/*
struct SIndexTriple : public CVector3<int>
{
	int mIndex;

	bool operator<(const SIndexTriple& k) const
	{
		if (x == k.x)
		{
			if (y == k.y)
			{
				return z < k.z;
			}
			return y < k.y;
		}
		return x < k.x;
	}
};


void AddIndex(const CVector3<int> Index)
{
	static int NewIndex;
	SIndexTriple ti;
	ti.x = Index.x;
	ti.y = Index.y;
	ti.z = Index.z;

	set<SIndexTriple>::iterator it = mIndexSet.find(ti);
	if (it == mIndexSet.end())
	{
		ti.mIndex = NewIndex++;
		mIndexSet.insert(ti);
	}
}
*/

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   ParseFaceIndex
//
//
//---------------------------------------------------------------------------

static int ParseFaceIndex(const string& FaceStr)
{
	int Index;
	if (FaceStr.size() == 0)
	{
		Index = -1;
		//cout << "-1" << endl;
	}
	else
	{
		Index = NStringTool::Cast<int>(FaceStr) - 1;
	}
	return Index;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ToLowerString
//
//
//---------------------------------------------------------------------------

static string ToLowerString(const string& s)
{
	string LowerString;
	for (int i = 0; i < s.size(); i++)
	{
		LowerString += tolower(s[i]);
	}
	return LowerString;
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   MakeName
//
//
//---------------------------------------------------------------------------

static string MakeName(const stringvector& sv)
{
	string Name;
	for (int i = 1; i < sv.size(); i++)
	{
		Name += sv[i];
		if (i < sv.size() - 1)
		{
			Name += "_";
		}
	}
	cout << "Name=" << Name << endl;
	
	return Name;
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   ReadMtl
//
//
//---------------------------------------------------------------------------

int CObjReader::ReadMtl(const char* Filename)
{
	bool r = false;
	CFileIO fio;
	unsigned int MatIndex = 0;
	mMtlFilename = Filename;

	cout << "CObjReader::ReadMtl START:" << Filename << endl;

  if (fio.OpenFileRead(Filename, ios::binary))
  {
    stringvector SplitVector;
		S3dfMaterial Material;
		bool HasMat = false;

		r = true;

    while (fio.ReadSplitLine(&SplitVector, ' '))
		{
      cout << "SplitVector.size()=" << SplitVector.size() << endl;
      
			string cmd;
			if (SplitVector.size() > 0)
			{
			  cmd = SplitVector[0];
			}

			//cout << "cmd=" << cmd << endl;

			if (cmd == "newmtl")  // Neues Material
			{
				if (HasMat)
				{
					mMaterialSet.insert(Material);
				}
				HasMat = true;
				Material.mIndex = MatIndex++;


				Material.mName  = MakeName(SplitVector);
				Material.mTextureFile.clear();
				cout << "New Material:" << Material.mName << endl;
			}
			else
			if (cmd == "map_Kd") // Textur
			{
				STextureFilename TextureFilename;
				Material.mTextureFile = ToLowerString(SplitVector[1]);
				cout << "Material.mTextureFile=" << Material.mTextureFile << endl;
  		  string PathName = NStringTool::Pathname(mMtlFilename, '/');

  		  cout << "PATHDIFF=" << PATHDIFF << endl;
  		  cout << "mMtlFilename=" << mMtlFilename << endl;
  		  cout << "PathName=" << PathName << endl;


				TextureFilename.mFilename = Material.mTextureFile;
				if (PathName.size() > 0)
				{
					TextureFilename.mFullname = PathName + PATHDIFF +  Material.mTextureFile;
				}
				else
				{
					TextureFilename.mFullname = Material.mTextureFile;
				}
				cout << "Material.mTextureFile=" << TextureFilename.mFullname  << endl;

				set<STextureFilename>::iterator it = mTextureFilesSet.find(TextureFilename);
				if (it == mTextureFilesSet.end())
				{
					mTextureFilesSet.insert(TextureFilename);
				}

				cout << "ok" << endl;
			}
			else
			if (cmd == "Ns")
			{
				Material.mNs = NStringTool::Cast<float>(SplitVector[1]);
			}
			else
			if (cmd == "d")
			{
				Material.mTransparency = NStringTool::Cast<float>(SplitVector[1]);
			}
			else
			if (cmd == "illum")
			{
				Material.mIllum = NStringTool::Cast<float>(SplitVector[1]);
			}
			else
			if (cmd == "Kd")
			{
				for (int i = 1; i < SplitVector.size(); i++)
				{
					Material.mDiffuse[i-1] = NStringTool::Cast<float>(SplitVector[i]);
				}
			}
			else
			if (cmd == "Ka")
			{
				for (int i = 1; i < SplitVector.size(); i++)
				{
					Material.mAmbient[i-1] = NStringTool::Cast<float>(SplitVector[i]);
				}
			}
			else
			if (cmd == "Ks")
			{
				for (int i = 1; i < SplitVector.size(); i++)
				{
					Material.mSpecular[i-1] = NStringTool::Cast<float>(SplitVector[i]);
				}
			}
			else
			if (cmd == "Ke")
			{
				for (int i = 1; i < SplitVector.size(); i++)
				{
					Material.mEmission[i-1] = NStringTool::Cast<float>(SplitVector[i]);
				}
			}
		}
		if (HasMat)
		{
			cout << "Insert" << endl;
			mMaterialSet.insert(Material);
		}
	}
	cout << "CObjReader::ReadMtl OK r=" << r << endl;

	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   Read
//
//
//---------------------------------------------------------------------------

int CObjReader::ReadObj(const char* Filename)
{
  CFileIO fio;

  cout << "CObjReader::Read: " << Filename << endl;
  int TotalFaces = 0;

  mVertexVec   = new std::vector<CVector3<float> >;
  mNormalVec   = new std::vector<CVector3<float> >;
  mTexCoordVec = new std::vector<CVector2<float> >;

  if (fio.OpenFileRead(Filename, ios::binary))
  {
    //cout << "open ok" << endl;

    stringvector SplitVector;
		SObjGroup* Group = NULL;
    while (fio.ReadSplitLine(&SplitVector, ' '))
    {
      unsigned int SplitSize = SplitVector.size();
      if (SplitSize > 0)
      {
       // cout << "SplitSize=" << SplitSize << endl;

        string cmd = SplitVector[0];

        if (cmd == "g")  // Group-Deklaration
				{
					Group = new SObjGroup;
					mGroupVec.push_back(Group);
					Group->mName = MakeName(SplitVector);

					cout << "Group:" << Group->mName  << endl;
				}
				else
        if (cmd == "f")  // Face-Deklaration
        {
         // cout << "face" << endl;
					TotalFaces++;

				// z.Zt werden nur Dreiecke unterstuetzt

					if (SplitSize == 4)
					{
						SFaceIndex* Face = new SFaceIndex;
						CVector3<int> Index;

						for (int i = 0; i < 3; i++)
						{
							stringvector FaceStr;
							NStringTool::Split(SplitVector[i+1], &FaceStr, '/');

							switch (FaceStr.size())
							{
								case 0:

									Face->mIndex[i].x = ParseFaceIndex(SplitVector[i+1]);
									Face->mIndex[i].y = -1;
									Face->mIndex[i].z = -1;
									break;


								case 1:

									Face->mIndex[i].x = ParseFaceIndex(FaceStr[0]);
									Face->mIndex[i].y = -1;
									Face->mIndex[i].z = -1;
									break;

								case 2:

									Face->mIndex[i].x = ParseFaceIndex(FaceStr[0]);
									Face->mIndex[i].y = ParseFaceIndex(FaceStr[1]);
									Face->mIndex[i].z = -1;

									if (Face->mIndex[i].y >= 0)
									{
										Group->mFlags |= OBJ_FLAG_TEXT;
									}
									break;

								case 3:

									Face->mIndex[i].x = ParseFaceIndex(FaceStr[0]);
									Face->mIndex[i].y = ParseFaceIndex(FaceStr[1]);
									Face->mIndex[i].z = ParseFaceIndex(FaceStr[2]);


									if (Face->mIndex[i].y >= 0)
									{
										Group->mFlags |= OBJ_FLAG_TEXT;
									}
									if (Face->mIndex[i].z >= 0)
									{
										Group->mFlags |= OBJ_FLAG_NORM;
									}
									break;

									default:

										cout << "***** malformed face definition:" << SplitVector[i+1] << endl;
										cout << "***** FaceStr.size()=" << FaceStr.size() << endl;
										exit(0);
										break;

								} // switch
						} // for

						Group->mFaceVec.push_back(Face);
					}
					else
					{
						cout << "***** faces must have 3 components (not " << SplitSize << ")" << endl;
						exit(0);
					}
        }
        else
        if (cmd == "v")  // Vertex
        {
          if (SplitSize == 4)
          {
            CVector3<float> vec;

						//cout << "vertex" << endl;

            vec.x = NStringTool::Cast<float>(SplitVector[1]);
            vec.y = NStringTool::Cast<float>(SplitVector[2]);
            vec.z = NStringTool::Cast<float>(SplitVector[3]);
            mVertexVec->push_back(vec);
          }
        }
        else
        if (cmd == "vn") // Normale
        {
          if (SplitSize == 4)
          {
            CVector3<float> vec;
            //cout << "normal" << endl;
            vec.x = NStringTool::Cast<float>(SplitVector[1]);
            vec.y = NStringTool::Cast<float>(SplitVector[2]);
            vec.z = NStringTool::Cast<float>(SplitVector[3]);
						vec.Normalize();
            mNormalVec->push_back(vec);
          }
					else
					{
						cout << "***** malformed normal:" << SplitSize << endl;
						exit(0);
					}
        }
        else
        if (cmd == "vt") // Textur-Koordinate
        {

          if (SplitSize == 3)
          {
            CVector2<float> vec;
            //cout << "texture coord" << endl;
            vec.x = NStringTool::Cast<float>(SplitVector[1]);
            vec.y = NStringTool::Cast<float>(SplitVector[2]);
            mTexCoordVec->push_back(vec);
          }
        }
        else
        if (cmd == "usemtl")
        {
					S3dfMaterial mat;
          //cout << "usemtl:" << SplitVector[1] << endl;

					mat.mName = MakeName(SplitVector);
					set<S3dfMaterial>::iterator imat = mMaterialSet.find(mat);
					if (imat != mMaterialSet.end())
					{
						cout << "mat found:" << imat->mIndex << endl;

						if (Group == NULL)
						{
							// Group = NULL: Creating empty Group
						  Group = new SObjGroup;
							Group->mName ="Group0";
							mGroupVec.push_back(Group);

						}

						if (Group != NULL)
						{
							Group->mMatIndex = imat->mIndex;
							Group->mMatName  = mat.mName;
							Group->mFlags |= OBJ_FLAG_MAT;
						}
						else
						{
							cout << "***** GROUP=NULL!" << endl;
						}
					}
					else
					{
						cout << "***** Material not found: " << SplitVector[1] << endl;
					}
        }
      }
    }
    fio.CloseFile();

		cout << "Faces       :" << TotalFaces << endl;
    cout << "Vertexes    :" << mVertexVec->size() << endl;
    cout << "Normals     :" << mNormalVec->size() << endl;
    cout << "TexCoords   :" << mTexCoordVec->size() << endl;
		cout << "Groups      :" << mGroupVec.size() << endl;
  }
  cout << "CObjReader::Read ok" << endl;
}


void ShowFloats(const float* f, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << " " << f[i];
	}
	cout << endl;
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteMaterial
//
//---------------------------------------------------------------------------

void CObjReader::WriteMaterial(CFileIO& fio, const S3dfMaterial& mat)
{
	fio.WriteBytes("fmat", 4);
  WriteString(fio, mat.mTextureFile);

	fio.WriteObject(mat.mNs);
	fio.WriteObject(mat.mTransparency);
	fio.WriteObject(mat.mIllum);

	cout << "MaterialX:" << mat.mName << endl;
	cout << "  Diffuse:";
	ShowFloats(mat.mDiffuse, 3);

	cout << "  Ambient:";
	ShowFloats(mat.mAmbient, 3);

	fio.WriteObjects(mat.mDiffuse, 3);
	fio.WriteObjects(mat.mAmbient, 3);
	fio.WriteObjects(mat.mEmission, 3);
	fio.WriteObjects(mat.mSpecular, 3);
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   SortMaterialSetIndex
//
// Material nach Index sortieren
//
//---------------------------------------------------------------------------

static bool CompareIndex(const S3dfMaterial& a, const S3dfMaterial& b)
{
	return a.mIndex < b.mIndex;
}

void CObjReader::SortMaterialSetIndex(std::vector<S3dfMaterial>& MaterialVec)
{
  cout << "SortMaterialSetIndex START" << endl;
	set<S3dfMaterial>::iterator imat;
	for (imat = mMaterialSet.begin(); imat != mMaterialSet.end(); ++imat)
	{
    cout << "imat->mName=" << imat->mName << endl;
    
		MaterialVec.push_back(*imat);
	}
	std::sort(MaterialVec.begin(), MaterialVec.end(), CompareIndex);
  
  cout << "SortMaterialSetIndex OK" << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   Write3df
//
// Dateiformat:
//
// char[4] "3dfc"
// int32    Version  // 2
// int32    Anzahl der Texturen
// int32    Anzahl der Materialien
// int32    Anzahl der Gruppen
//
// Fuer jede Textur:
//    char[4] "ftex"
//    int32   Texture-Filenamelaenge // 0 = Kein Dateiname
//    char *  Texture-Filename       // Texture-Dateiname, endet auf 0
//    int32   Groeße der Texturdaten
//    Texture-Daten (TGA. JPG, PNG)
//
// Fuer jedes Material:
//    char[4] "fmat"
//    int32   Texture-Filenamelaenge // 0 = Kein Dateiname
//    char *  Texture-Filename // Texture-Dateiname, endet auf 0//
//    float   mNs;             // Ns Specular Exponent
//	  float   mTransparency;   // Transparenz (1.0 = Opaque)
//	  float   mIllum;          // illum Illumination
//    float   mDiffuse[4];     // Kd
//	  float   mAmbient[4];     // Ka
// 	  float   mEmission[4];    // Ke
//	  float   mSpecular[4];    // Ks
//

//
// Fuer jede Gruppe:
//   char[4]  "fgrp" Facegroup
//   char[32] Name der Gruppe
//   int32 - Flags
//   int32 - Material-Index
//   int32 - Anzahl der Faces
//   Vertex   * Anzahl der Faces
//   TexCoord * Anzahl der Faces (falls vorhanden)
//   Normalen * Anzahl der Faces (falls vorhanden)
//
//

//
//---------------------------------------------------------------------------


void  CObjReader::Write3df(const char* Filename)
{
	CFileIO fio;

	if (fio.OpenFileWrite(Filename, ios::binary))
	{
		int Version = 1;

		fio.WriteBytes("3dfc", 4);
		fio.WriteObject(Version);



		int GroupCount    = mGroupVec.size();
		int MaterialCount = mMaterialSet.size();
		int TextureCount  = mTextureFilesSet.size();

		cout << "GroupCount=" << GroupCount << endl;


		fio.WriteObject(TextureCount);
		fio.WriteObject(MaterialCount);
		fio.WriteObject(GroupCount);

		WriteTextures(fio);
		WriteMaterials(fio);
		WriteGroups(fio);

		fio.CloseFile();
		cout << "GroupCount=" << GroupCount << endl;
		cout << "MaterialCount=" << MaterialCount << endl;
		cout << "TextureCount=" << TextureCount << endl;

	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteMaterials
//
//
//---------------------------------------------------------------------------

void CObjReader::WriteMaterials(CFileIO& fio)
{
	vector<S3dfMaterial> MaterialVec;
	vector<S3dfMaterial>::iterator imat;
	SortMaterialSetIndex(MaterialVec);

	for (imat = MaterialVec.begin(); imat != MaterialVec.end(); ++imat)
	{
		cout << "Material Index=" << imat->mIndex <<  endl;
		WriteMaterial(fio, *imat);
	}
}


//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteString
//
//
//---------------------------------------------------------------------------


void CObjReader::WriteString(CFileIO& fio, const string& str)
{
	if (str.size() > 0)
	{
		int StringLength = str.size() + 1;
		fio.WriteObject(StringLength);
		fio.WriteBytes(str.c_str(), StringLength);
	}
	else
	{
		int StringLength = 0;
		fio.WriteObject(StringLength);
	}
}


//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteTextures
//
//
//---------------------------------------------------------------------------




void CObjReader::WriteTextures(CFileIO& fio)
{
	set<STextureFilename>::iterator itex;

	/*
	struct STextureFilenae
	{
		std::string mFilename;
		std::string mFullname;
	};
  */

	cout << "Texture Count=" << mTextureFilesSet.size() << endl;

	for (itex = mTextureFilesSet.begin(); itex != mTextureFilesSet.end(); ++itex)
	{
		cout << "Write Texture: Name="  << itex->mFilename << " file=" << itex->mFullname << endl;

		CFileIO fin;
		int Filesize;

		const char* TexData = (const char*) fin.ReadFile(itex->mFullname.c_str(), &Filesize);

		if (TexData != NULL)
		{
			fio.WriteBytes("ftex", 4);

			WriteString(fio, itex->mFilename);

			fio.WriteObject(Filesize);
			fio.WriteBytes(TexData, Filesize);
			delete[] TexData;

			cout << "Filesize=" << Filesize << endl;
		}
		else
		{
			cout << "***** cannot read file:" << itex->mFullname << endl;
			exit(0);
		}

		//cout << "Material Index=" << imat->mIndex <<  endl;
		//WriteMaterial(fio, *imat);
	}
}


//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteGroups
//
//
//---------------------------------------------------------------------------

void CObjReader::WriteGroups(CFileIO& fio)
{
	vector<SObjGroup*>::iterator igroup;
	for (igroup = mGroupVec.begin(); igroup != mGroupVec.end(); ++igroup)
	{
		char GroupName[32];
		memset(GroupName, 0, 32);
		strncpy(GroupName, (*igroup)->mName.c_str(), 31);

		fio.WriteBytes("fgrp", 4);
		fio.WriteBytes(GroupName, 32);
		cout << "Writing Group:" << GroupName << endl;


		int FaceCount = (*igroup)->mFaceVec.size();

		fio.WriteObject((*igroup)->mFlags);
		fio.WriteObject((*igroup)->mMatIndex);
		fio.WriteObject(FaceCount);

		vector<SFaceIndex*>::iterator iface;

		// Vertices schreiben

		for (iface = (*igroup)->mFaceVec.begin(); iface != (*igroup)->mFaceVec.end(); ++iface)
		{
			for (int f = 0; f < 3; f++)
			{

				int Index = (*iface)->mIndex[f].x;

				//cout << "f=" << f << " Index=" << Index << endl;

				fio.WriteObject((*mVertexVec)[Index]);
			}
		}

		// TexCoords schreiben

		if ((*igroup)->mFlags & OBJ_FLAG_TEXT)
		{
			for (iface = (*igroup)->mFaceVec.begin(); iface != (*igroup)->mFaceVec.end(); ++iface)
			{
				for (int f = 0; f < 3; f++)
				{
					int Index = (*iface)->mIndex[f].y;
					fio.WriteObject((*mTexCoordVec)[Index]);
				}
			}
		}

		// Normalen schreiben

		if ((*igroup)->mFlags & OBJ_FLAG_NORM)
		{
			for (iface = (*igroup)->mFaceVec.begin(); iface != (*igroup)->mFaceVec.end(); ++iface)
			{
				for (int f = 0; f < 3; f++)
				{
					int Index = (*iface)->mIndex[f].z;
					fio.WriteObject((*mNormalVec)[Index]);
				}
			}
		}
	}
}




