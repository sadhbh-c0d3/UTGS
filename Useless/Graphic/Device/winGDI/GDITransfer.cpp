#include "UselessPch.h"

#include "Useless/Graphic/Device/winGDI/GDITransfer.h"
#include "Useless/Graphic/detail/ImageDesc.h"
#include "Useless/Graphic/Rect.h"

namespace Useless {

GDITransfer::GDITransfer( HDC hdc, HBITMAP bmp )
{
    _hdc=hdc;
    _bmp=bmp;

    _width=0;
    _height=0;
    _pitch=0;
    _pixel_format=0;
    _palette=0;
    _colors=0;
    _source=0;
}

GDITransfer::~GDITransfer()
{
}

void GDITransfer::Fetch( const void *source, const Rect &area )
{
    int bytes = ImageFormat::GetBpp( ImageFormat::Format(_pixel_format));
    int offset = (_height-area.GetY()-1) * _bmp_pitch + area.GetX()*bytes;
    int line_s = area.GetW() * bytes;

    Types::UBYTE *s_ptr = (Types::UBYTE*)source;
    Types::UBYTE *d_ptr = _buffer.GetPtrUB( offset > 0 ? offset : -offset );

    Types::UBYTE *d_min = _buffer.GetPtrUB();
    Types::UBYTE *d_max = _buffer.GetPtrUB( _buffer.GetSize() );

    int h = area.GetH();
    while (h--)
    {
        assert( d_ptr >= d_min );
        assert( d_ptr + line_s <= d_max );

        memcpy( d_ptr, s_ptr, line_s);
        s_ptr += _pitch;
        d_ptr -= _bmp_pitch;//_pitch;
    }
}

void GDITransfer::Transform()
{
    void *ptr;
    if ( 0 == _buffer.GetSize())
    {
        ptr = _source;
    }
    else {
        ptr = _buffer.GetPtr();
        if ( _source)
        {
            Fetch( _source, Rect(_width, _height));
        }
    }
    ::SetDIBits( _hdc, _bmp, 0, _height, ptr, (BITMAPINFO*)&_binf, DIB_RGB_COLORS);
}

void GDITransfer::SetSource( int  width,
                        int  height,
                        int  pitch,
                        int  pixel_format,
                 const void *palette ,
                        int  colors )
{
    _width=width;
    _height=height;
    _pitch=pitch;
    _pixel_format=pixel_format;
    _palette=palette;
    _colors=colors;
    _bmp_pitch=(_pitch < 0 ? -_pitch : _pitch);
    
    if ( !_source || pitch>0)
    {
        if ( _pitch % 4 )
        {
            _bmp_pitch += 4 - (_pitch % 4);
        }
        
        _buffer.Alloc( height * _bmp_pitch );
        
    }
    else {
        _source = ((char*)_source) + pitch*(_height-1);
    }

    _binf.bmiHeader.biSize=40;//sizeof(BITMAPINFOHEADER);
    _binf.bmiHeader.biWidth=_width;
    _binf.bmiHeader.biHeight=_height;
    _binf.bmiHeader.biPlanes=1;
    _binf.bmiHeader.biBitCount=ImageFormat::GetBpp( ImageFormat::Format(_pixel_format))*8;
    _binf.bmiHeader.biCompression=0;//BI_RGB;
    _binf.bmiHeader.biSizeImage=0;//_buffer.GetSize();
    _binf.bmiHeader.biXPelsPerMeter=10000;
    _binf.bmiHeader.biYPelsPerMeter=10000;
    _binf.bmiHeader.biClrUsed=256;
    _binf.bmiHeader.biClrImportant=256;

    for ( int i=0; i<256; ++i )
    {
        _binf.bmiColors[i].rgbBlue = i;
        _binf.bmiColors[i].rgbGreen = i;
        _binf.bmiColors[i].rgbRed = i;
        _binf.bmiColors[i].rgbReserved = i;
    }
}
    
void GDITransfer::GetSource( int  *width,
                            int  *height,
                            int  *pixel_format,
                     const void **palette,
                            int  *colors )
{
    *width=_width;
    *height=_height;
    *pixel_format=_pixel_format;
    *palette=_palette;
    *colors=_colors;
}

};//namespace Useless
