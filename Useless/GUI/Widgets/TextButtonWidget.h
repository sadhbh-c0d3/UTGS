#ifndef __INCLUDED_USELESS_TEXT_BUTTON_WIDGET_H__
#define __INCLUDED_USELESS_TEXT_BUTTON_WIDGET_H__

#include "Useless/GUI/Widgets/CaptionButton.h"

namespace Useless {

//USES
    class TextButtonStyle;

class USELESS_API TextButtonWidget : public CaptionButton
{
public:
    TextButtonWidget( const TextButtonStyle &style, const Text &title="...");
    TextButtonWidget( int w, int h, const SkinMan &skin, const Font &title_font,const Text &title="..." );
    virtual ~TextButtonWidget();

    void SetTitle(const Text &title) { CaptionButton::SetCaption(title);  }
    Text GetTitle() const            { return CaptionButton::GetCaption();}

private:
    static CaptionStyle ConvertStyle(const TextButtonStyle &style);
    static CaptionStyle ConvertStyle( int w, int h, const SkinMan &skin, const Font &title_font );

};

class TextButtonStyle
{
public:
    TextButtonStyle( int w,
                     int h,
                     int x,
                     Pos ap, 
                   const SkinMan &skinman,
                   const Font &font,
                   const Font &font_hilite ):
                   w(w), h(h), x_offset(x), skinman(skinman),
                   font(font), font_hilite(font_hilite), after_press(ap) {}

          int      w;
          int      h;
          int      x_offset; // -1 = align center, y is allways centered
          Pos      after_press;
    const SkinMan &skinman;
    const Font    &font;
    const Font    &font_hilite;
};

//inline void __obsolete_text_button() { STATIC_WARNING(USING_OBSOLETE_TEXT_BUTTON); }

};//namespace Useless
#endif//__INCLUDED_USELESS_TEXT_BUTTON_WIDGET_H__
