#ifndef __INCLUDED__USELESS_SMALL_HINT_H__
#define __INCLUDED__USELESS_SMALL_HINT_H__

#include "Useless/GUI/Skins/FrameSkin.h"
#include "Useless/GUI/Widgets/AdvTextWidget.h"

namespace Useless {

class USELESS_API SmallHint : public AdvTextWidget
{
public:
    SmallHint( const Font &font, const Skin &skin, const Text &text, int alignment=0 );
    virtual ~SmallHint();

    virtual void Repaint(const WidgetPainter &painter);
    virtual void Resize (int w, int h);

    void FitText();

    int GetWidth() const { return Widget::GetWidth();}
    int GetHeight() const { return Widget::GetHeight();}

private:
    SPointer<Skin>  _sp_skin;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_SMALL_HINT_H__
