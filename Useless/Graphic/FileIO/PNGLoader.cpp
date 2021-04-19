/*
 *  NAME:
 *      PNGLoader
 *
 *  PURPOSE:
 *      Load PNG images (using libPNG)
 *
 *  AUTHOR:
 *      Sadhbh Code (https://github.com/sadhbh-c0d3) 
 *
 */

#include "UselessPch.h"

#ifndef USELESS_HAS_LIBPNG
#   error  "USELESS_HAS_LIBPNG is not defined. Please, exclude appropriate files from build."
#endif


#include "Useless/Error/Error.h"
#include "Useless/Graphic/FileIO/PNGLoader.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Rect.h"
#include "Useless/Graphic/detail/ImageDesc.h"


// include libPNG | couldn't use "libpng.lib" - there were errors while program startup
#include <png.h>
#pragma comment(lib,"libpng.a")
// include ZLib
#pragma comment(lib,"zdll.lib")

namespace {

    void user_read_data( png_structp png_ptr, png_bytep data, png_size_t length )
    {
        voidp read_io_ptr = png_get_io_ptr( png_ptr );
        static_cast< Useless::IFile * >( read_io_ptr )->Read( data, length );
    }
};

namespace Useless {
    
    ImageLoader * PNGLoader::Create()
    {
        return new PNGLoader;
    }

    void PNGLoader::Load( IFile &file, OGfxFilter &filter )
    {
        throw Error("NOT IMPLEMENTED: "
                "void PNGLoader::Load( IFile &file, OGfxFilter &filter )");
    }

    bool PNGLoader::Recognize( IFile &file, int *w,int *h,int *bpp )
    {
        png_byte header[8];
        png_uint_32 number = 8;
        file.Read( header, number );
        bool is_png = !png_sig_cmp( header, 0, number );

        if ( !is_png )
        {
            return false;
        }
        
        png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

        if ( !png_ptr )
        {
            throw Error("Cannot create png_struct.");
        }

        png_infop info_ptr = png_create_info_struct( png_ptr );

        if ( !info_ptr )
        {
            throw Error("Cannot create png_info.");
        }

        png_infop end_info = png_create_info_struct( png_ptr );

        if ( !end_info )
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
            throw Error("Cannot create end_info.");
        }

