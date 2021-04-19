#ifndef __INCLUDED_OGRAPHICS_H__
#define __INCLUDED_OGRAPHICS_H__

/*
**  $Id: OGraphics.h,v 1.10 2004/06/08 18:44:59 koolas Exp $
**
**  NAME
**      OGraphics
**
**  PURPOSE
**      Abstract class - blits destination graphics surfaces
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/System/Types.h"
#include "Useless/Graphic/RectList.h"
#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

// REQUIRE:
    class IGraphics;
    class Surface;

///////////////////////////////////////////////////////////////////////////////////
//  CLASS: OGraphics
//
/*! \ingroup GfxPlanes
 *  Abstract destination graphics for blits
 */
class USELESS_API OGraphics
{
public:
    virtual ~OGraphics() {}

    //! Get dimentions
    virtual int      GetWidth()  const = 0;
    virtual int      GetHeight() const = 0;

    //! Get out buffer surface
    virtual const Surface* GetSurface() const = 0;
    virtual Surface* GetSurface() = 0;
    
    //! Clear rectangular area
    virtual void Clear( NormalPixel color=0L, const Rect &area = Rect() ) = 0;

    //! Set clip region
    virtual void SetClipper( const RectList &rect_list ) = 0;

    //! Get cropped clip region
    virtual RectList GetClipper( const Rect &crop = Rect() ) const = 0;

    //! Query interface to IGraphics - NULL when N/A
    virtual SPointer<IGraphics> QueryIGraphics( const Rect &cut ) = 0;

    //! Set constant alpha factor (255:opaque, 0:transparent)
    virtual void SetConstantAlpha( int alpha );

    //! Get constant alpha factor (255:opaque, 0:transparent)
    virtual int  GetConstantAlpha() const;
};


};// namespace Useless
#endif //__INCLUDED_OGRAPHICS_H__
