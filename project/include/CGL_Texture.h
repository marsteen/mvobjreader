#ifndef CGL_TEXTURE_H
#define CGL_TEXTURE_H

#include <CGraflib.h>

class CGL_Texture
{
    public:

        CGL_Texture()
        {
            mTexHandle = 0;
        }


        bool LoadTextureTga(const char* TextureFilename);
        void CreateTexture(const CGraflib* glib);
        static void CreateTexture(int TexHandle, const CGraflib* glib, int WrapMode);
        void DeleteTexture();


        unsigned int mTexHandle;
        int mTexWidth;
        int mTexHeight;
        int mTexBitDepth;
};


#endif
