#ifndef __INCLUDED__USELESS_PIXEL_SHADERS_H__
#define __INCLUDED__USELESS_PIXEL_SHADERS_H__

#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {

/* Some simple shaders for use with ScanLine or wherever
-----------------------------------------------------------------------*/


template< class TPixel > struct ColorPixelShader
{
    typedef TPixel PixelType;
    ColorPixelShader( NormalRGB pixel_color ): pixel_color(pixel_color) {}
    void SetScanLine( int X0, int X1, int Y ) {}
    PixelType GetColor( PixelType *ptr, int x) { return pixel_color; }

    PixelType pixel_color;
};


template< class TPixel> struct GradientPixelShader
{
    typedef TPixel PixelType;
    GradientPixelShader( NormalRGB pixel_color1, NormalRGB pixel_color2 ): pixel_color1(pixel_color1), pixel_color2(pixel_color2) {}
    void SetScanLine( int x0, int x1, int y ) { X0=x0; X1=x1; }
    PixelType GetColor( PixelType *pPixel, int ix) 
    {
        int W = X1-X0;
        ix = (ix<0)? 0 : (ix<W)? ix : W-1;
        int bl = (256*ix)/(W>0? W : 1);
        return pixel_color2.Get().c.Blend( bl, pixel_color1.Get().c ); 
    }

    PixelType pixel_color1;
    PixelType pixel_color2;
    int X0,X1,Y;
};


template< class TPixel > struct EdgeGlowPixelShader
{
    typedef TPixel PixelType;
    EdgeGlowPixelShader( NormalRGB pixel_color ): pixel_color(pixel_color) {}
    void SetScanLine( int x0, int x1, int y ) { X0=x0; X1=x1; }
    PixelType GetColor( PixelType *pPixel, int ix) 
    {
        int W = X1-X0;
        ix = (ix<0)? 0 : (ix<W)? ix : W-1;
        
        int bl = (256*ix)/(W>0? W : 1);

        return pixel_color.Get().c.Blend( bl, pPixel->Get().c ); 
    }

    PixelType pixel_color;
    int X0,X1,Y;
};


template< class TPixel > struct DimmPixelShader
{
    typedef TPixel PixelType;
    DimmPixelShader( int dimm ): _dimm(dimm) {}
    void SetScanLine( int x0, int x1, int y ) {}
    PixelType GetColor( PixelType  *pPixel, int ix) 
    {
        NormalRGB c = pPixel->Get().c;
        c.r = (((int)c.r) * _dimm)/256;
        c.g = (((int)c.g) * _dimm)/256;
        c.b = (((int)c.b) * _dimm)/256;
        return c;
    }

    int _dimm;
};

template< class TPixel > struct Dimm50PixelShader
{
    typedef TPixel PixelType;
    void SetScanLine( int x0, int x1, int y ) {}
    PixelType GetColor( PixelType *pPixel, int ix) 
    {
        unsigned int p = *((unsigned int*&)pPixel);
        (p &= PixelType::MASK1) >>= 1;
        return *(PixelType*)&p;
    }
};


template< class TPixel > struct Dimm75PixelShader
{
    typedef TPixel PixelType;
    void SetScanLine( int x0, int x1, int y ) {}
    PixelType GetColor( PixelType *pPixel, int ix) 
    {
        unsigned int p = *((unsigned int*&)pPixel);
        unsigned int q = p;
        (p &= PixelType::MASK1) >>= 1;
        (q &= PixelType::MASK2) >>= 2;
        return *(PixelType*)&(p+=q);
    }
};


template< class TPixel > struct Brighten50PixelShader
{
    typedef TPixel PixelType;
    void SetScanLine( int x0, int x1, int y ) {}
    PixelType GetColor( PixelType *pPixel, int ix) 
    {
        unsigned int p = *((unsigned int*&)pPixel);
        unsigned int q = p;
        (p &= PixelType::MASK1) >>= 1;
        return *(PixelType*)&(p+=q);
    }
};

template< class TPixel > struct NoPixelShader
{
    typedef TPixel PixelType;
    void SetScanLine( int x0, int x1, int y ) {}
    PixelType GetColor( PixelType *pPixel, int ix) 
    {
        unsigned int p = *((unsigned int*&)pPixel);
        return 0L;
    }
};


};//namespace Useless

#endif//__INCLUDED__USELESS_PIXEL_SHADERS_H__
