#include "Useless/UselessPch.h"

#include "DDScreen.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"
#include "Useless/Graphic/Device/DDraw/DDError.h"

#include "Useless/ErrorConfig.h"
#define DDScreen_ERROR(e) APP_ERROR(e,"DDScreen");

namespace Useless {

DDScreen::DDScreen( const char *title) throw()
    :DDrawWindow( title ), Window( title)
{}

DDScreen::DDScreen(int w, int h, int bpp, const char *title) throw()
    :Window( title), DDrawWindow(title, w, h, bpp), _w(w), _h(h), _bpp(bpp)
{
    SetClipper( Rect( w, h));
    Clear(0L); Swap();
    SetClipper( Rect( w, h));
    Clear(0L); Swap();
}

DDScreen::~DDScreen()
{
    _emul_sh_surface.ReleaseTarget();
}

void DDScreen::Open(int w, int h, int bpp, float refresh)
{
    assert( refresh == 0 && "refresh rate is not implemented for DDraw7" );
    _bpp=bpp; _w=w; _h=h;
    if( FAILED( DDrawWindow::GetDisplay().CreateFullScreen( Window::GetHandle(), w, h, bpp ) ) )
    {
        DDScreen_ERROR( Useless::Error("DDrawDDScreen::Open() Mode %ix%ix%i is not supported",w,h,bpp) );
    }
    SetClipper( Rect( w, h));
    Clear(0L); Swap();
    SetClipper( Rect( w, h));
    Clear(0L); Swap();
}
    
void DDScreen::OpenWindowed(int w, int h)
{
    _bpp=0; _w=w; _h=h;
    if( FAILED( DDrawWindow::GetDisplay().CreateWindowed( Window::GetHandle(), w, h ) ) )
    {
        DDScreen_ERROR( Useless::Error("DDrawDDScreen::Open() Mode %ix%i is not supported",w,h) );
    }
    Show();
    SetClipper( Rect( w, h));
    Clear(0L); Swap();
}

void DDScreen::Close()
{
    GetDisplay().DestroyObjects();
}

void DDScreen::RestoreDisplay()
{
    if ( DDrawWindow::GetDisplay().IsWindowed() )
    {
        Useless::Rect cr = Window::GetClientRect();
        OpenWindowed( _w, _h );
        MoveWindow( Window::GetHandle(), cr.GetX1(), cr.GetY1(), cr.GetW(), cr.GetH(), true );
    }
}

void DDScreen::Clear( NormalPixel color, const Useless::Rect &drc)
{
    DDSurface *dest = GetDisplay().GetBackBuffer();
    dest->Clear( color.u, drc );
    //RestoreDisplay(); 
}

void DDScreen::PrintFPS( int x, int y, double dt )
{
    if ( dt )
    {
        char text[20];
        sprintf(text, "FPS: %5.2f", 1000.0/dt );
        PrintText(x,y,text);
    }
}

void DDScreen::PrintNum(int x, int y, int fixed, int fraction)
{
    char text[40];
    ZeroMemory(&text,40);
    sprintf(text, "%8i.%8i", fixed, fraction );
    PrintText(x,y,text);
}

void DDScreen::PrintText(int x, int y, const char* text)
{
    HFONT font = ::CreateFont
    (
        12,
        8,
        0,
        0,
        0,
        false,
        false,
        false,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        FF_ROMAN,
        NULL
    );

    DDSurface *dest = GetDisplay().GetBackBuffer();
    
    TCHAR_string out = ToTCHAR(text);
    dest->DrawText(font, (TCHAR*)out.c_str(), x, y, 0L, 0xC0FF00);

    ::DeleteObject(font);
}

void DDScreen::Swap()
{
    DDrawWindow::GetDisplay().Present();
}

void DDScreen::Copy()
{
    DDrawWindow::GetDisplay().Present(true);
}

bool DDScreen::IsValid() const
{
    return
    DDrawWindow::GetDisplay().GetBackBuffer()->IsValid() &&
    DDrawWindow::GetDisplay().GetFrontBuffer()->IsValid();
}

ImageBuffer DDScreen::GetBackBuffer()
{
    // This is only Emulation of Share<Surface>
    // We actually don't wan't Share to be the Surface owner
    // So we tell Share to give up, and Release ownership
    // This way we'll avoid removal of our backbuffer Surface
    _emul_sh_surface.ReleaseTarget();
    _emul_sh_surface.SetTarget( GetSurface() );
    return ImageBuffer(_emul_sh_surface);
}

SPointer<IGraphics> DDScreen::QueryIGraphics( const Rect &cut )
{
    return GetBackBuffer().Copy(cut);
}

Surface *DDScreen::GetSurface()
{
    return DDrawWindow::GetDisplay().GetBackBuffer();
}

const Surface *DDScreen::GetSurface() const
{
    return DDrawWindow::GetDisplay().GetBackBuffer();
}

Point<double> DDScreen::GetZoomFactors() const
{
    DDSurface *p_back = DDrawWindow::GetDisplay().GetBackBuffer();
    Rect client = Window::GetClientRect();
    assert( p_back );
    double factor_x = client.GetW()/ (double)p_back->GetWidth();
    double factor_y = client.GetH()/ (double)p_back->GetHeight();

    return Point<double>( factor_x, factor_y );
}

/////////////////////////////////////////////////////////////////////////////////////
//  CLIPPERS

void DDScreen::SetClipper( const Useless::RectList &rects )
{
    _region_cache = rects;
    DDCreator &device = DDrawWindow::GetDisplay();
    device.GetBackBuffer()->SetClipper( rects );
}
RectList DDScreen::GetClipper( const Rect &crop ) const
{
    if ( !crop )
    {
        return _region_cache;
    }
    /*const DDCreator &device = DDrawWindow::GetDisplay();
    return device.GetBackBuffer()->DDSurface::GetClipper( crop );*/
    RectList cropped_list;
    RectList::const_iterator i=_region_cache.begin(), iend=_region_cache.end();
    for (; i!=iend; ++i )
    {
        if ( (*i) && crop )
        {
            cropped_list.push_back( (*i) & crop );
        }
    }
    return cropped_list;
}

};// namespace Useless
