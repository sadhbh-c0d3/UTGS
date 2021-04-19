#ifndef __INCLUDED_USELESSIMP_BMPSTRUCTURE_H__
#define __INCLUDED_USELESSIMP_BMPSTRUCTURE_H__

/*
**  $Id: TGAStructure.h,v 1.4 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      TGAStructure
**
**  PURPOSE
**      Structure of TGA file
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"

namespace Useless {
namespace Types {

class TGAFileHeader
{
public:
    UBYTE num_id_chars;
    UBYTE color_map_type;
    UBYTE image_type;
    UWORD color_map_origin;
    UWORD color_map_length;
    UBYTE color_map_entry_size;
    UWORD x_origin;
    UWORD y_origin;
    UWORD image_width;
    UWORD image_height;
    UBYTE image_pixel_size;
    UBYTE image_descriptor;

    TGAFileHeader() : num_id_chars(0), color_map_type(0), image_type(2),
        color_map_origin(0), color_map_length(0), color_map_entry_size(0),
        x_origin(0), y_origin(0), image_width(0), image_height(0), image_pixel_size(24),
        image_descriptor(0) {}

    void Load( IFile &f )
    {
        num_id_chars = f.ReadByte();
        color_map_type = f.ReadByte();
        image_type = f.ReadByte();
        color_map_origin = f.ReadWordLE();
        color_map_length = f.ReadWordLE();
        color_map_entry_size = f.ReadByte();
        x_origin = f.ReadWordLE();
        y_origin = f.ReadWordLE();
        image_width = f.ReadWordLE();
        image_height = f.ReadWordLE();
        image_pixel_size = f.ReadByte();
        image_descriptor = f.ReadByte();
    }

    void Save( OFile &f )
    {
        f.WriteByte( num_id_chars );
        f.WriteByte( color_map_type );
        f.WriteByte( image_type );
        f.WriteWordLE( color_map_origin );
        f.WriteWordLE( color_map_length );
        f.WriteByte( color_map_entry_size );
        f.WriteWordLE( x_origin );
        f.WriteWordLE( y_origin );
        f.WriteWordLE( image_width );
        f.WriteWordLE( image_height );
        f.WriteByte( image_pixel_size );
        f.WriteByte( image_descriptor );
    }

    int Size() { return 18; }
};

}; // namespace Types
}; // namespace Useless

#endif __INCLUDED_USELESSIMP_BMPSTRUCTURE_H__
