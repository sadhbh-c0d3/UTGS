/*
**  $Id: IGfxReader.cpp,v 1.7 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      OGfxBuffer
**
**  PURPOSE
**      Input graphics from buffer
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"
#include <assert.h>
#include <algorithm>

#include "Useless/Graphic/Transfer/IGfxReader.h"
namespace Useless {

void IGfxReader::GetSize( int *w, int *h, int *d )
{
    assert(w); assert(h); assert(d);
    *w=_width; *h=_height; *d=_bytes_per_pixel*8;
}

void IGfxReader::ReadPalette( const Types::UBYTE **rgb, int *pal_size )
{
    *rgb = _palette;
    *pal_size=_pal_size;
}

void IGfxReader::Read( Types::UBYTE *dest, int x, int y, int w, int h, int pitch )
{
    assert(w>0); assert(x+w <= _width); assert(h>0); assert(y+h <= _height); assert(dest);
    const Types::UBYTE *src = _data + (x + y*_width) * _bytes_per_pixel;
    int s_pitch = _width * _bytes_per_pixel; int cnt = w * _bytes_per_pixel;

    while(h--)
    {
	std::memcpy( dest, src, cnt );
        dest += pitch;
        src += s_pitch;
    }
}

}; // namespace Useless

