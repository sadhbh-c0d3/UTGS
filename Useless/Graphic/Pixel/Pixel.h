#ifndef __INCLUDED__USELESS_PIXEL_H__
#define __INCLUDED__USELESS_PIXEL_H__

#include "Useless/Graphic/detail/ImageDesc.h" //< For RT pixel information

#include "Useless/Graphic/Rect.h"
#include "Useless/System/Types.h"

namespace Useless {

inline unsigned char Saturate( int x ) { return (x<0)? 0 : (x<255)? x : 255; }

/*  8bit per pixel normal color representation
--------------------------------------------------*/
#define BGR_COLOR_32 //< DDSurface has often color format B8 G8 R8 A8


/*! Unified for calculations color type
--------------------------------------------------*/
struct NormalRGB
{
#ifdef BGR_COLOR_32
    unsigned char b, g, r, a;
#else
    unsigned char r, g, b, a;
#endif

    NormalRGB Blend( int alpha, NormalRGB color );
    NormalRGB Blend( NormalRGB color );
    int Similarity( NormalRGB color ) const;
};


/*! Simple union of RGB struct and RGB inside unsigned int
---------------------------------------------------------*/
union NormalPixel
{
    NormalPixel(): u(0) {}
    NormalPixel( unsigned long x): u(x) {}
    NormalPixel( const NormalRGB &x): c(x) {}

    NormalRGB       c;
    unsigned long   u;
};


/*! RGB struct constructor
---------------------------------------------------------------------------*/
inline NormalRGB MakeRGB( unsigned char r, unsigned char g, unsigned char b) 
{ 
    NormalRGB val; 
    val.r=r; val.g=g; val.b=b; val.a=255;
    return val; 
}

/*! RGB struct constructor with alpha
---------------------------------------------------------------------------*/
inline NormalRGB MakeRGBA( unsigned char r, unsigned char g, unsigned char b, unsigned char a ) 
{ 
    NormalRGB val; 
    val.r=r; val.g=g; val.b=b; val.a=a;
    return val; 
}

/*! RGB alpha blend
--------------------------------------------------------------*/
inline NormalRGB NormalRGB::Blend( int alpha, NormalRGB color )
{
    return MakeRGB (
                    ( (alpha)*(int)r + (256-alpha)*(int)color.r )/256,
                    ( (alpha)*(int)g + (256-alpha)*(int)color.g )/256,
                    ( (alpha)*(int)b + (256-alpha)*(int)color.b )/256 );
}

/*! RGBA blend
--------------------------------------------------------------*/
inline NormalRGB NormalRGB::Blend( NormalRGB color )
{
    return MakeRGB (
                    ( (int)a*(int)r + (256-(int)a)*(int)color.r )/256,
                    ( (int)a*(int)g + (256-(int)a)*(int)color.g )/256,
                    ( (int)a*(int)b + (256-(int)a)*(int)color.b )/256 );
}


/*! Calculate similarity
--------------------------------------------------------------*/
inline int NormalRGB::Similarity( NormalRGB color ) const
{
    int dr = (int)r - (int)color.r;
    int dg = (int)g - (int)color.g;
    int db = (int)b - (int)color.b;
    return dr*dr + dg*dg + db*db;
}


};//namespace Useless 

#endif//__INCLUDED__USELESS_PIXEL_H__
