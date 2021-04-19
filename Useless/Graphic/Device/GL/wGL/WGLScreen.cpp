#include "UselessPch.h"

#include "Useless/Graphic/Device/GL/wGL/WGLScreen.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"
#include "Useless/ErrorConfig.h"
#define WGLERR(e) APP_ERROR(e,"WGLScreen");

namespace Useless {

    WGLScreen::WGLScreen( const char *title): Window( title ), m_active( false )
    {

        // aviod refreshing with windows background color
        AllowClear(0);
        
        // Screen surface has these attributes:
        m_prop.primary = true;
        m_prop.renderable = true;
        m_prop.color = true;
        m_prop.zbuffer = true;
        m_prop.num_surfaces = 2;
    }

    WGLScreen::~WGLScreen()
    {
        Close();
    }

    void WGLScreen::Open( int width, int height, int bpp, float refresh )
    {
        Close();
        
        m_windowed = false;
        m_prop.width = width;
        m_prop.height = height;
        m_prop.pixelformat = ImageFormat::R8G8B8;

        HWND hwnd = Window::GetHandle();
        HDC hdc = GetDC( hwnd );

        SetWindowLong( hwnd, GWL_STYLE, WS_POPUP);

        InitDevMode( width, height, bpp, refresh );
        _glContextSurface.Reset( new WGLSurface( hdc, m_prop ) );
            
        // Signals below are used only in GDI fullscreen.
        // They are not available under DDraw fullscreen.
        Tie2Signal( OnActivate, this, &WGLScreen::SlotFullscreenActive );
        Tie2Signal( OnDeactivate, this, &WGLScreen::SlotFullscreenInactive );
        
        _glContextSurface->GetProperties( &m_prop );

        Tie2Signal( OnResize, this, &WGLScreen::SlotResize );
        Window::Show();
    }

    void WGLScreen::OpenWindowed( int width, int height)
    {
        Close();
        
        m_windowed  = true;
        m_prop.width = width;
        m_prop.height = height;
        m_prop.pixelformat = ImageFormat::R8G8B8;
        
        Rect dr = GetWindowRect() - GetClientRect();
        Window::Resize( width + dr.GetW(), height + dr.GetH() );
        Window::Show();

        HWND hwnd = Window::GetHandle();
        HDC hdc = GetDC( hwnd );
        
        _glContextSurface.Reset( new WGLSurface( hdc, m_prop ) );
        
        Tie2Signal( OnResize, this, &WGLScreen::SlotResize );
    }

    bool DbgTrace_DevMode( int w, int h, int bpp, int f )
    {
        return false;
    }

    struct DispMode
    {
        int i,w,h,d,f;
        
        static int sw,sh,sd,sf;

        DispMode( int ii, DEVMODE &d ) : i(ii), w( d.dmPelsWidth ), h( d.dmPelsHeight ), d( d.dmBitsPerPel ), f( d.dmDisplayFrequency )
        {
        }

        bool operator < ( const DispMode &b ) const
        {
            if ( d < 15 )
            {
                return false;
            }
            else if ( b.d < 15 )
            {
                return true;
            }

            if ( h < sh )
            {
                return false;
            }
            else if ( b.h < sh )
            {
                return true;
            }

            if ( w < sw )
            {
                return false;
            }
            else if ( b.w < sw )
            {
                return true;
            }
            
            int dw0 = ( w - sw ) * ( w - sw );
            int dw1 = ( b.w - sw ) * ( b.w - sw );
            
            int dh0 = ( h - sh ) * ( h - sh );
            int dh1 = ( b.h - sh ) * ( b.h - sh );
            
            int df0 = ( f - sf ) * ( f - sf );
            int df1 = ( b.f - sf ) * ( b.f - sf );

            int dd0 = (d - sd) * (d - sd);
            int dd1 = (b.d - sd) * (b.d - sd);

            if ( dw0 + 4 * dh0 > dw1 + 4 * dh1 )
            {
                return false;
            }
            else if ( dw0 + 4 * dh0 < dw1 + 4 * dh1 )
            {
                return true;
            }

            if ( dd0 > dd1 )
            {
                return false;
            }
            else if ( dd0 < dd1 )
            {
                return true;
            }
            
            if ( df0 > df1 )
            {
                return false;
            }

            return true;
        }
    };
        
    int DispMode::sw = 0,DispMode::sh = 0,DispMode::sd = 0,DispMode::sf = 0;

    void WGLScreen::InitDevMode( int width, int height, int bpp, float refresh )
    {
        HWND hwnd = Window::GetHandle();
        HDC hdc = GetDC( hwnd );

        DispMode::sw = width;
        DispMode::sh = height;
        DispMode::sd = bpp;
        DispMode::sf = ( refresh > 0 ? refresh : 100 );
        
        memset( &m_devmode, 0L, sizeof(DEVMODE));
        m_devmode.dmSize = sizeof(DEVMODE);

        std::multiset< DispMode > dispModes;
        int j = -1;
            
        for ( int i=0; ::EnumDisplaySettings( 0, i, &m_devmode ); ++i )
        {
            dispModes.insert( DispMode( i, m_devmode ));
            j = dispModes.begin()->i;
        }

        FILE *fDump = fopen("DisplayModes.log", "a+");
        fprintf( fDump, "[DisplayModes]\n" );
        fprintf( fDump, "Requested Mode: %i x %i x %i bpp x %i Hz\n", DispMode::sw, DispMode::sh, DispMode::sd, DispMode::sf );
        for ( std::multiset< DispMode >::iterator it = dispModes.begin(); it != dispModes.end(); ++it )
        {
            fprintf( fDump, "Mode[0x%08x]: %i x %i x %i bpp x %i Hz\n", it->i, it->w, it->h, it->d, it->f );
        }
        fprintf( fDump, "---\n" );

        if ( -1 == j )
        {
            throw Error("InitDevMode: Cannot find mode: %i x %i x %i : %f Hz.", width, height, bpp, refresh );
        }
        ::EnumDisplaySettings( 0, j, &m_devmode );
        DbgTrace_DevMode( m_devmode.dmPelsWidth, m_devmode.dmPelsHeight, m_devmode.dmBitsPerPel, m_devmode.dmDisplayFrequency );
        m_devmode.dmFields |= DM_DISPLAYFREQUENCY;
    }

