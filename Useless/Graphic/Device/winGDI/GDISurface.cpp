#include "UselessPch.h"

#include "Useless/Graphic/Device/winGDI/GDISurface.h"
#include "Useless/Graphic/Device/winGDI/GDITransfer.h"
#include "Useless/System/TypeInfo.h"
#include "Useless/ErrorConfig.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"
#include "Useless/Graphic/detail/ImageDesc.h"
#include "Useless/System/Application.h"

namespace Useless {

GDISurface::GDISurface()
    : _constantAlpha(255)
{
}

GDISurface::GDISurface( const Surf::Properties &properties, HWND hwnd )
    : _constantAlpha(255)
{
    Create( properties, hwnd);
}

GDISurface::GDISurface( const Surf::Properties &properties, HWND hwnd, int resourceName )
{
    _hbmp = ::LoadBitmap( Application::GetInstance(), MAKEINTRESOURCE( resourceName ));
    
    HDC window_dc=::GetDC( _hwnd=hwnd);
    _hdc=::CreateCompatibleDC( window_dc );
    HGDIOBJ hgdiobj=::SelectObject( _hdc, _hbmp );
    assert(hgdiobj!=0);
    _properties=properties;
}

void GDISurface::Create( const Surf::Properties &properties, HWND hwnd )
{
    HDC window_dc=::GetDC( _hwnd=hwnd);

    if ( !properties.color ) { APP_ERROR(Error("surface without color plane"),"GDISurface"); }
    if ( properties.zbuffer ) { APP_ERROR(Error("zbuffer is not supported"),"GDISurface"); }
    if ( !properties.alpha )
    {
        _hbmp=::CreateCompatibleBitmap( window_dc, properties.width, properties.height);
    }
    else
    {
        BITMAPINFO bmp;
        bmp.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        bmp.bmiHeader.biWidth=properties.width;
        bmp.bmiHeader.biHeight=properties.height;
        bmp.bmiHeader.biPlanes=1;
        bmp.bmiHeader.biBitCount=32;
        bmp.bmiHeader.biCompression=BI_RGB;
        bmp.bmiHeader.biSizeImage=0;
        bmp.bmiHeader.biXPelsPerMeter=10000;
        bmp.bmiHeader.biYPelsPerMeter=10000;
        bmp.bmiHeader.biClrUsed=0;
        bmp.bmiHeader.biClrImportant=0;
        _hbmp=::CreateDIBSection( window_dc, &bmp, DIB_RGB_COLORS, &_p_data, 0, 0 );
    }

    _hdc=::CreateCompatibleDC( window_dc);
    HGDIOBJ hgdiobj=::SelectObject( _hdc, _hbmp);
    assert(hgdiobj!=0);
    _properties=properties;
}

GDISurface::~GDISurface()
{
    ::DeleteDC( _hdc);
    ::DeleteObject( _hbmp);
}

Surface* GDISurface::CreateSurface( Surf::Properties &properties ) const
{
    return new GDISurface( properties, _hwnd);
}

SPointer<PixelTransfer> GDISurface::CreateWriter( int channel_format )
{
    //assert( channel_format==COLOR);
    PixelTransfer *pix_tr=0;
    if ( _properties.alpha )
    {
        int pitch=_properties.width*4;
        char *p_data=static_cast<char*>(_p_data);
        p_data+=pitch*(_properties.height-1);
        int fmt=(channel_format==COLOR)? ImageFormat::B8G8R8A8 : ImageFormat::ALPHA8PM;
        pix_tr=new GenericTransfer( 0, p_data);
        pix_tr->SetDestination( _properties.width, _properties.height, -pitch, fmt);
    }
    else
    {
        pix_tr=new GDITransfer( _hdc, _hbmp);
    }
    return pix_tr;
}

void GDISurface::SubBlit( const Pos &p, const GDISurface &s, const Rect &r, const Surf::BlitFX &fx )
{
    if ( s._properties.has_color_key ) // requires at least win98 or NT2000 and Msimg32.lib
    { 
        NormalPixel npix=s._properties.color_key;

        COLORREF ck=RGB( npix.c.r, npix.c.g, npix.c.b);
        ck=GetNearestColor( s._hdc, ck);
        ::TransparentBlt( _hdc, p.x, p.y, r.GetW(), r.GetH(),
                        s._hdc, r.GetX(), r.GetY(), r.GetW(), r.GetH(),
                        ck);
    }
    else if ( _constantAlpha < 255 )
    {
        BLENDFUNCTION blfun;
        blfun.BlendOp=(s._properties.alpha ? AC_SRC_OVER : 0);
        blfun.BlendFlags=0;
        blfun.SourceConstantAlpha = _constantAlpha;
        blfun.AlphaFormat=AC_SRC_ALPHA;

        ::AlphaBlend( _hdc, p.x, p.y, r.GetW(), r.GetH(),
                    s._hdc, r.GetX(), r.GetY(), r.GetW(), r.GetH(), blfun);
    }
    else if ( s._properties.alpha ) // requires at least win98 or NT2000 and Msimg32.lib
    {
        BLENDFUNCTION blfun;
        blfun.BlendOp=AC_SRC_OVER;
        blfun.BlendFlags=0;
        blfun.SourceConstantAlpha = fx.alpha_factors[1] - fx.alpha_factors[0];
        blfun.AlphaFormat=AC_SRC_ALPHA;

        ::AlphaBlend( _hdc, p.x, p.y, r.GetW(), r.GetH(),
                    s._hdc, r.GetX(), r.GetY(), r.GetW(), r.GetH(), blfun);
    }
    /*else if ( fx.stretch) // < TODO: calculate correct rectangles - TOO SLOW!
    {
        ::StretchBlt( _hdc, p.x, p.y, r.GetW(), r.GetH(),
                        s._hdc, r.GetX(), r.GetY(), r.GetW(), r.GetH(),
                        SRCCOPY);
    }*/
    else
    {
        ::BitBlt( _hdc, p.x, p.y, r.GetW(), r.GetH(),
                s._hdc, r.GetX(), r.GetY(), SRCCOPY);
    }
}

void GDISurface::Blit
(
    int x,
    int y,
    const Surface &src,
    const Useless::Rect &src_rect,
    const Surf::BlitFX &fx
)
{
    const GDISurface &gdi_src=static_cast<const GDISurface&>(src);
    Pos put_p(x,y); //position where to put
    Rect bound_rc( 0,0, _properties.width, _properties.height); //dest bound rect
    Pos ss_p = src_rect.GetPos(); //src crop rc pos in src coords
    Pos sd_p = put_p-ss_p; //src pos in dst coords
    Rect sd_r = src_rect+sd_p; //src crop rc in dst coords
    sd_r &= bound_rc;

    RectList::iterator i=_clipper.begin(), iend=_clipper.end();
    for (; i!=iend; ++i )
    {
        Rect r = (*i); //nth clip rect
        r &= sd_r;  //intersection in dest coords
        Pos p = r.GetPos(); //position in dest coords
        SubBlit( p, gdi_src, r-sd_p, fx);
    }
}

void GDISurface::Clear( unsigned long color, const Useless::Rect &drc )
{
    NormalPixel npix=color;
    COLORREF c=RGB( npix.c.r, npix.c.g, npix.c.b);
    c=GetNearestColor(_hdc, c);
    HBRUSH hbr=CreateSolidBrush(c);

    RectList::iterator i=_clipper.begin(), iend=_clipper.end();
    for (; i!=iend; ++i )
    {
        Rect rc=drc&(*i);

        RECT wrc;
        wrc.left=rc.GetX1();
        wrc.top=rc.GetY1();
        wrc.right=rc.GetX2();
        wrc.bottom=rc.GetY2();

        ::FillRect( _hdc, &wrc, hbr);
    }

    DeleteObject( hbr);
}
    
bool GDISurface::IsValidBlitSource( const Surface &surface ) const
{
    if ( const GDISurface *pSource = dynamic_cast< const GDISurface *>( &surface ))
    {
        if ( 0 != pSource->_hbmp )
        {
            return true;
        }
    }
    return false;
}

bool GDISurface::IsColorKeyed () const
{
    return _properties.has_color_key;
}
bool GDISurface::SetColorKey  (unsigned long color)
{
    _properties.has_color_key = true;
    _properties.color_key = color&0xFFFFFF;
    return true;
}
void GDISurface::SetNoColorKey()
{
    _properties.has_color_key = false;
}

bool GDISurface::SetClipper( const RectList &region )
{
    _clipper=region;
    return true;
}

RectList GDISurface::GetClipper( const Rect &crop ) const
{
    if ( !crop )
    {
        return _clipper;
    }
    RectList cropped_list;
    RectList::const_iterator i=_clipper.begin(), iend=_clipper.end();
    for (; i!=iend; ++i )
    {
        if ( (*i) && crop )
        {
            cropped_list.push_back( (*i) & crop );
        }
    }
    return cropped_list;
}

void GDISurface::SetConstantAlpha( int alpha )
{
    _constantAlpha = alpha;
}

int  GDISurface::GetConstantAlpha() const
{
    return _constantAlpha;
}

bool GDISurface::operator==( const Surface& s )
{
    return ( typeid(s)==typeid(GDISurface) );
}

bool GDISurface::operator!=( const Surface& s )
{
    return !((*this)==s);
}


};//namespace Useless
