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

#include <GLinclude.h>


#include <CGraflibTga.h>
#include <CGL_Texture.h>

using namespace std;

//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		DeleteTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_Texture::DeleteTexture()
{
    if (mTexHandle != 0)
    {
        glDeleteTextures(1, &mTexHandle);
        mTexHandle = 0;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		CreateTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_Texture::CreateTexture(int TexHandle, const CGraflib* glib, int WrapMode)
{
    int Format;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TexHandle);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    switch (glib->mBits)
    {
        case 8:
            Format = GL_ALPHA;
            break;

        case 24:
            Format = GL_RGB;
            break;

        case 32:
            Format = GL_RGBA;
            break;
    }

    //       GL_LINEAR
    // oder  GL_NEAREST

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapMode);    // GL_REPEAT oder GL_CLAMP_TO_EDGE); //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapMode);    // GL_REPEAT oder GL_CLAMP_TO_EDGE); //

    glTexImage2D(GL_TEXTURE_2D, 0, Format, glib->mWidth, glib->mHeight, 0, Format, GL_UNSIGNED_BYTE, glib->mData);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		CreateTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_Texture::CreateTexture(const CGraflib* glib)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &mTexHandle);              // Create One Texture
    CreateTexture(mTexHandle, glib, GL_REPEAT);

/*
 *
 *  glBindTexture(GL_TEXTURE_2D, (int) mTexHandle);
 *
 *      //       GL_LINEAR
 *      // oder  GL_NEAREST
 *
 *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 *
 *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE); //
 *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_CLAMP_TO_EDGE); //
 *
 *  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glib->mWidth, glib->mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, glib->mData);
 */
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		LoadTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CGL_Texture::LoadTextureTga(const char* TextureFilename)
{
    bool r = false;
    int Format;
    CGraflibTga tga;

    //cout << "CGL_Texture::LoadTextureTga START: " << TextureFilename << endl;

    tga.Read(TextureFilename);
    tga.SwapRedBlue();
    tga.Yflip();
    //tga.Write("test.tga", false);

    //cout << "CGL_Texture Width=" << tga.mWidth << " Height=" << tga.mHeight << endl;


    switch (tga.mBits)
    {
        case 8:
            Format = GL_ALPHA;
            break;

        case 24:
            Format = GL_RGB;
            break;

        case 32:
            Format = GL_RGBA;
            break;
    }

    if (tga.mData != NULL)  //
    {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &mTexHandle);              // Create One Texture
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, (int)mTexHandle);

        //       GL_LINEAR
        // oder  GL_NEAREST

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    //
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    //

        glTexImage2D(GL_TEXTURE_2D, 0, Format, tga.mWidth, tga.mHeight, 0, Format, GL_UNSIGNED_BYTE, tga.mData);

        mTexWidth = tga.mWidth;
        mTexHeight = tga.mHeight;
        mTexBitDepth = tga.mBits;

        delete[] tga.mData;

        glDisable(GL_TEXTURE_2D);

        r = true;
    }
    return r;                                   // Return The Status
}
