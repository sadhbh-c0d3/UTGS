#ifndef __INCLUDED_USELESS_DITHER_H__
#define __INCLUDED_USELESS_DITHER_H__

#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/Pixel/PixelPtr.h"
#include "Useless/Graphic/Pixel/PixelTypes.h"
#include <algorithm>

namespace Useless {

template< class PixelType >
NormalRGB PreserveColorKey( PixelType &, NormalRGB source, int r, int g, int b, int a )
{
    return PixelType( MakeRGBA( r, g, b, a)).Get().c;
}

template<>
NormalRGB PreserveColorKey( PixelType565 &, NormalRGB source, int r, int g, int b, int a )
{
    //Preserve color-keys
    r = (r<8 && source.r!=0)? 8 :(r>248 && source.r!=255)? 248 : (!source.r)? 0: (source.r==255)? 255: r;
    g = (g<8 && source.g!=0)? 8 :(g>248 && source.g!=255)? 248 : (!source.g)? 0: (source.g==255)? 255: g;
    b = (b<8 && source.b!=0)? 8 :(b>248 && source.b!=255)? 248 : (!source.b)? 0: (source.b==255)? 255: b;

return PixelType565( MakeRGB( r, g, b)).Get().c;}

template<>
NormalRGB PreserveColorKey( PixelType555 &, NormalRGB source, int r, int g, int b, int a )
{
    //Preserve color-keys
    r = (r<8 && source.r!=0)? 8 :(r>248 && source.r!=255)? 248 : (!source.r)? 0: (source.r==255)? 255: r;
    g = (g<8 && source.g!=0)? 8 :(g>248 && source.g!=255)? 248 : (!source.g)? 0: (source.g==255)? 255: g;
    b = (b<8 && source.b!=0)? 8 :(b>248 && source.b!=255)? 248 : (!source.b)? 0: (source.b==255)? 255: b;

    return PixelType555( MakeRGB( r, g, b)).Get().c;
}

template<>
NormalRGB PreserveColorKey( PixelType4444 &, NormalRGB source, int r, int g, int b, int a )
{
    //Preserve color-keys
    r = (r<16 && source.r!=0)? 16 :(r>240 && source.r!=255)? 240 : (!source.r)? 0: (source.r==255)? 255: r;
    g = (g<16 && source.g!=0)? 16 :(g>240 && source.g!=255)? 240 : (!source.g)? 0: (source.g==255)? 255: g;
    b = (b<16 && source.b!=0)? 16 :(b>240 && source.b!=255)? 240 : (!source.b)? 0: (source.b==255)? 255: b;
    a = (a<16 && source.a!=0)? 16 :(a>240 && source.a!=255)? 240 : (!source.a)? 0: (source.a==255)? 255: a;

    return PixelType4444( MakeRGBA( r, g, b, a)).Get().c;
}


template< class SrcPixelPtr, class DstPixelPtr >
void Dither( SrcPixelPtr s_ptr, DstPixelPtr d_ptr, int w, int h )
{
    int i,j;
    // allocate line error buffer
    MemBlock line( w * sizeof( PixelType8888 ) );
    PixelType8888 *line_error = (PixelType8888*)line.GetPtr();
    // setup line error to relative 0 which is 127 here
    for ( i=0; i!=w; ++i) 
    { 
        line_error[i] = MakeRGBA(127,127,127,127);
    }

    int r,g,b,a, dr=0,dg=0,db=0,da=0;
    // for each line
    for ( j=0; j!=h; ++j )
    {
        SrcPixelPtr in_ptr = s_ptr;
        DstPixelPtr out_ptr = d_ptr;
        // for each pixel in line
        for( i=0; i!=w; ++i )
        {
            NormalRGB pixel = (*in_ptr).Get().c;
            NormalRGB error = line_error[i].Get().c;
            // random diffuse
            int factor = rand();
            int factor2 = rand();
            int er = ((int)error.r-127);
            int eg = ((int)error.g-127);
            int eb = ((int)error.b-127);
            int ea = ((int)error.a-127);
            r = pixel.r + (factor * dr)/RAND_MAX + (factor2 * er)/RAND_MAX;
            g = pixel.g + (factor * dg)/RAND_MAX + (factor2 * eg)/RAND_MAX;
            b = pixel.b + (factor * db)/RAND_MAX + (factor2 * eb)/RAND_MAX;
            a = pixel.a + (factor * da)/RAND_MAX + (factor2 * ea)/RAND_MAX;
            r = Saturate(r);
            g = Saturate(g);
            b = Saturate(b);
            a = Saturate(a);
            // preserve values 0 and 255
            NormalRGB rgb_ = PreserveColorKey( *out_ptr, pixel, r, g, b, a );
            (*out_ptr) = rgb_;
            // calculate error
            dr = r - rgb_.r;
            dg = g - rgb_.g;
            db = b - rgb_.b;
            da = a - rgb_.a;
            line_error[i] = MakeRGBA( dr+127, dg+127, db+127, da+127);
            // move to next pixel
            ++in_ptr;
            ++out_ptr;
        }
        // move to next line
        s_ptr.Next();
        d_ptr.Next();
    }
}

};//namespace Useless
#endif//__INCLUDED_USELESS_DITHER_H__
