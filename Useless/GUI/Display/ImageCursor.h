#ifndef __INCLUDED_USELESS_IMAGE_CURSOR_H__
#define __INCLUDED_USELESS_IMAGE_CURSOR_H__

#include "Useless/GUI/Display/MouseCursor.h"
#include "Useless/Graphic/Planes/Image.h"

namespace Useless {

/*! \ingroup gDisplay
 *  Cursor shape form Image.
 */
class USELESS_API ImageCursor : public MouseCursor
{
public:
    ImageCursor() {}
    ImageCursor( const IGraphics &image, const Pos& hotspot=Pos(0,0) );
    virtual ~ImageCursor() {}

    virtual MouseCursor* Copy() const { return new ImageCursor(*this); }

    virtual void Paint(const Pos& p, OGraphics& output);
    virtual Rect GetRect() const { return Rect( 0,0,(*_sp_image).GetWidth(),(*_sp_image).GetHeight() )-_hotspot; }

private:
    SPointer<IGraphics> _sp_image;
    Pos                 _hotspot;

};


};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,MouseCursor,ImageCursor);

#endif//__INCLUDED_USELESS_IMAGE_CURSOR_H__
