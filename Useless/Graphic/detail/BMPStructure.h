#ifndef __INCLUDED_USELESSIMP_BMPSTRUCTURE_H__
#define __INCLUDED_USELESSIMP_BMPSTRUCTURE_H__

/*
**  $Id: BMPStructure.h,v 1.4 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      BMPStructure
**
**  PURPOSE
**      Structure of BMP file
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"

namespace Useless {
namespace Types {

class BMPFileHeader
{
public:
        Types::UWORD    bfType;
        Types::ULONG    bfSize;
        Types::UWORD    bfReserved1;
        Types::UWORD    bfReserved2;
        Types::ULONG    bfOffBits;
        void Load( IFile &f )
        {
            bfType = f.ReadWordLE();
            bfSize = f.ReadLongLE();
            bfReserved1 = f.ReadWordLE();
            bfReserved2 = f.ReadWordLE();
            bfOffBits = f.ReadLongLE();
        }

        void Save( OFile &f )
        {
            f.WriteWordLE( bfType );
            f.WriteLongLE( bfSize );
            f.WriteWordLE( bfReserved1 );
            f.WriteWordLE( bfReserved2 );
            f.WriteLongLE( bfOffBits );
        }
        int Size() { return 14; }
};


class BMPInfoHeader
{
public:
        Types::ULONG    biSize;
        Types::LONG     biWidth;
        Types::LONG     biHeight;
        Types::UWORD    biPlanes;
        Types::UWORD    biBitCount;
        Types::ULONG    biCompression;
        Types::ULONG    biSizeImage;
        Types::LONG     biXPelsPerMeter;
        Types::LONG     biYPelsPerMeter;
        Types::ULONG    biClrUsed;
        Types::ULONG    biClrImportant;
        void Load( IFile &f ) 
        {
            biSize = f.ReadLongLE();
            biWidth = f.ReadLongLE();
            biHeight = f.ReadLongLE();
            biPlanes = f.ReadWordLE();
            biBitCount = f.ReadWordLE();
            biCompression = f.ReadLongLE();
            biSizeImage = f.ReadLongLE();
            biXPelsPerMeter = f.ReadLongLE();
            biYPelsPerMeter = f.ReadLongLE();
            biClrUsed = f.ReadLongLE();
            biClrImportant = f.ReadLongLE();
        }
        void Save( OFile &f ) 
        {
            f.WriteLongLE(biSize);
            f.WriteLongLE(biWidth);
            f.WriteLongLE(biHeight);
            f.WriteWordLE(biPlanes);
            f.WriteWordLE(biBitCount);
            f.WriteLongLE(biCompression);
            f.WriteLongLE(biSizeImage);
            f.WriteLongLE(biXPelsPerMeter);
            f.WriteLongLE(biYPelsPerMeter);
            f.WriteLongLE(biClrUsed);
            f.WriteLongLE(biClrImportant);
        }
        int Size() { return 40; }
};

};// namespace Types ends
};// namespace Useless ends

#endif //__INCLUDED_USELESSIMP_BMPSTRUCTURE_H__
