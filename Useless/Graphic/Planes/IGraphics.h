#ifndef __INCLUDED_IGRAPHICS_H__
#define __INCLUDED_IGRAPHICS_H__

/*
**  $Id: IGraphics.h,v 1.14 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      IGraphics
**
**  PURPOSE
**      Abstract class - source graphics surfaces for blits
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/RectList.h"
#include "Useless/System/Types.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {
/*! \defgroup GfxPlanes Image Plane Level
 *  \ingroup Graphics
 */

class Surface;
class OGraphics;

/*! \ingroup GfxPlanes
 *  Abstract for source graphics.
 */
class USELESS_API IGraphics
{
public:
    virtual ~IGraphics() {}


    //! REQUIRED pure virtual methods
    //@{
    
    virtual SPointer<IGraphics> //! Copy image
    Copy                ()                 const = 0;
    
    virtual SPointer<IGraphics> //! Copy sub-area of image
    Copy                (const Rect &rect) const = 0;

    virtual void //! Initialize Surface from Surface
    Cooperate           ( const Surface& surface ) = 0;

    virtual void //! Blit this source image to out buffer
    BlitTo              ( int x, int y, OGraphics &output);
    
    //! Blit several parts of this image onto output buffer
    // in several positions
    virtual void 
    MultiBlitTo         (   OGraphics       & destination_graphics,
                            const PointList & destination_positions,
                            const RectList  & source_areas );

    
    virtual const Surface * //! Get source image data
    GetSurface          ()  const = 0;
    
    virtual const Rect & //! Get source sub-image rectangle
    GetRect             ()  const = 0;

    //@}
    //! OPTIONAL virtual methods
    //@{
    
    virtual float //! Get frame rate if it is an animation
    GetFrameRate        ()  const { return 0.0;}
    
    virtual bool //! Is it solid rectangle (no transparent areas)
    IsSolid             ()  const { return HasColorKey();}
    
    virtual bool //! Is transparency created by color key
    HasColorKey         ()  const { return false; }
    
    virtual NormalPixel //! Get transparent color
    GetColorKey         ()  const { return 0L; }


    int
    GetWidth        ()  const   { return GetRect().GetW(); }
    int
    GetHeight       ()  const   { return GetRect().GetH(); }
    
    //! Get destination dimensions
    virtual int
    GetDestWidth    ()  const   { return GetRect().GetW(); }
    virtual int
    GetDestHeight   ()  const   { return GetRect().GetH(); }
    //@}
};

};// namespace Useless

#endif //__INCLUDED_IGRAPHICS_H__
