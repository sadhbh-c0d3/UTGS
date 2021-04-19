#include "UselessPch.h"

#include "Useless/Graphic/Transfer/SurfaceWriter.h"
#include "Useless/Graphic/Device/Surface.h"

namespace Useless {

SurfaceWriter::SurfaceWriter( Surface &dest_srf ): _dest(dest_srf)
{
    _transfer = _dest.CreateWriter( Surface::COLOR);
    _source_pitch = 0;
    void *pal; int col;
    _transfer->GetDestination( &_w, &_h, &_bpp, &pal, &col);
}

void SurfaceWriter::Write(const Useless::Types::UBYTE *src, int x, int y, int w, int h, int pitch)
{
    if ( !_source_pitch )
    {
        int bytes_pp = ImageFormat::GetBpp( ImageFormat::Format(_bpp));
        _source_pitch = (pitch)? pitch : _w * bytes_pp;
        _transfer->SetSource( _w, _h, _source_pitch, _bpp );
    }
    else
        { assert( _source_pitch==pitch || !pitch ); }

    _transfer->Fetch( src, Rect(x,y,w,h));
}

void SurfaceWriter::WritePalette(const Useless::Types::UBYTE *ptr, int n)
{
}

void SurfaceWriter::SetSize(int w, int h, int bpp)
{
    _w=w; _h=h; _bpp=bpp;
}

void SurfaceWriter::Flush()
{
    _transfer->Transform();
    _transfer = 0;
}

SurfaceWriter::~SurfaceWriter()
{
    Flush();
}


};// namespace Useless
