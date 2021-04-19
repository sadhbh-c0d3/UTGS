/*
**  $Id: TGASaver.cpp,v 1.9 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      TGASaver
**
**  PURPOSE
**      Save TGA images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"

#include "Useless/Error/Error.h"
#include "Useless/Graphic/FileIO/TGASaver.h"
#include "Useless/Graphic/detail/TGAStructure.h"
#include "Useless/Util/Storage/MemBlock.h"

namespace Useless {

void TGASaver::Save(OFile &file, IGfxFilter &filter )
{
    Types::TGAFileHeader header;
    int w,h,bpp;
    filter.GetSize( &w, &h, &bpp );
    header.image_width = w;
    header.image_height = h;
    header.image_pixel_size = bpp;


    if (header.image_pixel_size==24 || header.image_pixel_size==32 || header.image_pixel_size==8)
    {
        if (header.image_pixel_size==8)
        {
            int pal_size;
            const Types::UBYTE *colormap;
            filter.ReadPalette(&colormap, &pal_size);
            header.image_type = 1;
            header.color_map_entry_size = 32;
            header.color_map_length = pal_size;
            header.color_map_origin = 0;
            header.color_map_type = 1;
            header.image_descriptor = 0;
            header.image_descriptor = 8;
            header.Save( file );
            int colormap_size = header.color_map_length * (header.color_map_entry_size/8);
            file.Write(colormap, colormap_size);
        }
        else 
        { 
            if (header.image_pixel_size==32) { header.image_descriptor = 8; }
            header.image_type = 2; 
            header.Save( file );
        }
        

        MemBlock linebuf;
        int line_size = header.image_width*( header.image_pixel_size/8);
        linebuf.Alloc(line_size);

        for( int i=0; i!=header.image_height; ++i)
        {
            filter.Read( linebuf.GetPtrUB(), 0, header.image_height-i-1, header.image_width, 1, 0);
            file.Write(linebuf.GetPtrUB(), line_size);
        }
    }
    else { throw Error("Unsupported type"); }
}

ImageSaver * TGASaver::Create()
{
    return new TGASaver;
}

}; // namespace Useless
