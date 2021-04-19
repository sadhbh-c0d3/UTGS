#ifndef __INCLUDED_USELESS_DDSURFACE__H__
#define __INCLUDED_USELESS_DDSURFACE__H__

#include "Useless/Graphic/Rect.h"
#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/detail/SurfaceStructs.h"
#include "Useless/Util/PreInitializer.h"
#include "Useless/Graphic/Device/DDraw/DDClipper.h"
#include "Useless/Graphic/detail/MemorySurface.h"
#include "Useless/System/w32/COMObject.h"

#include <ddraw.h>
#pragma comment (lib,"ddraw.lib")

namespace Useless {

class DDSurface;

/*! \defgroup gDDraw Direct Draw 
 *  \ingroup gGfxAPI
 */

typedef IDirectDraw7 * DDraw7Ptr; //not COMObject couse guid for IDirectDraw7 not available
typedef COMObject<IDirectDrawSurface7> DDrawSurface7;

/*! \ingroup gDDraw
 *  Class specialized for DirecDraw
 *
 *  Uses DirectDraw to create any interface of DirectDraw,
 *  such as IDirectDrawSurface, IDirectDrawClipper
 */
class DDCreator
{
public:
    DDCreator();
    virtual ~DDCreator();
    
    int CreateFullScreen( HWND hWnd, int width, int height, int bpp);
    int CreateWindowed( HWND hWnd, int width, int height);

            void    DestroyObjects();
       DDSurface   *GetFrontBuffer() const { return _p_front; }
       DDSurface   *GetBackBuffer()  const { return _p_back;  }
            bool    IsWindowed()     const { return _windowed;}
             int    InitClipper();
             int    UpdateBounds();
             int    Present(bool copy=false);

    static DDraw7Ptr GetDirectDraw() { return _p_ddraw; }

    static unsigned int _blitter_status;
    static unsigned int _blitter_flags;
    static unsigned int _flipper_status;
    static unsigned int _flipper_flags;

private:
            HWND     _hWnd;
            RECT     _rcWindow;
       DDSurface    *_p_front;
       DDSurface    *_p_back;
            bool     _windowed;

    static DDraw7Ptr _p_ddraw;
    static void Init();
    static void Destroy();
    friend class PreInitializer<DDCreator>;
};

#ifndef SUPPRESS_DIRECT_DRAW_INITIALIZE
static PreInitializer<DDCreator> _init_ddraw_object;
#endif

/*! \ingroup gDDraw
 *  Class specialized for DirecDraw
 *
 *  Creates IDirectDrawSurface7 interface.
 *  Gives Useless compatibile interface.
 */
class DDSurface : public LockableSurface
{
public:

    DDSurface();
    DDSurface(const DDSurface&);
    DDSurface(const Surf::Properties &prop);
    DDSurface(const DDrawSurface7 &dd_surface);
    virtual ~DDSurface();

    virtual Surface* CreateSurface( Surf::Properties &properties ) const;

    virtual bool  Lock  ( int channel_format, Surf::Ptr *surf_ptr );
    virtual void  Unlock( int channel_format );
    
    virtual bool  IsValidBlitSource( const Surface &surface ) const;

    bool IsValid() const;

    virtual void  Clear( unsigned long color, const Useless::Rect &drc );
    virtual void  Blit( int x, int y, const Surface &src, const Useless::Rect &rect, const Surf::BlitFX &fx );

    virtual  int  GetWidth()  const { return _properties.width; }
    virtual  int  GetHeight() const { return _properties.height; }

    virtual void  GetProperties( Surf::Properties *prop ) const;
    virtual bool  SetProperties( const Surf::Properties &prop );

    virtual bool operator==( const Surface& s );
    virtual bool operator!=( const Surface& s );

    virtual bool     SetClipper( const RectList &region );
    virtual RectList GetClipper( const Rect &crop = Rect() ) const;
    
    virtual void SetConstantAlpha( int alpha ) {}
    virtual int  GetConstantAlpha() const { return 255; }

    virtual bool  IsColorKeyed()   const  { return  _properties.has_color_key; }
    virtual bool  SetColorKey(unsigned long color);
    virtual void  SetNoColorKey() { _properties.has_color_key = false; }


    // Strict direct draw stuff
    const DDrawSurface7& GetDDrawSurface() const { return  _dd_surface; }
    LPDDSURFACEDESC2     GetSurfaceDesc()  const { return const_cast<LPDDSURFACEDESC2>(&_dds_desc); }
    LPDIRECTDRAWCLIPPER  GetDDrawClipper() const;

    int  DrawText( HFONT hFont,
                   TCHAR* strText,
                   DWORD dwOriginX,
                   DWORD dwOriginY,
                   COLORREF crBackground,
                   COLORREF crForeground,
                   DWORD dwBkMode = TRANSPARENT );

    bool  SetClipper( const DDClipper &clipper );
    const DDClipper &GetDDClipper() const { return _clipper; }

    /* Cache software operations */
    bool Enter_SW_Mode();
    void Leave_SW_Mode();
    bool IsIn_SW_Mode() const { return !!_data_ptr.ptr; }

private:
    MemorySurface        *_software_surface;
    DDrawSurface7         _dd_surface;

    Surf::Properties      _properties;
    DDSURFACEDESC2        _dds_desc;
    DDClipper             _clipper;

    Surf::Ptr _data_ptr;

    void  Create( const Surf::Properties &prop );
    void  Release();

    void HW_Blit( const Rect &dest_rect,
                  const DDSurface &source,
                  const Rect &source_rect,
                  const Surf::BlitFX &fx = Surf::BlitFX() );

    void SW_Blit( const Rect &dest_rect,
                  const DDSurface &source,
                  const Rect &source_rect,
                  const Surf::BlitFX &fx = Surf::BlitFX() );


    /* DDSurface is non-assignable */
    DDSurface& operator =(const DDSurface&);
};

///////////////////////////////////////////////////////////////////////////

};//namespace Useless
#endif//__INCLUDED_USELESS_DDSURFACE__H__
