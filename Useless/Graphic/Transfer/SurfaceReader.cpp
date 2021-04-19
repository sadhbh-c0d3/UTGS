#include "UselessPch.h"

#include "Useless/Graphic/Transfer/SurfaceReader.h"
#include "Useless/Graphic/Device/Surface.h"

namespace Useless {

SurfaceReader::SurfaceReader( const Surface &surface ): _source(surface)
{
    int flags = Surface::COLOR;
    Surf::Properties srcProp;
    surface.GetProperties( &srcProp );
    if ( srcProp.alpha )
    {
        flags += Surface::ALPHA;
    }
    _transfer = _source.CreateReader( flags );
    _dest_pitch = 0;
    const void *pal; int col;
    _transfer->GetSource( &_w, &_h, &_bpp, &pal, &col);
}

SurfaceReader::~SurfaceReader()
{}

void SurfaceReader::GetSize( int *width, int *height, int *depth )
{
    *width = _w;
    *height = _h;
    *depth = _bpp;
}

void SurfaceReader::Read( Types::UBYTE *dest, int x, int y, int w, int h, int pitch )
{
    if ( !_dest_pitch )
    {   
        int bytes_pp = ImageFormat::GetBpp( ImageFormat::Format(_bpp));
        _dest_pitch = (pitch)? pitch : _w * bytes_pp;
        _transfer->SetDestination( _w, _h, _dest_pitch, _bpp );
        _transfer->Transform();
    }
    else
        { assert( _dest_pitch==pitch || !pitch ); }

    _transfer->Store( dest, Rect(x,y,w,h));
}

void SurfaceReader::SetFormat( int format )
{
    _w = _source.GetWidth();
    _h = _source.GetHeight();
    _bpp = format;
}

void SurfaceReader::ReadPalette( const Types::UBYTE **pPallete, int *pCount)
{
    static Types::UBYTE sPalette[4*256];

    for ( int i=0; i<256; ++i )
    {
        sPalette[4*i+0] = i;
        sPalette[4*i+1] = i;
        sPalette[4*i+2] = i;
        sPalette[4*i+3] = i;
    }

    *pCount = 256;
    *pPallete = sPalette;
}

};//namespace Useless
