/*
**  $Id: OGfxBuffer.cpp,v 1.9 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      OGfxBuffer
**
**  PURPOSE
**      Output graphics buffer
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"
#include <assert.h>

#include "Useless/Graphic/Transfer/OGfxBuffer.h"
#include <algorithm>

namespace Useless {

void OGfxBuffer::SetSize( int w, int h, int d )
{
    assert(w>=0); assert(h>=0); assert(d==8 || d==24 || d==32 || d==16);
    _width = w; _height = h; _bytes_per_pixel = d/8;
  
    int size = _width * _height * _bytes_per_pixel;
    _data.Alloc( size + (4-size%4) );
}

void OGfxBuffer::WritePalette( const Types::UBYTE *rgb, int pal_size )
{
    assert( pal_size >= 0);
    _palette.Alloc( pal_size*(32/8) );
    if( pal_size ) std::memcpy( _palette.GetPtr(), rgb, pal_size*(32/8) );
}

void OGfxBuffer::Write( const Types::UBYTE *src, int x, int y, int w, int h, int pitch )
{
    assert(w>0); assert(x+w <= _width); assert(h>0); assert(y+h <= _height); assert(src);
    Types::UBYTE *dst = _data.GetPtrUB() + (x + y*_width) * _bytes_per_pixel;
    int d_pitch = _width * _bytes_per_pixel; int cnt = w * _bytes_per_pixel;

    while(h--)
    {
	std::memcpy( dst, src, cnt );
        src += pitch;
        dst += d_pitch;
    }
}

}; // namespace Useless

