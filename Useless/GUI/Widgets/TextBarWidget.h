#ifndef __INCLUDED_USELESS_TEXT_BAR_WIDGET_H__
#define __INCLUDED_USELESS_TEXT_BAR_WIDGET_H__

#include "Useless/GUI/Widgets/CaptionTrigger.h"

namespace Useless {

//USES
    class TextBarStyle;

class USELESS_API TextBarWidget : public CaptionTrigger
{
public:
    TextBarWidget( const TextBarStyle &style, const Text &title="...");
    TextBarWidget( int w, int h, const SkinMan &skin, const Font &title_font,const Text &title="..." );
    virtual ~TextBarWidget();

    void SetTitle(const Text &title) { CaptionTrigger::SetCaption(title);  }
    Text GetTitle() const            { return CaptionTrigger::GetCaption();}

private:
    static CaptionStyle ConvertStyle(const TextBarStyle &style);
    static CaptionStyle ConvertStyle( int w, int h, const SkinMan &skin, const Font &title_font );

};

class TextBarStyle
{
public:
    TextBarStyle( int w, int h, int x, const SkinMan &skins, const Font &font, const Font &font_hilite)
                : w(w), h(h), x_offset(x), skinman(skins), font(font), font_hilite(font_hilite) {}

          int      w;
          int      h;
          int      x_offset; // -1 = align center, y is allways centered
    const SkinMan &skinman;
    const Font    &font;
    const Font    &font_hilite;
};

inline void __obsolete_text_bar() { STATIC_WARNING(USING_OBSOLETE_TEXT_BAR); }

};//namespace Useless
#endif//__INCLUDED_USELESS_TEXT_BAR_WIDGET_H__
