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


template <typename T>
static void WriteVals(ofstream& outf, const T* f, int n)
{
  outf << "  ";
	for (int i = 0; i < n; i++)
	{
		outf << f[i] << ",";
	}
	outf << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   WriteVal
//
//
//---------------------------------------------------------------------------


template <typename T>
static void WriteVal(ofstream& outf, const T v)
{
	outf << "  " << v << "," << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   WriteStringJS
//
//
//---------------------------------------------------------------------------


template <typename T>
static void WriteStringJS(ostream& outf, const T str)
{
  outf << "  " << '"' << str << '"' << ',' << endl;
}


template <typename T>
static void WriteCommentJS(ostream& outf, const T str)
{
  outf << "  // " << str << endl;
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteGroupsJS
//
//
//---------------------------------------------------------------------------



void CObjReader::WriteGroupsJS(ofstream& outf)
{
	vector<SObjGroup*>::iterator igroup;
	for (igroup = mGroupVec.begin(); igroup != mGroupVec.end(); ++igroup)
	{
		WriteStringJS(outf, "GRP");
		WriteStringJS(outf, (*igroup)->mName);
		WriteStringJS(outf, (*igroup)->mMatName);

		WriteVal(outf, (*igroup)->mFlags);            // Flags
		WriteVal(outf, (*igroup)->mFaceVec.size());   // FaceCount

		vector<SFaceIndex*>::iterator iface;

		//
		// Vertices schreiben
		//


		WriteCommentJS(outf, "Vertices");

		for (iface = (*igroup)->mFaceVec.begin(); iface != (*igroup)->mFaceVec.end(); ++iface)
		{
			for (int f = 0; f < 3; f++)
			{

				int Index = (*iface)->mIndex[f].x;

				//cout << "f=" << f << " Index=" << Index << endl;

				WriteVals(outf, (*mVertexVec)[Index].v(), 3);
			}
		}




    //
		// Texturkoordinaten schreiben
		//

		if ((*igroup)->mFlags & OBJ_FLAG_TEXT)
		{
		  WriteCommentJS(outf, "Texcoords");

			for (iface = (*igroup)->mFaceVec.begin(); iface != (*igroup)->mFaceVec.end(); ++iface)
			{
				for (int f = 0; f < 3; f++)
				{
					int Index = (*iface)->mIndex[f].y;

					WriteVals(outf, (*mTexCoordVec)[Index].v(), 2);
				}
			}
		}

    //
		// Normalen schreiben
		//

		if ((*igroup)->mFlags & OBJ_FLAG_NORM)
		{
		  WriteCommentJS(outf, "Normals");

			for (iface = (*igroup)->mFaceVec.begin(); iface != (*igroup)->mFaceVec.end(); ++iface)
			{
				for (int f = 0; f < 3; f++)
				{
					int Index = (*iface)->mIndex[f].z;
					WriteVals(outf, (*mNormalVec)[Index].v(), 3);
				}
			}
		}

	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteMaterialsJS
//
//
//---------------------------------------------------------------------------

void CObjReader::WriteMaterialsJS(ofstream& outf)
{
	vector<S3dfMaterial> MaterialVec;
	vector<S3dfMaterial>::iterator imat;
	SortMaterialSetIndex(MaterialVec);
  
  cout << "MaterialVec.size()=" << MaterialVec.size() << endl;
  int i = 0;

	for (imat = MaterialVec.begin(); imat != MaterialVec.end(); ++imat)
	{

		string TextureFile = "";
		if (imat->mTextureFile.size() > 0)
		{
			TextureFile = mObjNameJS + '/' + imat->mTextureFile;
		}
		
		cout << "imat->mName=" << imat->mName << endl;


		WriteStringJS(outf, "MAT");
		WriteStringJS(outf, imat->mName);
		WriteStringJS(outf, TextureFile);
		WriteVal(outf, imat->mNs);           // Specular Exponent
		WriteVal(outf,  imat->mTransparency); // D Transparenz (1.0 = Opaque)
		WriteVal(outf,  imat->mIllum);        // Illumination
		WriteVals(outf, imat->mDiffuse, 3);
		WriteVals(outf, imat->mAmbient, 3);
		WriteVals(outf, imat->mEmission, 3);
		WriteVals(outf, imat->mSpecular, 3);
    
    cout << i++ << " ok" << endl;
	}
}


//---------------------------------------------------------------------------
//
// Klasse:    CObjReader
// Methode:   WriteJS
//
// Schreibe JavaScript
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
//    int32   Groesse der Texturdaten
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




void  CObjReader::WriteJS(const std::string& Filename)
{
	ofstream outf(Filename.c_str());

	if (outf.good())
	{



		{
		  stringvector sp;
      NStringTool::Split(Filename, &sp, '/');
      if (sp.size() >= 1)
      {
        mObjNameJS = NStringTool::Cut(sp[sp.size() -1 ], '.');
      }
    }


		outf << "var " << mObjNameJS << " =" << endl;;
		outf << "[" << endl;

		WriteStringJS(outf, "IMA3D");

  	WriteCommentJS(outf, "Number of Materials:");
		WriteVal(outf,  mMaterialSet.size());

		WriteCommentJS(outf, "Number of Groups:");
		WriteVal(outf,  mGroupVec.size() );
    
    cout << "Write Materials:" << endl;

		//
		// Materialien schreiben
		//

		WriteCommentJS(outf, "");
		WriteCommentJS(outf, "Materials:");
		WriteCommentJS(outf, "");
    cout << "001" << endl;
    
		WriteMaterialsJS(outf);
    //outf.close();
    //return;

    cout << "Write Groups:" << endl;
		//
		// Gruppen schreiben
		//

		WriteCommentJS(outf, "");
		WriteCommentJS(outf, "Groups:");
		WriteCommentJS(outf, "");

		WriteGroupsJS(outf);

		outf << "];" << endl;

		outf.close();
	}
}
