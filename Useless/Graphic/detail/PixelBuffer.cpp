#include "UselessPch.h"

#include "Useless/Graphic/detail/PixelBuffer.h"
#include "Useless/Graphic/detail/ImageDesc.h"
#include "Useless/Graphic/FileIO/ImageFactory.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"
#include "Useless/File/VIFS.h"

namespace Useless {

PixelBuffer::PixelBuffer(): _data(0), _buffer_size(0)
{
}

PixelBuffer::~PixelBuffer()
{
    FreePixels();
}
    
void PixelBuffer::FreePixels()
{
    delete[] _data;
    _data = 0;
    _buffer_size = 0;
}

void PixelBuffer::AllocatePixels( int w, int h, int fmt, int alignment_x, int alignment_y, int border_w, int border_h )
{
    FreePixels();
    _w = w;
    _h = h;
    _fmt = fmt;

    int rw = (_w % alignment_x);
    int rh = (_h % alignment_y);
    
    w = ( 0 != rw ? (_w + alignment_x - rw) : _w ) + border_w;
    h = ( 0 != rh ? (_h + alignment_y - rh) : _h ) + border_h;

    _bytes_pp=ImageFormat::GetBpp( ImageFormat::Format(_fmt));
    _pitch = w * _bytes_pp;
    _buffer_size = h * _pitch;
    _data = new char[ _buffer_size ];
    for ( char *p=_data, *P=_data + _buffer_size; p != P; ++p ) { (*p) = 0; }
    // ???
    _pitch = _w * _bytes_pp;
}

bool PixelBuffer::BoundCheck( char *ptr, int w, int h ) const
{
    assert( w > 0 );
    assert( h > 0 );

    if ( ptr < _data ) { return false; }

    char *dataEnd = _data + _buffer_size;
    char *ptrLast = ptr + ((h-1) * GetPitch()) + (w * GetBytesPP());

    if ( ptrLast > dataEnd ) { return false; }

    return true;
}

char * PixelBuffer::GetData()
{
    return _data + _pitch + _bytes_pp;
}

const char * PixelBuffer::GetData() const
{
    return _data + _pitch + _bytes_pp;
}

}; //namespace Useless
