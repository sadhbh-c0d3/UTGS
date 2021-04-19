#ifndef __INCLUDED_ADV_IMAGE_BUFFER_H__
#define __INCLUDED_ADV_IMAGE_BUFFER_H__

/*
**  $Id: AdvImageBuffer.h,v 1.11 2003/01/22 18:58:13 koolas Exp $
**
**  NAME
**      AdvImageBuffer
**
**  PURPOSE
**      Advanced Image Buffer which supports cacheing for future blits
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Planes/ImageBuffer.h"
#include "Useless/Functional/DutyList.h"

namespace Useless {
/*! \ingroup GfxPlanes
 *  Advanced Image Buffer supports lazy calls of Blit, MultiBlit, Clear and SetClipper.
 *  See ImageBuffer for other details.
 */

//////////////////////////////////////////////////////////////////////
//  CLASS: AdvImageBuffer
//
class USELESS_API AdvImageBuffer : public ImageBuffer
{
public:
    AdvImageBuffer();
    AdvImageBuffer(int w, int h);
    AdvImageBuffer(const AdvImageBuffer& image);
    AdvImageBuffer(const AdvImageBuffer& image, const Rect &rect);
    virtual ~AdvImageBuffer();
    virtual SPointer<IGraphics> Copy()                 const { return new AdvImageBuffer(*this); }
    virtual SPointer<IGraphics> Copy(const Rect &rect) const { return new AdvImageBuffer(*this, rect);}

    virtual void Clear( NormalPixel color=0L, const Rect &area=Rect() );

    virtual void SetClipper( const RectList &region );
    virtual RectList GetClipper( const Rect &crop = Rect() ) const;

private:
    DutyList<AdvImageBuffer> _scheduled;
    SPointer<RectList>       _simulated_region;
   
    Signal::FuncID           _my_on_paint;
    void TieSignals();
    void Flush();
};

};// namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,IGraphics,AdvImageBuffer);
REGISTER_COPIER(Useless,ImageBase,AdvImageBuffer);
REGISTER_COPIER(Useless,ImageBuffer,AdvImageBuffer);

#endif
