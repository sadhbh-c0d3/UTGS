#include "UselessPch.h"

#pragma warning(disable:4101)

#include "Useless/GUI/Widgets/TextButtonWidget.h"

namespace Useless {

TextButtonWidget::TextButtonWidget( const TextButtonStyle &style, const Text &title):
    CaptionButton( title, ConvertStyle(style) )
{
    Resize(style.w, style.h);
}

TextButtonWidget::TextButtonWidget( int w, int h, const SkinMan &skins, const Font &title_font,const Text &title ):
    CaptionButton( title, ConvertStyle(w,h,skins,title_font) )
{
    Resize(w,h);
}


TextButtonWidget::~TextButtonWidget()
{
}

CaptionStyle TextButtonWidget::ConvertStyle(const TextButtonStyle &style)
{
    static FontSet s_fonts; 
    
    s_fonts = FontSet();
    s_fonts.Insert( DisplayState::NORMAL,      style.font);
    s_fonts.Insert( DisplayState::PRESSED,     style.font_hilite);
    s_fonts.Insert( DisplayState::HIGHLIGHTEN, style.font_hilite);
    s_fonts.Insert( DisplayState::DISABLED,    style.font);

    return CaptionStyle( style.skinman,
                         s_fonts,
                        (style.x_offset==-1)? INNER : INNER_LEFT,
                        Pos( (style.x_offset==-1)? 0 : style.x_offset , 0) );
}

CaptionStyle TextButtonWidget::ConvertStyle( int w, int h, const SkinMan &skins, const Font &title_font )
{
    static FontSet s_fonts; 
    
    s_fonts = FontSet();
    s_fonts.Insert( ButtonLogic::NORMAL, title_font);

    return CaptionStyle( skins, s_fonts, INNER );
}

};//namespace Useless
