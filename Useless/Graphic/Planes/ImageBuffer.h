#ifndef __INCLUDED_IMAGE_BUFFER_H__
#define __INCLUDED_IMAGE_BUFFER_H__

/*
**  $Id: ImageBuffer.h,v 1.20 2004/07/13 10:14:33 koolas Exp $
**
**  NAME
**      Image
**
**  PURPOSE
**      Image read-only graphics for blits
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Planes/ImageBase.h"
#include "Useless/Graphic/Planes/OGraphics.h"
#include "Useless/Util/CommonReference.h"
#include "Useless/Functional/Signal.h"

namespace Useless {

//REQUIRE:
    class Screen;
    class Clipper;

//////////////////////////////////////////////////////////////////////
//  CLASS: ImageBuffer
//
/*! \ingroup GfxPlanes
 *  Acts as source graphics, which can be blited onto some dest graphics,
 *  and also gives interface to draw on it's Surface
 */
class USELESS_API ImageBuffer : public ImageBase, public OGraphics
{
public:
    // Dummy initializer
    ImageBuffer();
    
    // New buffer W x H (writeable)
    ImageBuffer( int w, int h);

    // Just copy all ImageBuffer members, and adjust rectangle
    ImageBuffer( const ImageBuffer &source, const Rect &rect );
    
    // Adopt surface for this ImageBuffer.
    ImageBuffer( Share< Surface > surface, const Rect &rect = Useless::Rect() );
   
    // Buffer containing new copy of source pixels (always copy)
    ImageBuffer( Surface *source, const Rect &rect = Rect() );
    
    // Just copy all ImageBuffer members
    ImageBuffer &operator = ( const ImageBuffer & );
    
    virtual ~ImageBuffer();
    
    virtual SPointer<IGraphics> Copy()                 const { return new ImageBuffer (*this); }
    virtual SPointer<IGraphics> Copy(const Rect &rect) const { return new ImageBuffer (*this, rect); }
    
    // Buffer containing new copy of source pixels (always copy)
    virtual SPointer<IGraphics> QueryIGraphics( const Rect &cut );

    virtual const Surface* GetSurface() const { return _sh_surface.GetTarget(); }
    virtual       Surface* GetSurface()       { return _sh_surface.GetTarget(); }

    virtual int   GetWidth  ()  const   { return _rectangle.GetW(); }
    virtual int   GetHeight () const    { return _rectangle.GetH(); }

    virtual void  Clear    ( NormalPixel color=0L, const Rect &area = Rect() );
    
    virtual void     SetClipper( const RectList &region );
    virtual RectList GetClipper( const Rect &crop = Rect() ) const;
    
    void Create( const Screen& screen );
    void Resize(int w, int h);

    bool IsValid() const { return (!!_sh_surface); }

    Signal OnPaint;

protected:
    virtual void Load() { OnPaint.Send(); }
};

};// namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,IGraphics,ImageBuffer);
REGISTER_COPIER(Useless,ImageBase,ImageBuffer);

#endif //__INCLUDED_IMAGE_BUFFER_H__
