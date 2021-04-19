#ifndef __INCLUDED_USELESS_PIXEL_TYPES_H__
#define __INCLUDED_USELESS_PIXEL_TYPES_H__

#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {

/*------------------------------------------------------------------------------------------*
 * PIXEL TYPES                                                                              *
-*------------------------------------------------------------------------------------------*/


/*! Red:5 Green:6 Blue:5
 -----------------------------------------*/
struct PixelType565 
{ 
    PixelType565(): pixel(0) {}
    PixelType565( unsigned short pixel): pixel(pixel) {}
    PixelType565( unsigned long pixel): pixel((unsigned short)pixel) {}
    PixelType565( int pixel): pixel((unsigned short)pixel) {}
    PixelType565( unsigned char r, unsigned char g, unsigned char b ) { pixel = (r>>3)<<11 | (g>>2)<<5 | (b>>3); }
    PixelType565( NormalRGB p )                                {  pixel = (p.r>>3)<<11 | (p.g>>2)<<5 | (p.b>>3); }

    NormalPixel Get() 
    {   
        NormalPixel p; unsigned long px = pixel;
        p.c.a = 255;
        p.c.r = (unsigned char)((px&0xF800)>>11<<3);
        p.c.g = (unsigned char)((px&0x07E0)>>5<<2);
        p.c.b = (unsigned char)((px&0x001F)<<3);
        return p;
    }


    bool operator ==( const PixelType565 p) { return pixel==p.pixel; }
    operator unsigned short() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::R5G6B5; }

    enum { MASK1=0xF7DE, MASK2=0xE79C, MASK3=0xC718 }; 
    unsigned short pixel;
};

/*! Alpha:1 Red:5 Green:5 Blue:5
 -----------------------------------------*/
struct PixelType555
{ 
    PixelType555(): pixel(0) {}
    PixelType555( unsigned short pixel): pixel(pixel) {}
    PixelType555( unsigned long pixel): pixel((unsigned short)pixel) {}
    PixelType555( int pixel): pixel((unsigned short)pixel) {}
    PixelType555( unsigned char r, unsigned char g, unsigned char b ) { pixel = (r>>3)<<10 | (g>>3)<<5 | (b>>3); }
    PixelType555( NormalRGB p )                 {  pixel = (p.a>>7)<<15 | (p.r>>3)<<10 | (p.g>>3)<<5 | (p.b>>3); }

    NormalPixel Get() 
    {   
        NormalPixel p; unsigned long px = pixel;
        p.c.a = (unsigned char)((px&0x8000)>>15<<7);
        p.c.r = (unsigned char)((px&0x7C00)>>10<<3);
        p.c.g = (unsigned char)((px&0x03E0)>>5<<3);
        p.c.b = (unsigned char)((px&0x001F)<<3);
        return p;
    }


    bool operator ==( const PixelType555 p) { return pixel==p.pixel; }
    operator unsigned short() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::R5G5B5; }

    enum { MASK1=0x7BDE, MASK2=0x739C, MASK3=0x6318 }; 
    unsigned short pixel;
};

/*! Alpha:4 Red:4 Green:4 Blue:4
 -----------------------------------------*/
struct PixelType4444
{ 
    PixelType4444(): pixel(0) {}
    PixelType4444( unsigned short pixel): pixel(pixel) {}
    PixelType4444( unsigned long pixel): pixel((unsigned short)pixel) {}
    PixelType4444( int pixel): pixel((unsigned short)pixel) {}
    PixelType4444( unsigned char r, unsigned char g, unsigned char b ) { pixel = (r>>4)<<8 | (g>>4)<<4 | (b>>4); }
    PixelType4444( NormalRGB p )                 {  pixel = (p.a>>4)<<12 | (p.r>>4)<<8 | (p.g>>4)<<4 | (p.b>>4); }

    NormalPixel Get() 
    {   
        NormalPixel p; unsigned long px = pixel;
        p.c.a = (unsigned char)((px&0xF000)>>12<<4);
        p.c.r = (unsigned char)((px&0x0F00)>>8<<4);
        p.c.g = (unsigned char)((px&0x00F0)>>4<<4);
        p.c.b = (unsigned char)((px&0x000F)<<4);
        return p;
    }


