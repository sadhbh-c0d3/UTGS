#include "UselessPch.h"

#include "Useless/Graphic/Device/winGDI/GDIScreen.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"
#include "Useless/ErrorConfig.h"
#define GDIERR(e) APP_ERROR(e,"GDIScreen");

namespace Useless {

HWND GDIScreen::s_hwnd;

GDIScreen::GDIScreen( const char *title): Window( title), _w(0), _h(0)
{
    s_hwnd = Window::GetHandle();
    _hdc = 0;
}

GDIScreen::~GDIScreen()
{
    if (_hdc) { ::ReleaseDC( Window::GetHandle(), _hdc); _hdc=0;}
}

void GDIScreen::Open( int width, int height, int bpp, float refresh )
{
    //assert( refresh == 0 && "Refresh not implemented for GDIScreen" );
    HWND hwnd = Window::GetHandle();
    if (_hdc) { ::ReleaseDC( Window::GetHandle(), _hdc); _hdc=0;}
    _hdc = GetDC( hwnd);
    SetWindowLong( hwnd, GWL_STYLE, WS_POPUP);
    Window::Resize( _w=width, _h=height);

    DEVMODE devmode;
    memset( &devmode, 0L, sizeof(DEVMODE));
    devmode.dmSize = sizeof(DEVMODE);

    int maxRefresh = 0;
    int i,j;
    for ( i=0,j=0;; ++i)
    {
        if ( !::EnumDisplaySettings( 0, ++i, &devmode ))
        {
            break;
        }
        if ( devmode.dmBitsPerPel==bpp &&
             devmode.dmPelsWidth==width &&
             devmode.dmPelsHeight==height )
        {
            if ( devmode.dmDisplayFrequency > maxRefresh )
            {
                maxRefresh = devmode.dmDisplayFrequency;
                j=i;
            }

            if ( devmode.dmDisplayFrequency == refresh )
            {
                break;
            }
        }
    }
    ::EnumDisplaySettings( 0, j, &devmode );
    devmode.dmFields |= DM_DISPLAYFREQUENCY;

    long res=ChangeDisplaySettingsEx( 0, &devmode, 0, CDS_FULLSCREEN, 0);
    switch(res)
    {
    case DISP_CHANGE_BADFLAGS: GDIERR("DISP_CHANGE_BADFLAGS"); break;
    case DISP_CHANGE_BADMODE: GDIERR("DISP_CHANGE_BADMODE"); break;
    case DISP_CHANGE_BADPARAM: GDIERR("DISP_CHANGE_BADPARAM"); break;
    case DISP_CHANGE_FAILED: GDIERR("DISP_CHANGE_FAILED"); break;
    case DISP_CHANGE_RESTART: GDIERR("DISP_CHANGE_RESTART"); break;
    };

    int w=::GetSystemMetrics( SM_CXSCREEN);
    int h=::GetSystemMetrics( SM_CYSCREEN);
    Window::Reposition( (w-_w)/2, (h-_h)/2);

    Surf::Properties properties;
    properties.width=width;
    properties.height=height;
    _back_buffer = new GDISurface( properties, hwnd);
    SetClipper( Rect(0,0,_w,_h));

    Window::Show();
}

void GDIScreen::OpenWindowed( int width, int height)
{
    Rect dr = GetWindowRect() - GetClientRect();
    HWND hwnd=Window::GetHandle();
    if (_hdc) { ::ReleaseDC( Window::GetHandle(), _hdc); _hdc=0;}
    _hdc = GetDC( hwnd);
    _w = width;
    _h = height;
    Window::Resize( _w + dr.GetW(), _h + dr.GetH() );
    Surf::Properties properties;
    properties.width=width;
    properties.height=height;
    _back_buffer = new GDISurface( properties, Window::GetHandle());
    SetClipper( Rect(0,0,_w,_h));
    Window::Show();
}

void GDIScreen::Close()
{
    if (_hdc) { ::ReleaseDC( Window::GetHandle(), _hdc); _hdc=0;}
    _back_buffer = 0;
}

void GDIScreen::Swap()
{
    Copy();
}

void GDIScreen::Copy()
{
    PAINTSTRUCT paint;
    ::BeginPaint( Window::GetHandle(), &paint);
    ::BitBlt( _hdc, 0,0, _w, _h, _back_buffer->_hdc, 0,0, SRCCOPY);
    ::GdiFlush();
    ::EndPaint( Window::GetHandle(), &paint);
}

Point<double> GDIScreen::GetZoomFactors() const
{
    return Point<double>(1,1);
}

const Surface* GDIScreen::GetSurface() const
{
    return &*_back_buffer;
}

Surface* GDIScreen::GetSurface()
{
    return &*_back_buffer;
}

void GDIScreen::Clear( NormalPixel color, const Useless::Rect &area )
{
    _back_buffer->Clear( color.u, area);
}

void GDIScreen::SetClipper( const RectList &rect_list )
{
    _back_buffer->SetClipper( rect_list);
}

RectList GDIScreen::GetClipper( const Rect &crop ) const
{
    return _back_buffer->GetClipper( crop);
}


ImageBuffer GDIScreen::GetBackBuffer()
{
    assert(0);
    return ImageBuffer();
}

SPointer<IGraphics> GDIScreen::QueryIGraphics( const Rect &cut )
{
    assert(0);
    return new ImageBuffer();
}


void GDIScreen::PrintFPS( int x, int y, double dt )
{
    if ( dt )
    {
        char text[20];
        sprintf(text, "FPS: %5.2f", 1000.0/dt );
        PrintText(x,y,text);
    }
}

void GDIScreen::PrintNum(int x, int y, int fixed, int fraction)
{
    char text[40];
    ZeroMemory(&text,40);
    sprintf(text, "%8i.%8i", fixed, fraction );
    PrintText(x,y,text);
}

void GDIScreen::PrintText(int x, int y, const char* text)
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
    
    TCHAR_string textout( ToTCHAR( text));

    COLORREF c=::GetNearestColor( _back_buffer->_hdc, RGB(0x00,0xFF,0xC0));
    ::SelectObject( _back_buffer->_hdc, font);
    ::SetBkMode( _back_buffer->_hdc, TRANSPARENT );
    ::SetBkColor( _back_buffer->_hdc, 0L );
    ::SetTextColor( _back_buffer->_hdc, c);
    ::TextOut( _back_buffer->_hdc, x, y, textout.c_str(), textout.length());
    ::SelectObject( _back_buffer->_hdc, _back_buffer->_hbmp);
    ::DeleteObject( font);
}

};//namespace Useless
