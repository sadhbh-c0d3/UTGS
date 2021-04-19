#ifndef __INCLUDED_USELESS_COPY_PIXELS_H__
#define __INCLUDED_USELESS_COPY_PIXELS_H__

#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/Pixel/PixelPtr.h"

namespace Useless {

template< class SrcPixelPtr, class DstPixelPtr >
void CopyPixels( SrcPixelPtr s_ptr, DstPixelPtr d_ptr, int _w, int _h )
{
    int h =_h;
    for ( ;h-- ; s_ptr.Next(), d_ptr.Next() )
    {
        SrcPixelPtr i_ptr = s_ptr;
        DstPixelPtr o_ptr = d_ptr;
        int w =_w;
        for ( ;w-- ; ++o_ptr, ++i_ptr )
        {
            (*o_ptr) = (*i_ptr).Get().c;
        }
    }
}

};//namespace Useless
#endif//__INCLUDED_USELESS_COPY_PIXELS_H__