    bool operator ==( const PixelType4444 p) { return pixel==p.pixel; }
    operator unsigned short() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::A4R4G4B4; }

    enum { MASK1=0xEEE, MASK2=0xCCC, MASK3=0x888 }; 
    unsigned short pixel;
};


/*! Alpha:8 Red:8 Green:8 Blue:8
-----------------------------------------*/
struct PixelType8888
{ 
    PixelType8888(): pixel(0) {}
    PixelType8888( int pixel): pixel(pixel) {}
    PixelType8888( unsigned long pixel): pixel(pixel) {}
    PixelType8888( long pixel): pixel((unsigned long)pixel) {}
    PixelType8888( unsigned char r, unsigned char g, unsigned char b )  { pixel = r<<16 | g<<8 | b; }
    PixelType8888( NormalRGB p ) { pixel = p.a<<24 | p.r<<16 | p.g<<8 | p.b; }

    NormalPixel Get() 
    {   
        NormalPixel p;

#ifdef BGR_COLOR_32
        p.u = pixel;
#else
        p.c.a = (unsigned char)((pixel&0xFF000000)>>24);
        p.c.b = (unsigned char)((pixel&0x00FF0000)>>16);
        p.c.g = (unsigned char)((pixel&0x0000FF00)>>8);
        p.c.r = (unsigned char) (pixel&0x000000FF);
#endif
        return p;
    }
    bool operator ==( const PixelType8888 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::B8G8R8A8; }

    enum { MASK1=0xFEFEFE, MASK2=0xFCFCFC, MASK3=0xF8F8F8 }; 
    unsigned long pixel;
};

/*! Alpha:0 Red:8 Green:8 Blue:8
-----------------------------------------*/
struct PixelType0888
{ 
    PixelType0888(): pixel(0) {}
    PixelType0888( int pixel): pixel(pixel) {}
    PixelType0888( unsigned long pixel): pixel(pixel) {}
    PixelType0888( long pixel): pixel((unsigned long)pixel) {}
    PixelType0888( unsigned char r, unsigned char g, unsigned char b )  { pixel = 255<<24 | r<<16 | g<<8 | b; }
    PixelType0888( NormalRGB p ) { pixel = 255<<24 | p.r<<16 | p.g<<8 | p.b; }

    NormalPixel Get() 
    {   
        NormalPixel p;

        p.c.a = 255;
        p.c.r = (unsigned char)((pixel&0xFF0000)>>16);
        p.c.g = (unsigned char)((pixel&0x00FF00)>>8);
        p.c.b = (unsigned char) (pixel&0x0000FF);
        
        return p;
    }

    PixelType0888& operator=( const PixelType0888 &p )
    {
        pixel = (p.pixel & 0x00FFFFFF) | (/*pixel &*/ 0xFF000000);
        return *this;
    }
    
    bool operator ==( const PixelType8888 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::B8G8R8A0; }

    enum { MASK1=0xFEFEFE, MASK2=0xFCFCFC, MASK3=0xF8F8F8 }; 
    unsigned long pixel;
};


/*! Red:8 Green:8 Blue:8
-----------------------------------------*/
struct PixelType888
{ 
    PixelType888() { bytes[0]=0; bytes[1]=0; bytes[2]=0; }
    PixelType888( unsigned long pixel) { ( (*(unsigned long*)bytes) &= 0xff000000 )|= (pixel&0x00ffffff); }
    PixelType888( unsigned char r, unsigned char g, unsigned char b )  { bytes[2]=r; bytes[1]=g; bytes[0]=b; }
    PixelType888( NormalRGB p ) { bytes[2]=p.r; bytes[1]=p.g; bytes[0]=p.b; }

