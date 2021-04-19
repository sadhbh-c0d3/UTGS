#include "UselessPch.h"

#include "Useless/Graphic/Transfer/MemOGfx.h"
#include "Useless/System/Assert.h"
#include <algorithm>

namespace Useless {

void MemOGfx::Write(const Types::UBYTE *src, int x, int y, int w, int h, int pitch)
{
    Types::UBYTE *dest = _output_memory + _pitch*y + _pxs*x;

    while (h--)
    {
	std::memcpy( dest, src, _pxs*w );
        dest += _pitch;
         src += pitch;
    }
}


void MemOGfx::SetSize( int w, int h, int bpp ) 
{
    C_assert( bpp % 8 == 0);
    _w=w; _h=h; _pxs = bpp/8;
}

void MemOGfx::WritePalette( const Types::UBYTE *rgb, int pal_size ) 
{
    if (_palette_output) { std::memcpy( _palette_output, rgb, pal_size*4); }
}

};//namespace Useless