        if ( setjmp( png_jmpbuf( png_ptr )))
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, &end_info);
            throw Error("PNG read error.");
        }
    
        png_set_read_fn( png_ptr, &file, &user_read_data );

        png_set_sig_bytes( png_ptr, number );

        png_read_info( png_ptr, info_ptr );

        png_uint_32 width, height;
        int bit_depth, color_type, interlace_type, compression_type, filter_type, channels;
        png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_type );
        channels = png_get_channels( png_ptr, info_ptr );

        bool is_supported = (PNG_COLOR_TYPE_GRAY == color_type) || (PNG_COLOR_TYPE_RGB == color_type) || (PNG_COLOR_TYPE_RGB_ALPHA == color_type);

        if ( !is_supported )
        {
            throw Error("PNGLoader: Unsupported color_type.");
        }
        
        int pixel_format = ImageFormat::INVALID;

        if ( PNG_COLOR_TYPE_GRAY == color_type )
        {
            if ( 8 != bit_depth || 1 != channels )
            {
                throw Error("PNGLoader: PNG_COLOR_TYPE_GRAY expects 1 channel in 8 bit color");
            }
            pixel_format = ImageFormat::SINGLE8;
        }
        else if ( PNG_COLOR_TYPE_RGB == color_type )
        {
            if ( 8 != bit_depth || 3 != channels )
            {
                throw Error("PNGLoader: PNG_COLOR_TYPE_RGB expects 3 channels in 8 bit color");
            }
            pixel_format = ImageFormat::R8G8B8;
        }
        else if ( PNG_COLOR_TYPE_RGB_ALPHA == color_type )
        {
            if ( 8 != bit_depth || 4 != channels )
            {
                throw Error("PNGLoader: PNG_COLOR_TYPE_RGB_ALPHA expects 4 channels in 8 bit color");
            }
            pixel_format = ImageFormat::R8G8B8A8;
        }
        else
        {
            throw Error("PNGLoader expects color_type: PNG_COLOR_TYPE_{GRAY/RGB/RGB_ALPHA}.");
        }


        if ( 0 != w )   { *w = width; }
        if ( 0 != h )   { *h = height; }
        if ( 0 != bpp ) { *bpp = pixel_format; }

        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info);
        return is_supported;
    }

    void PNGLoader::Load( IFile &file, PixelTransfer &transfer )
    {
        png_byte header[8];
        png_uint_32 number = 8;
        file.Read( header, number );
        bool is_png = !png_sig_cmp( header, 0, number );

        if ( !is_png )
        {
            throw Error("Not PNG file: \"%s\".", file.GetName() );
        }
        
        png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

        if ( !png_ptr )
        {
            throw Error("Cannot create png_struct.");
        }

        png_infop info_ptr = png_create_info_struct( png_ptr );

        if ( !info_ptr )
        {
            throw Error("Cannot create png_info.");
        }

        png_infop end_info = png_create_info_struct( png_ptr );

        if ( !end_info )
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
            throw Error("Cannot create end_info.");
        }

        if ( setjmp( png_jmpbuf( png_ptr )))
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, &end_info);
            throw Error("PNG read error.");
        }
    
        png_set_read_fn( png_ptr, &file, &user_read_data );

        png_set_sig_bytes( png_ptr, number );

        png_read_info( png_ptr, info_ptr );

        png_uint_32 width, height;
        int bit_depth, color_type, interlace_type, compression_type, filter_type, channels;
        png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_type );
        channels = png_get_channels( png_ptr, info_ptr );

        bool is_supported = (PNG_COLOR_TYPE_GRAY == color_type) || (PNG_COLOR_TYPE_RGB == color_type) || (PNG_COLOR_TYPE_RGB_ALPHA == color_type);

        if ( !is_supported )
        {
            throw Error("PNGLoader: Unsupported color_type.");
        }
    
        png_uint_32 row_bytes = png_get_rowbytes( png_ptr, info_ptr );

        png_bytep png_image_data = new png_byte[ row_bytes * height ];

        png_bytepp png_rows = new png_bytep[ height ];
        png_bytep byte_ptr = &png_image_data[0];

        for ( png_uint_32 n=0; n<height; ++n )
        {
            png_rows[n] = byte_ptr;
            byte_ptr += row_bytes;
        }

        png_read_image( png_ptr, png_rows );

        int pixel_format = ImageFormat::INVALID;

        if ( PNG_COLOR_TYPE_GRAY == color_type )
        {
            if ( 8 != bit_depth || 1 != channels )
            {
                throw Error("PNGLoader: PNG_COLOR_TYPE_GRAY expects 1 channel in 8 bit color");
            }
            pixel_format = ImageFormat::SINGLE8;
        }
        else if ( PNG_COLOR_TYPE_RGB == color_type )
        {
            if ( 8 != bit_depth || 3 != channels )
            {
                throw Error("PNGLoader: PNG_COLOR_TYPE_RGB expects 3 channels in 8 bit color");
            }
            pixel_format = ImageFormat::R8G8B8;
        }
        else if ( PNG_COLOR_TYPE_RGB_ALPHA == color_type )
        {
            if ( 8 != bit_depth || 4 != channels )
            {
                throw Error("PNGLoader: PNG_COLOR_TYPE_RGB_ALPHA expects 4 channels in 8 bit color");
            }
            pixel_format = ImageFormat::R8G8B8A8;
        }
        else
        {
            throw Error("PNGLoader expects color_type: PNG_COLOR_TYPE_{GRAY/RGB/RGB_ALPHA}.");
        }

        transfer.SetSource( width, height, row_bytes, pixel_format, 0, 0 );
        transfer.Fetch( png_image_data, Rect( width, height ));

        delete [] png_rows;
        delete [] png_image_data;

        png_read_end( png_ptr, end_info );

        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info);
    }

};
