#ifndef __INCLUDED_USELESS_DDClipper_H__
#define __INCLUDED_USELESS_DDClipper_H__

#include <ddraw.h>
#include "Useless/Graphic/RectList.h"

namespace Useless {

//////////////////////////////////////////////////////////////
//  CLASS: DDClipper
//
/*! \ingroup gGfxAPI
 *  Class attended for use with DDSurface
 *
 *  DirectDraw Surface DDClipper
 */
class DDClipper
{
public:
    DDClipper();
    DDClipper( const Rect &rect );
    DDClipper( const RectList &rect_list );
    DDClipper( const DDClipper& DDClipper );

    DDClipper& operator=(const DDClipper &DDClipper);
    ~DDClipper();

    RectList GetRectangles() const;
    RectList GetRectangles(const Rect &rect) const;
    RectList GetRectangles(const RectList &rect_list) const;

    DDClipper(LPDIRECTDRAWCLIPPER pDDClipper);

private:
    LPDIRECTDRAWCLIPPER  _dd_clipper;

    friend class DDCreator;
    friend class DDSurface;
};

#ifdef DEBUG_REGION
    extern RECT g_LastRegion[1024];
    extern int g_SizeLastRegion;
#endif

};// namespace Useless
#endif// __INCLUDED_USELESS_DDClipper_H__
