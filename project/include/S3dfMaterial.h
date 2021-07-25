#ifndef S3dfMaterial_H
#define S3dfMaterial_H

struct S3dfMaterial
{
  /*
	void operator=(const S3dfMaterial& k)
	{
		memcpy(this, &k, sizeof(S3dfMaterial));
	}
	*/

	S3dfMaterial()
	{
		mDiffuse[0] = mAmbient[0] = mEmission[0] = mSpecular[0] = 0.0;
		mDiffuse[1] = mAmbient[1] = mEmission[1] = mSpecular[1] = 0.0;
		mDiffuse[2] = mAmbient[2] = mEmission[2] = mSpecular[2] = 0.0;
		mDiffuse[3] = mAmbient[3] = mEmission[3] = mSpecular[3] = 1.0;
		mTextureHandle = 0;
    mIndex = 0;
    mNs = 0;
    mTransparency = 0;
    mIllum = 0;
    
	}

	bool operator<(const S3dfMaterial& k) const
	{
		return mName < k.mName;
	}

	unsigned int  mIndex;
	std::string   mName;
	std::string   mTextureFile;	
	unsigned int  mTextureHandle;
	float mNs;             // Ns Specular Exponent
	float mTransparency;   // D Transparenz (1.0 = Opaque)
	float mIllum;          // illum Illumination
	float mDiffuse[4];     // Kd
	float mAmbient[4];     // Ka
	float mEmission[4];    // Ke
	float mSpecular[4];    // Ks
};

#endif
