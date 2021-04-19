#ifndef __INCLUDED_USELESS_DISPLAY_MAN_H__
#define __INCLUDED_USELESS_DISPLAY_MAN_H__

/*
 *    $Id: DisplayMan.h,v 1.8 2002/09/13 16:22:11 koolas Exp $
 *
 *    NAME
 *        DisplayMan
 *
 *    PURPOSE
 *        Abstract class for Screen and Offscreen manager 
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/Graphic/RectList.h"
#include "Useless/Graphic/Planes/OGraphics.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"

namespace Useless {

////////////////////////////////////////////////////////////
//  CLASS: DisplayMan
//
/*! \defgroup gDisplay Display Management
 *  \ingroup gGUI
 */

/*! \ingroup Display
 *  Abstract class for display management
 */
class USELESS_API DisplayMan
{
public:
    virtual ~DisplayMan() {}

    virtual OGraphics&  GetPlane    ( const Rect &rect = Rect() ) = 0; //!< Returns drawing area (may use locks)
    virtual ImageBuffer GetImageRect( const Rect &area)           = 0; //!< Returns data inside Rect
    virtual RectList    GetDirtyRegion()                    const = 0; //!< Returns full list of dirty Rects
    virtual RectList    GetClipRects()                      const = 0; //!< Returns current clip-region
    virtual void        SetClipRects( const RectList& rect_list)  = 0; //!< Set clip-region
    virtual bool        Intersect   ( const Rect &rect)           = 0; //!< Crop clip-region with Rect
};

};// namespace Useless


#endif //__INCLUDED_USELESS_DISPLAY_MAN_H__
