#include "Useless/UselessPch.h"

#include "Useless/Graphic/Device/DDraw/DDSurface.h"
#include "Useless/Graphic/Device/DDraw/DDError.h"

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include "Useless/System/w32/DbgOut.h"
#include "Useless/ErrorConfig.h"
#define   DDCREATOR_ERROR(e) APP_CRITICAL(e, "DDCreator")

namespace Useless {

int PreInitializer<DDCreator>::_counter = 0;
DDraw7Ptr DDCreator::_p_ddraw = 0;
unsigned int DDCreator::_blitter_status = DD_OK;
unsigned int DDCreator::_flipper_status = DD_OK;
unsigned int DDCreator::_blitter_flags = /*DDBLT_DONOTWAIT*/ /*DDBLT_WAIT*/ DDBLT_ASYNC;
unsigned int DDCreator::_flipper_flags = DDFLIP_NOVSYNC /*DDFLIP_WAIT*/;

void DDCreator::Init()
{
    if( DirectDrawCreateEx( NULL, (void**)&_p_ddraw, IID_IDirectDraw7, NULL ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::Init(): DirectDrawCreateEx failed") );

    DDCAPS c_driver, c_hel;
    memset( &c_driver, 0, sizeof(DDCAPS) );
    memset( &c_hel, 0, sizeof(DDCAPS) );
    c_driver.dwSize = c_hel.dwSize =sizeof(DDCAPS);
    _p_ddraw->GetCaps( &c_driver, NULL );
    _p_ddraw->GetCaps( NULL, &c_hel );

    //Direct draw 7.0 says ALPHA feature is not implemented yet
    //As I found out, there is no implementation for ALPHA blitting in DDraw
    //Maybe in future we'll use D3D and cut wide surfaces to small 2^n pieces
    //If any out of these assertions fail then report it to me, couse
    //YOU'RE THE LUCKY ONE who has ALPHA support in DDraw
    assert( !c_driver.dwAlphaBltConstBitDepths );
    assert( !c_driver.dwAlphaBltPixelBitDepths );
    assert( !c_driver.dwAlphaBltSurfaceBitDepths );
    assert( !c_hel.dwAlphaBltConstBitDepths );
    assert( !c_hel.dwAlphaBltPixelBitDepths );
    assert( !c_hel.dwAlphaBltSurfaceBitDepths );
}

void DDCreator::Destroy()
{
    if (_p_ddraw)
    {
        _p_ddraw->Release();
        _p_ddraw = 0;
    }
}

DDCreator::DDCreator()
{
    _p_front = 0;
    _p_back  = 0;
    _windowed = 0;
}

DDCreator::~DDCreator()
{
    DestroyObjects();
}

void DDCreator::DestroyObjects()
{
    if( !!_p_ddraw )
    {
        _p_ddraw->SetCooperativeLevel( _hWnd, DDSCL_NORMAL );
    }
    delete _p_back; _p_back=0;
    delete _p_front; _p_front=0;
}

int DDCreator::CreateFullScreen( HWND hWnd, int width, int height, int bpp)
{
    // Cleanup anything from a previous call
    DestroyObjects();
    SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );

    // Set cooperative level
    if ( _p_ddraw->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): SetCooperativeLevel failed") );

    // Set the display mode
    if ( _p_ddraw->SetDisplayMode( width, height, bpp, 0, 0 ) !=DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): SetDisplayMode failed") );

    // Create primary surface (with backbuffer attached)
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize            = sizeof( ddsd );
    ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
                             DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
    ddsd.dwBackBufferCount = 1;

    DDrawSurface7 front_buffer;
    DDrawSurface7::Ptr back_buffer;

    if ( _p_ddraw->CreateSurface( &ddsd, &front_buffer, NULL ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): CreateSurface failed") );

    // Get a pointer to the back buffer
    DDSCAPS2 ddscaps;
    ZeroMemory( &ddscaps, sizeof( ddscaps ) );
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

    if( front_buffer->GetAttachedSurface( &ddscaps, &back_buffer ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): GetAttachedSurface failed") );

    _p_front = new DDSurface( front_buffer );
    _p_back  = new DDSurface( back_buffer );

    _hWnd = hWnd;
    _windowed = false;
    UpdateBounds();

    return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////



int DDCreator::CreateWindowed( HWND hWnd, int width, int height)
{
    // Cleanup anything from a previous call
    DestroyObjects();

    // Set cooperative level
    if( _p_ddraw->SetCooperativeLevel( hWnd, DDSCL_NORMAL ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): SetCooperativeLevel failed") );

    RECT  rcWork;
    RECT  rc;
    DWORD dwStyle;

    // If we are still a WS_POPUP window we should convert to a normal app
    // window so we look like a windows app.
    dwStyle  = GetWindowStyle( hWnd );
    dwStyle &= ~WS_POPUP;
    dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
    SetWindowLong( hWnd, GWL_STYLE, dwStyle );

    // Aet window size
    SetRect( &rc, 0, 0, width, height );

    AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL,
                        GetWindowExStyle(hWnd) );

    SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
                  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

    SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

    //  Make sure our window does not hang outside of the work area
    SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
    GetWindowRect( hWnd, &rc );
    if( rc.left < rcWork.left ) rc.left = rcWork.left;
    if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
    SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0,
                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

    LPDIRECTDRAWCLIPPER pcClipper;
    
    // Create the primary surface
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    DDrawSurface7 front_buffer, back_buffer;

    if( _p_ddraw->CreateSurface( &ddsd, &front_buffer, NULL ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): CreateSurface failed") );

    // Create the backbuffer surface
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth        = width;
    ddsd.dwHeight       = height;

    if( _p_ddraw->CreateSurface( &ddsd, &back_buffer, NULL ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): CreateSurface failed") );

    if( _p_ddraw->CreateClipper( 0, &pcClipper, NULL ) != DD_OK )
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): CreateClipper failed") );

    if( pcClipper->SetHWnd( 0, hWnd ) != DD_OK )
    {
        pcClipper->Release(); pcClipper=0;
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): SetHWnd failed") );
    }

    if( front_buffer->SetClipper( pcClipper ) != DD_OK )
    {
        pcClipper->Release(); pcClipper=0;
        DDCREATOR_ERROR( Error("DDCreator::CreateFullScreen(): SetClipper failed") );
    }

    // Done with clipper
    pcClipper->Release(); pcClipper=0;
    _p_front = new DDSurface( front_buffer );
    _p_back  = new DDSurface( back_buffer );

    _hWnd     = hWnd;
    _windowed = true;
    UpdateBounds();

    return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////



int DDCreator::InitClipper()
{
    LPDIRECTDRAWCLIPPER pClipper;
    HRESULT hr;

    // Create a clipper when using GDI to draw on the primary surface 
    if( FAILED( hr = _p_ddraw->CreateClipper( 0, &pClipper, NULL ) ) )
        return hr;

    pClipper->SetHWnd( 0, _hWnd );

    if( FAILED( hr = _p_front->GetDDrawSurface()->SetClipper( pClipper ) ) )
        return hr;

    // We can release the clipper now since g_pDDSPrimary 
    // now maintains a ref count on the clipper
    if (pClipper) { pClipper->Release(); pClipper=0; }

    return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////



int DDCreator::UpdateBounds()
{
    if( _windowed )
    {
        GetClientRect( _hWnd, &_rcWindow );
        ClientToScreen( _hWnd, (POINT*)&_rcWindow );
        ClientToScreen( _hWnd, (POINT*)&_rcWindow+1 );
    }
    else
    {
        SetRect( &_rcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) );
    }

    return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////


int DDCreator::Present(bool copy)
{
#ifdef DEBUG_REGION
    for ( int i=0; i!=g_SizeLastRegion; ++i )
    {
        print_dbg("[%i] %i %i %i %i\n", i, g_LastRegion[i] );
    }
#endif

    HRESULT hr;
    const DDrawSurface7 &front = _p_front->GetDDrawSurface();
    const DDrawSurface7 &back =  _p_back->GetDDrawSurface();

    _p_back->Leave_SW_Mode();

    if( !front && !back )
        return E_POINTER;

    while( 1 )
    {
        if( _windowed || copy)
            hr = front->Blt( &_rcWindow, back.get(), NULL, DDBLT_DONOTWAIT, NULL );
        else
        {
            //while ( back->GetBltStatus(DDGBS_ISBLTDONE) == DDERR_WASSTILLDRAWING  )
            {// wait for blitter to finish
            }
            hr = front->Flip( NULL, _flipper_flags );
            _flipper_status = hr;
        }

        if( hr == DDERR_SURFACELOST )
        {
            front->Restore();
            back->Restore();
        }

        if( hr != DDERR_WASSTILLDRAWING )
            return hr;
    }
}


};//namespace Useless
