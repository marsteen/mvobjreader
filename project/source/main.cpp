#include <CFileIO.h>
#include <NStringTool.h>
#include "CObjReader.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        //CObjReader ObjReader;
        CObjReader* ObjReader = new CObjReader;

        cout << "File=" << argv[1] << endl;

        string MaterialObj = NStringTool::ReplaceExtension(argv[1], ".mtl");
        string Filename3df = NStringTool::ReplaceExtension(argv[1], ".img3df");
        string FilenameJS = NStringTool::ReplaceExtension(argv[1], ".js");

        ObjReader->ReadMtl(MaterialObj.c_str());


        ObjReader->ReadObj(argv[1]);
        cout << "Read ok" << endl;



        //ObjReader->Write3df(Filename3df.c_str());
        ObjReader->WriteJS(FilenameJS);

        cout << "Outfile:" << Filename3df << endl;
    }
    else
    {
        cout << "usage: objreader <file.obj>" << endl;
        cout << "version 1.1" << endl;
    }


    return 0;
}
