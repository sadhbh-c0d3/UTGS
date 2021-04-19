#ifndef __INCLUDED__USELESS__MEM_SURFACE_H__
#define __INCLUDED__USELESS__MEM_SURFACE_H__

#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/detail/MemorySurface.h"

namespace Useless {

/*! Surface interface to MemorySurface
*/
class USELESS_API MemSurface : public LockableSurface, public MemorySurface
{
public:
    MemSurface();
    MemSurface( Surf::Properties &properties );
    virtual ~MemSurface();

    virtual Surface* CreateSurface( Surf::Properties &properties ) const;

    virtual bool IsValidBlitSource( const Surface &surf ) const { assert(0); return false; }

    virtual bool Lock  ( int channel_format, Surf::Ptr *srf_ptr );
    virtual void Unlock( int channel_format );

    virtual void Blit( int x, int y,                           // where to blit
                       const Surface &src,                     // what is to be blit
                       const Useless::Rect &src_rect,             // where from
                       const Surf::BlitFX &fx = Surf::BlitFX() // what effect should be applied
                      );

    virtual void Clear( unsigned long color,
                        const Useless::Rect &drc
                      );

    virtual  int GetWidth     () const { return _width; }
    virtual  int GetHeight    () const { return _height; }

    virtual bool IsColorKeyed () const { return false; }
    virtual bool SetColorKey  ( unsigned long color ) { return false; }
    virtual void SetNoColorKey() {}

    virtual void GetProperties( Surf::Properties *prop ) const;
    virtual bool SetProperties( const Surf::Properties &prop );

    virtual bool     SetClipper( const RectList &region );
    virtual RectList GetClipper( const Rect &crop = Rect() ) const;
    
    virtual void SetConstantAlpha( int alpha );
    virtual int  GetConstantAlpha() const;

    virtual bool operator==( const Surface& s );
    virtual bool operator!=( const Surface& s );
};

};//namespace Useless
#endif//__INCLUDED__USELESS__MEM_SURFACE_H__