    NormalPixel Get() 
    {   
        NormalPixel p;

#ifdef BGR_COLOR_32
        p.u = (*(unsigned long*)bytes)&0x00ffffff | 0xff000000;
#else
        p.c.a = 0xff;
        p.c.b = bytes[2];
        p.c.g = bytes[1];
        p.c.r = bytes[0];
#endif
        return p;
    }
    bool operator ==( const PixelType888 p) { return ( bytes[0]==p.bytes[0] )&&( bytes[1]==p.bytes[1] )&&( bytes[2]==p.bytes[2] ); }
    operator unsigned long() { return *(unsigned long*)bytes; }
    static ImageFormat::Format Fmt() { return ImageFormat::B8G8R8; }

    enum { MASK1=0xFEFEFE, MASK2=0xFCFCFC, MASK3=0xF8F8F8 }; 

    unsigned char bytes[3];
};

/*! Red:8 Green:8 Blue:8 (swapped R<=>B)
-----------------------------------------*/
struct PixelType888s
{ 
    PixelType888s() { bytes[0]=0; bytes[1]=0; bytes[2]=0; }
    PixelType888s( unsigned long pixel) { ( (*(unsigned long*)bytes) &= 0xff000000 )|= (pixel&0x00ffffff); }
    PixelType888s( unsigned char r, unsigned char g, unsigned char b )  { bytes[2]=b; bytes[1]=g; bytes[0]=r; }
    PixelType888s( NormalRGB p ) { bytes[2]=p.b; bytes[1]=p.g; bytes[0]=p.r; }

    NormalPixel Get() 
    {   
        NormalPixel p;

#ifndef BGR_COLOR_32
        p.u = (*(unsigned long*)bytes)&0x00ffffff | 0xff000000;
#else
        p.c.a = 0xff;
        p.c.b = bytes[2];
        p.c.g = bytes[1];
        p.c.r = bytes[0];
#endif
        return p;
    }
    bool operator ==( const PixelType888s p) { return ( bytes[0]==p.bytes[0] )&&( bytes[1]==p.bytes[1] )&&( bytes[2]==p.bytes[2] ); }
    operator unsigned long() { return *(unsigned long*)bytes; }
    static ImageFormat::Format Fmt() { return ImageFormat::R8G8B8; }

    enum { MASK1=0xFEFEFE, MASK2=0xFCFCFC, MASK3=0xF8F8F8 }; 

    unsigned char bytes[3];
};


/*! Alpha:8 Red:8 Green:8 Blue:8
-----------------------------------------*/
struct PixelType8888s
{ 
    PixelType8888s(): pixel(0) {}
    PixelType8888s( int pixel): pixel(pixel) {}
    PixelType8888s( unsigned long pixel): pixel(pixel) {}
    PixelType8888s( long pixel): pixel((unsigned long)pixel) {}
    PixelType8888s( unsigned char r, unsigned char g, unsigned char b )  { pixel = b<<16 | g<<8 | r; }
    PixelType8888s( NormalRGB p ) { pixel = p.a<<24 | p.b<<16 | p.g<<8 | p.r; }

    NormalPixel Get() 
    {   
        NormalPixel p;

#ifndef BGR_COLOR_32
        p.u = pixel;
#else
        p.c.a = (unsigned char)((pixel&0xFF000000)>>24);
        p.c.b = (unsigned char)((pixel&0x00FF0000)>>16);
        p.c.g = (unsigned char)((pixel&0x0000FF00)>>8);
        p.c.r = (unsigned char) (pixel&0x000000FF);
#endif
        return p;
    }
    bool operator ==( const PixelType8888s p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::R8G8B8A8; }

    enum { MASK1=0xFEFEFE, MASK2=0xFCFCFC, MASK3=0xF8F8F8 }; 
    unsigned long pixel;
};

/*! Alpha:4 Red:4 Green:4 Blue:4
 -----------------------------------------*/
struct PixelType4444s
{ 
    PixelType4444s(): pixel(0) {}
    PixelType4444s( unsigned short pixel): pixel(pixel) {}
    PixelType4444s( unsigned long pixel): pixel((unsigned short)pixel) {}
    PixelType4444s( int pixel): pixel((unsigned short)pixel) {}
    PixelType4444s( unsigned char r, unsigned char g, unsigned char b ) { pixel = (b>>4)<<8 | (g>>4)<<4 | (r>>4); }
    PixelType4444s( NormalRGB p )                 {  pixel = (p.a>>4)<<12 | (p.b>>4)<<8 | (p.g>>4)<<4 | (p.r>>4); }

