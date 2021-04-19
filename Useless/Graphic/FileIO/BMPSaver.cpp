#include "UselessPch.h"


/*
**  $Id: BMPSaver.cpp,v 1.9 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      BMPSaver
**
**  PURPOSE
**      Save BMP images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Error/Error.h"
#include "Useless/Graphic/FileIO/BMPSaver.h"
#include "Useless/Graphic/detail/BMPStructure.h"
#include "Useless/Util/Storage/MemBlock.h"

namespace Useless {

void BMPSaver::Save( OFile &file, IGfxFilter &filter )
{

    Types::BMPFileHeader FileHeader;
    Types::BMPInfoHeader InfoHeader;
    
    int depth, width, height, bytes, colors;
    MemBlock rgbbuffer;

    // get width, height and depth from filter
    filter.GetSize(&width,&height,&depth);
    const Types::UBYTE *palette;
    filter.ReadPalette( &palette, &colors);
    if (depth != 8) { colors=0; }

    bytes= depth/8;

    if (bytes==1 || bytes > 2) {

        // fill FileHeader & InfoHeader 
        FileHeader.bfType = 19778;
        FileHeader.bfSize = FileHeader.Size() + InfoHeader.Size() + colors*4 + width*height*bytes; 
        FileHeader.bfReserved1= 0;
        FileHeader.bfReserved2= 0;
        FileHeader.bfOffBits= FileHeader.Size() + InfoHeader.Size() + colors*4;

        InfoHeader.biSize= InfoHeader.Size(); 
        InfoHeader.biWidth= width;
        InfoHeader.biHeight= height;
        InfoHeader.biPlanes= 1;
        InfoHeader.biBitCount= depth;
        InfoHeader.biCompression= 0;
        InfoHeader.biSizeImage= 0; 
        InfoHeader.biXPelsPerMeter= 2834;
        InfoHeader.biYPelsPerMeter= 2834;
        InfoHeader.biClrUsed= colors;
        InfoHeader.biClrImportant= colors;

        //write FileHeader
        //file.Write(&FileHeader,sizeof(FileHeader));
        FileHeader.Save(file);

        //write InfoHeader
        //file.Write(&InfoHeader,sizeof(InfoHeader));
        InfoHeader.Save(file);

        //write palette if depth is 8 bit per pixel
        if (depth == 8) file.Write(palette,colors*4);

        //alocate memory for rgbbuffer
        rgbbuffer.Alloc( width * bytes );

        //read each line from filter
        for (int i= height-1 ; i>=0 ; i--) 
        {
            filter.Read(rgbbuffer.GetPtrUB(), 0, i, width, 1, 0);
            file.Write(rgbbuffer.GetPtrUB(), width * bytes);
        }
    }
    else throw Error("Unsuported bmp format.");

}

ImageSaver * BMPSaver::Create()
{
    return new BMPSaver;
}

}; // namespace Useless
