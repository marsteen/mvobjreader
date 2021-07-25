//------------------------------------------------------------------------------
//
// PROJECT : OpenGL
//
// FILE    : COpenGL.h
//
// VERSION : 1.2
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Deklaration der Klasse COpenGL
//
//------------------------------------------------------------------------------
//
//

#ifndef COPENGL_H
#define COPENGL_H

#include  <string>
#include <CVector3T.h>
#include <CGL_Mouse.h>
#include <CGL_Camera.h>

enum
{
	EDRAWMODE_ENVMAP     = 1,
	EDRAWMODE_ANAGLYPH   = 2,
	EDRAWMODE_STEREO     = 4,
	EDRAWMODE_INTERLACED = 8
};



class COpenGL
{
  public:

  	COpenGL();


		bool InitWindowGLUT(int argc, char** argv, const char* WindowName, int width, int height, bool Fullscreen);
    void Action();
    void MouseButtonAction();
    void MouseMotionAction();
    void KeyboardAction(unsigned char key);
    void StartModelView();
    void StartProjectionView();
		void SaveGLStrings(char* filename);
		void InitGLUTCallback(int fq);
		unsigned int* CreateSplitTextures(std::string& TextureFile, int XPatches, int YPatches);
		void   SetViewport(int w, int h);
		void   ViewportAction(int w, int h);
		void  DrawAnaglyphMode();
		int   DrawStereoMode();
		int   DrawInterlacedMode();
		void  TestSkipFrame();
		void  SetSkipFrame();
		void  ResetStereoMode();


    int WindowWidth()  { return mWidth; }
    int WindowHeight() { return mHeight; }



		int             mActionFrequency;
		CGL_Mouse       mMouse;
		CVector3<float> mLightPosition;
		CGL_Camera      mCamera;
		bool            mAnaglyph;
		int             mDrawmode;

  protected:

		int             mWindowHandle;
    int             mWidth;
    int             mHeight;
    float           mAspect;
    bool            mSkipFrame; // Fuer Stereo-Synchronisation
    unsigned long   mFrameCounter;


		void   CreateStencilBuffer();
    void   SetupLighting();
		void   SetPixelFormatDescriptorBitmap();


};

#endif