    NormalPixel Get() 
    {   
        NormalPixel p; unsigned long px = pixel;
        p.c.a = (unsigned char)((px&0xF000)>>12<<4);
        p.c.b = (unsigned char)((px&0x0F00)>>8<<4);
        p.c.g = (unsigned char)((px&0x00F0)>>4<<4);
        p.c.r = (unsigned char)((px&0x000F)<<4);
        return p;
    }

    bool operator ==( const PixelType4444s p) { return pixel==p.pixel; }
    operator unsigned short() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::A4B4G4R4; }

    enum { MASK1=0xEEE, MASK2=0xCCC, MASK3=0x888 }; 
    unsigned short pixel;
};


/*! Alpha:8 - separates alpha channel
-----------------------------------------*/
struct PixelType8000
{ 
    PixelType8000(): pixel(0) {}
    PixelType8000( NormalRGB p ) { pixel = p.a << 24; }
    NormalPixel Get() { return pixel; }

    /* Such operation
     * (PixelType8000&)(mem) = MakeRGBA( red, green, blue, alpha)
     * writes into mem alpha without changing other channels
     */
    PixelType8000& operator=( const PixelType8000 &p )
    {
        pixel = (pixel & 0x00FFFFFF) | (p.pixel & 0xFF000000);
        return *this;
    }

    bool operator ==( const PixelType8000 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::ALPHA8; }

    unsigned long pixel;
};

/*! Alpha:4 - separates alpha channel
-----------------------------------------*/
struct PixelType4000
{ 
    PixelType4000(): pixel(0) {}
    PixelType4000( NormalRGB p ) { pixel = (p.a>>4)<<12; }
    NormalPixel Get() { return pixel; }

    PixelType4000& operator=( const PixelType4000 &p )
    {
        pixel = (pixel & 0x0FFF) | (p.pixel & 0xF000);
        return *this;
    }

    bool operator ==( const PixelType4000 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::ALPHA4; }

    unsigned short pixel;
};

/*! Alpha:1 - separates alpha channel
-----------------------------------------*/
struct PixelType1000
{ 
    PixelType1000(): pixel(0) {}
    PixelType1000( NormalRGB p ) { pixel = (p.a>>7)<<15; }
    NormalPixel Get() { return pixel; }

    PixelType1000& operator=( const PixelType1000 &p )
    {
        pixel = (pixel & 0x7FFF) | (p.pixel & 0x8000);
        return *this;
    }

    bool operator ==( const PixelType1000 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::ALPHA4; }

    unsigned short pixel;
};

/*! Single channel:8
-----------------------------------------*/
struct PixelType8
{ 
    PixelType8(): pixel(0) {}
    PixelType8( int pixel): pixel(pixel) {}
    PixelType8( NormalRGB p )
    {
		if ( p.a < 0x10 )
		{
			pixel = p.a;
		}
		else if ( 0xFF == p.r && 0xFF == p.g && 0xFF == p.b )
		{
			pixel = p.a;
		}
		else
		{
			pixel = ((2 * ((long)p.r+1) + ((long)p.g+1) + ((long)p.b+1)) * ((long)p.a+1)) /1024;
		}
    }

    NormalPixel Get() 
    {   
        NormalPixel p;
        p.c.a = pixel;
        p.c.r = pixel;
        p.c.g = pixel;
        p.c.b = pixel;
        return p;
    }
    bool operator ==( const PixelType8 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::SINGLE8; }

    enum { MASK1=0xFE, MASK2=0xFC, MASK3=0xF8 }; 
    unsigned char pixel;
};

/*! Alpha:8 - premultiplies alpha channel
-----------------------------------------*/
struct PixelType8XXX
{ 
    PixelType8XXX(): pixel(0) {}
    PixelType8XXX( NormalRGB p ) { pixel = p.a << 24; }
    NormalPixel Get() { return pixel; }

