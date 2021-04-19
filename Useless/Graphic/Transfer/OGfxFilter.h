#ifndef __INCLUDED_USELESS_OGFXFILTER_H__
#define __INCLUDED_USELESS_OGFXFILTER_H__

/*
**  $Id: OGfxFilter.h,v 1.4 2002/09/13 16:22:17 koolas Exp $
**
**  NAME
**      OGfxFilter
**
**  PURPOSE
**      Output graphics filter abstract class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/System/Types.h"

namespace Useless {
/*! \ingroup ImgIO
 *  Abstract destination for image transfer.
 */

class USELESS_API OGfxFilter
{
public:
    OGfxFilter() {}
    virtual ~OGfxFilter() {}

    virtual void SetSize( int w, int h, int d ) = 0;
    virtual void WritePalette( const Types::UBYTE *rgb, int pal_size=256 ) = 0;
    virtual void Write( const Types::UBYTE *s, int x, int y, int w, int h, int pitch ) = 0;
};


} // namespace Useless

#endif __INCLUDED_USELESS_OGFXFILTER_H__
