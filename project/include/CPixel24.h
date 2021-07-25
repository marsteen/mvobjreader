
#ifndef byte
typedef unsigned short word;
typedef unsigned char byte;
#endif

class CPixel24
{
	public:

  	byte rgb[3];

    void operator=(CPixel24& p2)
    {
			rgb[0] = p2.rgb[0];
			rgb[1] = p2.rgb[1];
			rgb[2] = p2.rgb[2];
		}


    void operator=(int i)
    {
			rgb[0] = i >> 16;
			rgb[1] = (i >> 8) & 0xFF;
			rgb[2] = i & 0xFF;
		}

		unsigned int GetInt()
		{
			return (rgb[2] << 16) | (rgb[1] << 8) | rgb[0];
		}


    bool operator==(CPixel24& p2);
    bool operator!=(CPixel24& p2);
		void Set(word w1, byte m1);
		void Set(byte r, byte g, byte b);
		void Set(unsigned int b);
		void SwapRedBlue(void);
		int  Brightness(void);
};