    /* Such operation
     * (PixelType8XXX&)(mem) = MakeRGBA( red, green, blue, alpha)
     * writes into mem alpha premultiplying by other channels
     */
    PixelType8XXX& operator=( const PixelType8XXX &p )
    {
        int alpha= (p.pixel & 0xFF000000)>>24;
        int ch_0 = (pixel   & 0x00FF0000)>>16;
        int ch_1 = (pixel   & 0x0000FF00)>>8;
        int ch_2 = (pixel   & 0x000000FF);
        (ch_0 *= alpha) /= 255;
        (ch_1 *= alpha) /= 255;
        (ch_2 *= alpha) /= 255;

        pixel = alpha<<24 | ch_0<<16 | ch_1<<8 | ch_2;
        return *this;
    }

    bool operator ==( const PixelType8000 p) { return pixel==p.pixel; }
    operator unsigned long() { return pixel; }
    static ImageFormat::Format Fmt() { return ImageFormat::ALPHA8; }

    unsigned long pixel;
};


/*------------------------------------------------------------------------------------------*
 * PIXEL TRAITS                                                                             *
-*------------------------------------------------------------------------------------------*/

template< int >
struct PixelTraits {};

template<>
struct PixelTraits< ImageFormat::B8G8R8A0 >
{
    typedef PixelType0888 PixelType;
};

template<>
struct PixelTraits< ImageFormat::B8G8R8A8 >
{
    typedef PixelType8888 PixelType;
};

template<>
struct PixelTraits< ImageFormat::B8G8R8 >
{
    typedef PixelType888 PixelType;
};

template<>
struct PixelTraits< ImageFormat::R5G6B5 >
{
    typedef PixelType565 PixelType;
};

//template<> -- same as ImageFormat::A1R5G5B5
//struct PixelTraits< ImageFormat::R5G5B5 >
//{
//    typedef PixelType555 PixelType;
//};

template<> 
struct PixelTraits< ImageFormat::A1R5G5B5 >
{
    typedef PixelType555 PixelType;
};

template<>
struct PixelTraits< ImageFormat::R8G8B8A8 >
{
    typedef PixelType8888s PixelType;
};

template<>
struct PixelTraits< ImageFormat::R8G8B8 >
{
    typedef PixelType888s PixelType;
};

template<>
struct PixelTraits< ImageFormat::A4R4G4B4 >
{
    typedef PixelType4444 PixelType;
};

template<>
struct PixelTraits< ImageFormat::A4B4G4R4 >
{
    typedef PixelType4444s PixelType;
};

template<>
struct PixelTraits< ImageFormat::ALPHA8 >
{
    typedef PixelType8000 PixelType;
};

template<>
struct PixelTraits< ImageFormat::ALPHA4 >
{
    typedef PixelType4000 PixelType;
};

template<>
struct PixelTraits< ImageFormat::ALPHA1 >
{
    typedef PixelType1000 PixelType;
};

template<>
struct PixelTraits< ImageFormat::SINGLE8 >
{
    typedef PixelType8 PixelType;
};

template<>
struct PixelTraits< ImageFormat::ALPHA8PM >
{
    typedef PixelType8XXX PixelType;
};


#define __PIXEL_SWITCH_CASE( format, command, name ) \
    case format: { \
        PixelTraits<format>::PixelType name; \
        command; \
    } break;

#define __PIXEL_SWITCH( format, command, name )\
    switch( format )\
    {\
        __PIXEL_SWITCH_CASE(ImageFormat::B8G8R8A8,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::B8G8R8  ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::R5G6B5  ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::A1R5G5B5,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::R8G8B8A8,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::R8G8B8  ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::A4R4G4B4,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::A4B4G4R4,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::ALPHA8  ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::ALPHA8PM,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::ALPHA4  ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::ALPHA1  ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::SINGLE8 ,command,name)\
        __PIXEL_SWITCH_CASE(ImageFormat::B8G8R8A0,command,name)\
    }

};//namespace Useless
#endif//__INCLUDED_USELESS_PIXEL_TYPES_H__
