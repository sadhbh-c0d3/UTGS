#include "UselessPch.h"

#include "Useless/Graphic/detail/ImageDesc.h"

namespace Useless {

ImageFormat::Format ImageFormat::Recognize( unsigned long maskR, unsigned long maskG, unsigned long maskB, unsigned long bitcount)
{
    switch( bitcount )
    {
    case 32:    
        if( maskR==0x0000FF && maskG==0x00FF00 && maskB==0xFF0000 ) return ImageFormat::R8G8B8A8; 
        else if( maskR==0xFF0000 && maskG==0x00FF00 && maskB==0x0000FF ) return ImageFormat::B8G8R8A8;
        
    case 24:    
        if( maskR==0x0000FF && maskG==0x00FF00 && maskB==0xFF0000 ) return ImageFormat::R8G8B8; 
        else if( maskR==0xFF0000 && maskG==0x00FF00 && maskB==0x0000FF ) return ImageFormat::B8G8R8;

    case 16:    
        if( maskR==0xF800 && maskG==0x07E0 && maskB==0x001F ) return ImageFormat::R5G6B5; 
        else if( maskR==0x7C00 && maskG==0x3E0 && maskB==0x001F ) return ImageFormat::R5G5B5;
        else if( maskR==0x0F00 && maskG==0x00F0 && maskB==0x000F ) return ImageFormat::A4R4G4B4;
    case 8:
        return ImageFormat::INDEX8;    
    }
    return ImageFormat::INVALID;
}

int ImageFormat::GetBpp( ImageFormat::Format fmt )
{
    switch( fmt )
    {
    default:
        return (fmt+7)/8;
    }
}


unsigned long ImageFormat::Encode( Format fmt, unsigned int color )
{
    return Encode(fmt, color>>16, color>>8, color);
}


unsigned long ImageFormat::Encode( Format fmt,
                                  unsigned char r,
                                  unsigned char g,
                                  unsigned char b)
{
    switch( fmt )
    {
    case R8G8B8A8:
    case R8G8B8:
        return r | g<<8 | b<<16;

    case B8G8R8A8:
    case B8G8R8:
        return r<<16 | g<<8 | b;

    case R5G6B5:
        return (r>>3)<<11 | (g>>2)<<5 | (b>>3);

    case R5G5B5:
        return (r>>3)<<10 | (g>>3)<<5 | (b>>3);

     case A4R4G4B4:
        return (r>>4)<<12 | (g>>4)<<8 | (b>>4);
   }
    return 0L;
}

unsigned long ImageFormat::Decode( ImageFormat::Format fmt, unsigned long color_in )
{
    union
    {
        struct { unsigned char b,g,r,reserved; } c;
        unsigned long u;
    } cu; cu.u=0L;

    Decode( fmt, color_in, &cu.c.r, &cu.c.g, &cu.c.b );
    return cu.u;
}

void ImageFormat::Decode( ImageFormat::Format fmt, unsigned long color,
                          unsigned char *r,
                          unsigned char *g,
                          unsigned char *b)
{
    switch( fmt )
    {
    case R8G8B8A8:
    case R8G8B8:
        *r = (unsigned char)(color&0x0000FF);
        *g = (unsigned char)((color&0x00FF00)>>8);
        *b = (unsigned char)((color&0xFF0000)>>16);
        break;

    case B8G8R8A8:
    case B8G8R8:
        *r = (unsigned char)((color&0xFF0000)>>16);
        *g = (unsigned char)((color&0x00FF00)>>8);
        *b = (unsigned char)(color&0x0000FF);
        break;

    case R5G6B5:
        *r = (unsigned char)((color&0xF800)>>11<<3);
        *g = (unsigned char)((color&0x07E0)>>5<<2);
        *b = (unsigned char)((color&0x001F)<<3);
        break;

    case R5G5B5:
        *r = (unsigned char)((color&0x7C00)>>10<<3);
        *g = (unsigned char)((color&0x3E0)>>5<<3);
        *b = (unsigned char)((color&0x001F)<<3);
        break;

    case A4R4G4B4:
        *r = (unsigned char)((color&0x0F00)>>12<<4);
        *g = (unsigned char)((color&0x00F0)>>8<<4);
        *b = (unsigned char)((color&0x000F)<<4);
        break;

    }
}

void ImageFormat::GetColorMask( ImageFormat::Format fmt, unsigned long *r, unsigned long *g, unsigned long *b )
{
    switch( fmt )
    {
    case R8G8B8A8:
    case R8G8B8:
        *r = 0x0000FF;
        *g = 0x00FF00;
        *b = 0xFF0000;
        break;

    case B8G8R8A8:
    case B8G8R8:
        *r = 0xFF0000;
        *g = 0x00FF00;
        *b = 0x0000FF;
        break;

    case R5G6B5:
        *r = 0xF800;
        *g = 0x07E0;
        *b = 0x001F;
        break;

    case R5G5B5:
        *r = 0x7C00;
        *g = 0x3E0;
        *b = 0x001F;
        break;

    case A4R4G4B4:
        *r = 0x0F00;
        *g = 0x00F0;
        *b = 0x000F;
        break;
    }
}

void ImageFormat::GetBitCount( ImageFormat::Format fmt, int *r, int *g, int *b )
{
    switch( fmt )
    {
    case R8G8B8A8:
    case R8G8B8:
        *r = 8;
        *g = 8;
        *b = 8;
        break;

    case B8G8R8A8:
    case B8G8R8:
        *r = 8;
        *g = 8;
        *b = 8;
        break;

    case R5G6B5:
        *r = 5;
        *g = 6;
        *b = 5;
        break;

    case R5G5B5:
        *r = 5;
        *g = 5;
        *b = 5;
        break;

    case A4R4G4B4:
        *r = 4;
        *g = 4;
        *b = 4;
        break;
    }
}


};// namespace Useless
