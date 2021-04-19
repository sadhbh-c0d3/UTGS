#ifndef __INCLUDED_SURFACE_H__
#define __INCLUDED_SURFACE_H__

/*
**  $Id: Surface.h,v 1.18 2004/07/13 10:14:34 koolas Exp $
**
**  NAME
**      Surface (DDraw version)
**
**  PURPOSE
**      Surfaces for DDraw blits
**
**  NOTE
**      You should not instantiate this class manualy.
**      Use Image, Screen or ImageBuffer class instead.
**      Allways remember the number of references to it.
**      Use rather Release than delete
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Rect.h"
#include "Useless/Graphic/RectList.h"
#include "Useless/Graphic/Planes/IGraphics.h"
//#include "Useless/Graphic/Planes/SurfacePainter.h"
#include "Useless/Functional/Signal.h"
#include "Useless/Graphic/detail/SurfaceStructs.h"
#include "Useless/Graphic/Transfer/PixelTransfer.h"

namespace Useless {
    class SurfacePainter;

/*! \ingroup gGfxAPI
 *  Class attended to use with DDSurface DDScreen
 *  
 *  Class may act in future as facade between User program
 *  and several low level API's
 */
class USELESS_API Surface
{
public:
    Surface();
    virtual ~Surface();

    ////////////////////////////////////
    //  Create Surface based on same API
    //  
    virtual Surface* CreateSurface( Surf::Properties &properties ) const = 0;

    ////////////////////////////////////
    //  Validation methods
    //
    virtual bool IsValidBlitSource( const Surface &surface ) const = 0;

    ////////////////////////////////////
    //  Pixel transfer methods
    //
    enum ChannelFormat { COLOR, ALPHA, DEPTH };
    
    virtual SPointer< PixelTransfer >
        CreateReader( int channel_format=COLOR ) const = 0;
    
    virtual SPointer< PixelTransfer >
        CreateWriter( int channel_format=COLOR ) = 0;
    
    virtual SPointer< SurfacePainter >
        CreatePainter() = 0;

    ////////////////////////////////////
    //  Blit methods
    //
    virtual void Blit( int x, int y,                           // where to blit
                       const Surface &src,                     // what is to be blit
                       const Useless::Rect &src_rect,             // where from
                       const Surf::BlitFX &fx = Surf::BlitFX() // what effect should be applied
                      ) = 0;

    virtual void MultiBlit( const Surface &sur,
                            const PointList &p,
                            const RectList& r,
                            const Surf::BlitFX &fx = Surf::BlitFX() );

    virtual void Clear( unsigned long color,
                        const Useless::Rect &drc
                      ) = 0;

    ////////////////////////////////////
    //  Property methods
    //
    virtual  int GetWidth     ()                         const = 0;
    virtual  int GetHeight    ()                         const = 0;
    virtual bool IsColorKeyed ()                         const = 0;
    virtual bool SetColorKey  (unsigned long color)            = 0;
    virtual void SetNoColorKey()                               = 0;
    virtual void GetProperties( Surf::Properties *prop ) const = 0;
    virtual bool SetProperties( const Surf::Properties &prop ) = 0;

    ////////////////////////////////////
    //  Clipper methods
    //
    virtual bool     SetClipper( const RectList &region ) = 0;
    virtual RectList GetClipper( const Rect &crop = Rect() ) const = 0;
    
    
    /////////////////////////////////////
    //  Constant alpha methods
    //
    virtual void SetConstantAlpha( int alpha ) = 0;
    virtual int  GetConstantAlpha() const = 0;

    //  Get extension function
    virtual void * GetExtFun( const std::string & name ) { return 0; }

    /////////////////////////////////////
    //  Equality methods
    //
    virtual bool operator==( const Surface& s ) = 0;
    virtual bool operator!=( const Surface& s ) = 0;
};

/*\ingroup gGfxAPI
 *  LockableSurface is Surface that might be locked
 */
class USELESS_API LockableSurface : public Surface
{
public:
    /////////////////////////////////////
    //  Direct access methods
    //
    virtual bool Lock  ( int channel_format, Surf::Ptr *srf_ptr ) = 0;
    virtual void Unlock( int channel_format = COLOR) = 0;

    /////////////////////////////////////
    //  Internally overloaded methods
    //
    virtual SPointer<PixelTransfer> CreateReader( int channel_format=COLOR ) const;
    virtual SPointer<PixelTransfer> CreateWriter( int channel_format=COLOR );
    virtual SPointer<SurfacePainter> CreatePainter();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////

};// namespace Useless
#endif  //__INCLUDED_SURFACE_H__
 
