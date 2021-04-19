/*
**  $Id: BMPLoader.cpp,v 1.13 2004/06/23 12:12:35 koolas Exp $
**
**  NAME
**      BMPLoader
**
**  PURPOSE
**      Load BMP images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"

#include "Useless/Error/Error.h"
#include "Useless/Graphic/FileIO/BMPLoader.h"
#include "Useless/Graphic/detail/BMPStructure.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Rect.h"

namespace Useless {

void BMPLoader :: Load ( IFile &file, OGfxFilter &filter )
{
    Types::BMPFileHeader FileHeader;
    Types::BMPInfoHeader InfoHeader;
    
    int j, depth, width, height, bytes, colors;
    MemBlock rgbbuffer,palette;
  
    FileHeader.Load(file);
    InfoHeader.Load(file);

    if (FileHeader.bfType != 19778) throw Error("This file is not BMP file");

    // set  width ,height, and depth from file
    width = InfoHeader.biWidth;
    height = InfoHeader.biHeight;
    depth = InfoHeader.biBitCount;
    colors = InfoHeader.biClrImportant;

    bytes = depth/8;
   
    if ( ((depth == 8) || (depth > 16)) && !InfoHeader.biCompression ) {

        //allocate memory for one line
        rgbbuffer.Alloc(width * bytes);

        //set size 
        filter.SetSize(width, height, depth);

        // write palette 
        if (bytes == 1) {
            //allocate memory for palette
            palette.Alloc(bytes * colors * 4 + 1);
            file.Read(palette.GetPtrUB(), 4*colors);
            filter.WritePalette(palette.GetPtrUB(), colors);
        }

        size_t seek = FileHeader.bfOffBits - file.GetPos();
        file.Seek( seek );
    
        int align_bytes = depth==8 ? 0 : (4 - ((width * bytes)%4))%4;

        // loop reading line by line
        for (j=0; j < height; j++)
        {
            //load one line into buffer
            file.Read( rgbbuffer.GetPtrUB(), width * bytes);
            filter.Write(rgbbuffer.GetPtrUB(), 0, height-j-1, width, 1, 0);

            int k=align_bytes; while (k--)
            {
                try { file.ReadByte(); }
                catch(...){}
            }
        }
    }
    else throw Error("Unsuported file format.");
            
}

bool BMPLoader::Recognize( IFile &file, int *w,int *h,int *bpp )
{

    Types::BMPFileHeader FileHeader;
    Types::BMPInfoHeader InfoHeader;
    
    FileHeader.Load(file);
    InfoHeader.Load(file);

    bool is_ok = (FileHeader.bfType == 19778)&&
    ( (InfoHeader.biBitCount==8)||((InfoHeader.biBitCount==24||InfoHeader.biBitCount==32)&&(!InfoHeader.biClrImportant)) );
    if( is_ok && w && h && bpp)
    {
        *w = InfoHeader.biWidth;
        *h = InfoHeader.biHeight;
        *bpp = InfoHeader.biBitCount;
    }
    return is_ok;
}

ImageLoader * BMPLoader::Create()
{
    return new BMPLoader;
}

//------------------------------------
void BMPLoader::Load( IFile &file, PixelTransfer &transfer )
{
    Types::BMPFileHeader FileHeader;
    Types::BMPInfoHeader InfoHeader;
    
    int j, depth, width, height, bytes, colors;
    MemBlock rgbbuffer,palette;
  
    FileHeader.Load(file);
    InfoHeader.Load(file);

    if (FileHeader.bfType != 19778) throw Error("This file is not BMP file");

    // set  width ,height, and depth from file
    width = InfoHeader.biWidth;
    height = InfoHeader.biHeight;
    depth = InfoHeader.biBitCount;
    colors = InfoHeader.biClrImportant;

    bytes = depth/8;
   
    if ( ((depth == 8) || (depth > 16)) && !InfoHeader.biCompression ) {

        // allocate memory for one line
        rgbbuffer.Alloc(width * bytes);

        // set source parameters 
        if ( bytes == 1 ) // use palette
        {
            //allocate memory for palette
            palette.Alloc(bytes * colors * 4 + 1);
            file.Read( palette.GetPtrUB(), 4*colors);
            transfer.SetSource( width, height, width * bytes, depth, palette.GetPtr(), colors );
        }
        else // dons't use palatte
        {
            transfer.SetSource( width, height, width * bytes, depth );
        }
    
        size_t seek = FileHeader.bfOffBits - file.GetPos();
        file.Seek( seek );

        int align_bytes = depth==8 ? 0 : (4 - ((width * bytes)%4))%4;

        // loop reading line by line
        for (j=0; j < height; j++)
        {
            //load one line into buffer
            file.Read( rgbbuffer.GetPtrUB(), width * bytes);
            transfer.Fetch( rgbbuffer.GetPtrUB(), Rect( 0, height-j-1, width, 1));

            int k=align_bytes; while (k--)
            {
                try { file.ReadByte(); }
                catch(...){}
            }
        }
    }
    else throw Error("Unsuported file format.");           
}

}; // namespace Useless