    void WGLScreen::SlotFullscreenActive()
    {
        if ( m_active )
        {
            return;
        }
        m_active = true;
        if ( !m_windowed )
        {
            Window::Resize( m_prop.width, m_prop.height );
            Window::Reposition( 0, 0);

            long res = ChangeDisplaySettingsEx( 0, &m_devmode, 0, CDS_FULLSCREEN, 0);
            switch(res)
            {
                case DISP_CHANGE_SUCCESSFUL: break;
                case DISP_CHANGE_BADFLAGS: WGLERR("DISP_CHANGE_BADFLAGS"); break;
                case DISP_CHANGE_BADMODE: WGLERR("DISP_CHANGE_BADMODE"); break;
                case DISP_CHANGE_BADPARAM: WGLERR("DISP_CHANGE_BADPARAM"); break;
                case DISP_CHANGE_FAILED: WGLERR("DISP_CHANGE_FAILED"); break;
                case DISP_CHANGE_RESTART: WGLERR("DISP_CHANGE_RESTART"); break;
                default: WGLERR("Unknown reason."); break;
            };
	    Window::Show();
        }
    }

    void WGLScreen::SlotFullscreenInactive()
    {
        if ( !m_active )
        {
            return;
        }
        m_active = false;
        if ( !m_windowed )
        {
            Minimize();
            ::ChangeDisplaySettingsEx( NULL, NULL, NULL, 0, NULL );
        }
    }
            
    void WGLScreen::SlotResize( int w, int h )
    {
        if (!!_glContextSurface && ( w > 0 && h > 0 ))
        {
            _glContextSurface->SetSize( w, h );
            _glContextSurface->GetProperties( &m_prop );
        }
    }

    void WGLScreen::Close()
    {
        if ( !!_glContextSurface )
        {
            HDC hdc = _glContextSurface->_devContext;
            _glContextSurface.Reset();
            ::ReleaseDC( GetHandle(), hdc );
        }
    }

    void WGLScreen::Swap()
    {
        if (!!_glContextSurface && ( m_windowed || m_active ))
        {
            SwapBuffers( _glContextSurface->_devContext );
            _glContextSurface->GLFreeGarbage();
            GLUpdateMovieSamples( _glContextSurface.get() );
        }
    }

    void WGLScreen::Copy()
    {
        if (!!_glContextSurface && ( m_windowed || m_active ))
        {
            SwapBuffers( _glContextSurface->_devContext );
        }
    }

    Point<double> WGLScreen::GetZoomFactors() const
    {
        return Point<double>(1,1);
    }

    void WGLScreen::Clear( NormalPixel color, const Useless::Rect &area )
    {
        if (!!_glContextSurface && ( m_windowed || m_active ))
        {
            _glContextSurface->Clear( color.u, area );
        }
    }

    void WGLScreen::SetClipper( const RectList &rect_list )
    {
        if (!!_glContextSurface && (m_windowed || m_active ))
        {
            _glContextSurface->SetClipper( rect_list );
        }
    }

    RectList WGLScreen::GetClipper( const Rect &crop ) const
    {
        if (!!_glContextSurface && ( m_windowed || m_active ))
        {
            return _glContextSurface->GetClipper( crop);
        }
        else
        {
            return RectList();
        }
    }


    ImageBuffer WGLScreen::GetBackBuffer()
    {
        assert(0);
        return ImageBuffer();
    }

    SPointer<IGraphics> WGLScreen::QueryIGraphics( const Rect &cut )
    {
        assert(0);
        return new ImageBuffer();
    }


    void WGLScreen::PrintFPS( int x, int y, double dt )
    {
        if ( dt )
        {
            char text[20];
            sprintf(text, "FPS: %5.2f", 1000.0/dt );
            PrintText(x,y,text);
        }
    }

    void WGLScreen::PrintNum(int x, int y, int fixed, int fraction)
    {
        char text[40];
        ZeroMemory(&text,40);
        sprintf(text, "%8i.%8i", fixed, fraction );
        PrintText(x,y,text);
    }

    void WGLScreen::PrintText(int x, int y, const char* text)
    {
        if ( !m_windowed && !m_active )
        {
            return;
        }

        // Initialize default font
        HFONT font = ::CreateFont
            (
             15,
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

        HDC hdc = _glContextSurface->_devContext;

        COLORREF c=::GetNearestColor( hdc, RGB( 0xFF, 0x00, 0x00 ));
        HGDIOBJ lastObj = ::SelectObject  ( hdc, font );
        ::SetBkMode     ( hdc, TRANSPARENT );
        ::SetBkColor    ( hdc, 0L );
        ::SetTextColor  ( hdc, c);
        ::TextOut       ( hdc, x, y, textout.c_str(), textout.length() );
        ::SelectObject  ( hdc, lastObj );
        ::DeleteObject  ( font );
    }

};//namespace Useless
