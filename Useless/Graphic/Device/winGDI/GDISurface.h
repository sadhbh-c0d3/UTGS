#ifndef __INCLUDED_USELESS_GDI_SURFACE_H__
#define __INCLUDED_USELESS_GDI_SURFACE_H__

#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/Device/winGDI/GDISurfacePainter.h"

#pragma comment(lib,"Msimg32.lib") // defines: AlphaBlend() & TransparentBlt()

namespace Useless {

class USELESS_API GDISurface : public Surface
{
public:
    GDISurface();
    GDISurface( const Surf::Properties &properties, HWND hwnd );
    GDISurface( const Surf::Properties &properties, HWND hwnd, int resourceName );
    virtual ~GDISurface();
    void Create( const Surf::Properties &properties, HWND hwnd );
    
    ////////////////////////////////////
    //  Create Surface based on same API
    //  
    virtual Surface* CreateSurface( Surf::Properties &properties ) const;

    virtual SPointer< SurfacePainter > CreatePainter()
    {
        return (new GDISurfacePainter());
    }

    ////////////////////////////////////
    //  Validation methods
    //
    virtual bool IsValidBlitSource( const Surface &surface ) const;

    ////////////////////////////////////
    //  Pixel transfer methods
    //
    virtual SPointer<PixelTransfer> CreateReader( int channel_format=COLOR ) const { assert(0); return 0;}
    virtual SPointer<PixelTransfer> CreateWriter( int channel_format=COLOR );

    ////////////////////////////////////
    //  Blit methods
    //
    virtual void Blit( int x, int y,                           // where to blit
                       const Surface &src,                     // what is to be blit
                       const Useless::Rect &src_rect,             // where from
                       const Surf::BlitFX &fx = Surf::BlitFX() // what effect should be applied
                      );

    virtual void Clear( unsigned long color,
                        const Useless::Rect &drc
                      );

    ////////////////////////////////////
    //  Property methods
    //
    virtual  int GetWidth     ()                         const { return _properties.width;}
    virtual  int GetHeight    ()                         const { return _properties.height;}
    virtual bool IsColorKeyed ()                         const;
    virtual bool SetColorKey  (unsigned long color);
    virtual void SetNoColorKey();
    virtual void GetProperties( Surf::Properties *prop ) const { *prop=_properties; }
    virtual bool SetProperties( const Surf::Properties &prop ) { assert(0); return 0; }

    ////////////////////////////////////
    //  Clipper methods
    //
    virtual bool     SetClipper( const RectList &region );
    virtual RectList GetClipper( const Rect &crop ) const;
    
    /////////////////////////////////////
    //  Constant alpha methods
    //
    virtual void SetConstantAlpha( int alpha );
    virtual int  GetConstantAlpha() const;

    /////////////////////////////////////
    //  Equality methods
    //
    virtual bool operator==( const Surface& s );
    virtual bool operator!=( const Surface& s );

private:
    Surf::Properties _properties;
    RectList         _clipper;
    int     _constantAlpha;
    HWND    _hwnd;// handle to window
    HDC     _hdc; // handle to device context
    HBITMAP _hbmp;// handle to bitmap
    void   *_p_data;// initialized only for surfaces with alpha channel

    friend class GDIScreen;
    void SubBlit( const Pos&, const GDISurface&, const Rect&, const Surf::BlitFX& );
};

};//namespace Useless
#endif//__INCLUDED_USELESS_GDI_SURFACE_H__
