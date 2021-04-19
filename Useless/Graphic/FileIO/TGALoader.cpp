/*
**  $Id: TGALoader.cpp,v 1.12 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      TGALoader
**
**  PURPOSE
**      Load TGA images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**      Janusz Gregorczyk (proxi@iavision.com)
//      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/Error/Error.h"
#include "Useless/Graphic/FileIO/TGALoader.h"
#include "Useless/Graphic/detail/TGAStructure.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Rect.h"


namespace Useless {


enum { DEFAULT_BUF_SIZE = 1024*1024 };


void TGALoader::Load( IFile &file, OGfxFilter &filter )
{
    Types::TGAFileHeader header;
    header.Load( file );

    if( (header.image_type==2)&&(header.image_pixel_size==16 || header.image_pixel_size==24 || header.image_pixel_size==32) || 
        (header.image_type==1 || header.image_type==3)&&(header.image_pixel_size==8) )
    {
        MemBlock id_field;
        if ( header.num_id_chars > 0 )  
        { 
            id_field.Alloc( header.num_id_chars );
            file.Read( id_field.GetPtrC(), header.num_id_chars );
        }
        
        filter.SetSize(header.image_width, header.image_height, header.image_pixel_size);

        const int line_size = header.image_width*( header.image_pixel_size/8);
        int loadbuf_lines = DEFAULT_BUF_SIZE / line_size;
        if(loadbuf_lines<1) { loadbuf_lines=1; }
        MemBlock linesbuf( line_size*loadbuf_lines );

        if( header.color_map_type )
        {
            int colormap_size = header.color_map_length * 4;
            MemBlock colormap( colormap_size );
            
            if ( header.color_map_entry_size == 32 )
            {
                file.Read( colormap.GetPtrUB(), colormap_size );
            }
            else if ( header.color_map_entry_size == 24 )
            {
                file.Read( colormap.GetPtrUB(), header.color_map_length * 3 );
                Types::UBYTE *colors = colormap.GetPtrUB();
                
                for( int i=header.color_map_length-1; i>=0; --i)
                {
                    colors[4*i+3] = 0;
                    colors[4*i+2] = colors[3*i+2];
                    colors[4*i+1] = colors[3*i+1];
                    colors[4*i+0] = colors[3*i+0];
                }
            }
            else { throw Error("TGALoader does not support this format: wrong palette bpp"); }
            filter.WritePalette( colormap.GetPtrUB(), header.color_map_length );
        }

        for( int i=0; i < header.image_height; i+=loadbuf_lines )
        {
            int cur_load_lines = header.image_height-i;
            if(cur_load_lines>loadbuf_lines) { cur_load_lines=loadbuf_lines; }
            file.Read( linesbuf.GetPtrUB(), line_size*cur_load_lines );

            //20011206: proxi: 555->565 conversion.
            if (header.image_pixel_size==16)
            {
                Types::UWORD *buf = linesbuf.GetPtrUW();
                
                for( int i=0; i<line_size*cur_load_lines / 2; i++)
                    *buf++ = (*buf & 31)
                           | ((*buf & (31 << 5)) << 1)
                           | ((*buf & (31 << 10)) << 1);
            }
            //20011206: proxi:

            //int fill_bytes = (4 - (line_size%4))%4;
            //while(fill_bytes--) { try { file.ReadByte();} catch(...){} }

            const Types::UBYTE *last_line_ptr = linesbuf.GetPtrUB( line_size*(cur_load_lines-1) );
            filter.Write( last_line_ptr, 0, header.image_height-i-cur_load_lines, header.image_width, cur_load_lines, -line_size );
        }
    }
    else { throw Error("TGALoader does not support this format: wrong image bpp"); }
}

bool TGALoader::Recognize( IFile &file, int *w,int *h,int *bpp )
{
    Types::TGAFileHeader header;
    header.Load( file );

    bool is_ok = (header.image_type==2)&&(header.image_pixel_size==16 || header.image_pixel_size==24 || header.image_pixel_size==32) || 
           (header.image_type==1)&&(header.image_pixel_size==8)&&(header.color_map_type==1) ||
           (header.image_type==3)&&(header.image_pixel_size==8);
    if( is_ok && w && h && bpp)
    {
        *w = header.image_width;
        *h = header.image_height;
        *bpp = header.image_pixel_size;
    }
    return is_ok;
}

ImageLoader * TGALoader::Create()
{
    return new TGALoader;
}
//------------------------------------------------------------------------------
void TGALoader::Load( IFile &file, PixelTransfer &transfer )
{
    Types::TGAFileHeader header;
    header.Load( file );

    if( (header.image_type==2)&&(header.image_pixel_size==16 || header.image_pixel_size==24 || header.image_pixel_size==32) || 
        (header.image_type==1 || header.image_type==3)&&(header.image_pixel_size==8) )
    {
        MemBlock id_field;
        if ( header.num_id_chars > 0 )  
        { 
            id_field.Alloc( header.num_id_chars );
            file.Read( id_field.GetPtrC(), header.num_id_chars );
        }
        

        const int line_size = header.image_width*( header.image_pixel_size/8);
        int loadbuf_lines = DEFAULT_BUF_SIZE / line_size;
        if(loadbuf_lines<1) { loadbuf_lines=1; }
        MemBlock linesbuf( line_size*loadbuf_lines );

        if ( header.color_map_type )
        {
            int colormap_size = header.color_map_length * 4;
            MemBlock colormap( colormap_size );
            
            if ( header.color_map_entry_size == 32 )
            {
                file.Read( colormap.GetPtrUB(), colormap_size );
            }
            else if ( header.color_map_entry_size == 24 )
            {
                file.Read( colormap.GetPtrUB(), header.color_map_length * 3 );
                Types::UBYTE *colors = colormap.GetPtrUB();
                
                for( int i=header.color_map_length-1; i>=0; --i)
                {
                    colors[4*i+3] = 0;
                    colors[4*i+2] = colors[3*i+2];
                    colors[4*i+1] = colors[3*i+1];
                    colors[4*i+0] = colors[3*i+0];
                }
            }
            else { throw Error("TGALoader does not support this format: wrong palette bpp");
            }
            transfer.SetSource( header.image_width, header.image_height,
                -line_size, header.image_pixel_size, colormap.GetPtrUB(), header.color_map_length );
        }
        else
        {
            transfer.SetSource( header.image_width, header.image_height,
                -line_size, header.image_pixel_size );
        }

        for( int i=0; i < header.image_height; i+=loadbuf_lines )
        {
            int cur_load_lines = header.image_height-i;
            if(cur_load_lines>loadbuf_lines) { cur_load_lines=loadbuf_lines; }
            file.Read( linesbuf.GetPtrUB(), line_size*cur_load_lines );

            //20011206: proxi: 555->565 conversion.
            if (header.image_pixel_size==16)
            {
                Types::UWORD *buf = linesbuf.GetPtrUW();
                
                for( int i=0; i<line_size*cur_load_lines / 2; i++)
                    *buf++ = (*buf & 31)
                           | ((*buf & (31 << 5)) << 1)
                           | ((*buf & (31 << 10)) << 1);
            }
            //20011206: proxi:

            //int fill_bytes = (4 - (line_size%4))%4;
            //while(fill_bytes--) { try { file.ReadByte();} catch(...){} }

            const Types::UBYTE *last_line_ptr = linesbuf.GetPtrUB( line_size*(cur_load_lines-1) );
            transfer.Fetch( last_line_ptr,
                Rect( 0, header.image_height-i-cur_load_lines, header.image_width, cur_load_lines)
                );
        }
    }
    else { throw Error("TGALoader does not support this format: wrong image bpp"); }
}

}; // namespace Useless
