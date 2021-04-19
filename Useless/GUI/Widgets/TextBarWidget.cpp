#include "UselessPch.h"

#pragma warning(disable:4101)

#include "Useless/GUI/Widgets/TextBarWidget.h"

namespace Useless {

TextBarWidget::TextBarWidget( const TextBarStyle &style, const Text &title):
    CaptionTrigger( title, ConvertStyle(style) )
{
    Resize(style.w, style.h);
}

TextBarWidget::TextBarWidget( int w, int h, const SkinMan &skins, const Font &title_font,const Text &title ):
    CaptionTrigger( title, ConvertStyle(w,h,skins,title_font) )
{
    Resize(w,h);
}


TextBarWidget::~TextBarWidget()
{}

CaptionStyle TextBarWidget::ConvertStyle(const TextBarStyle &style)
{
    static FontSet s_fonts; 
    
    s_fonts = FontSet();

    s_fonts.Insert( DisplayState::NORMAL,      style.font);
    s_fonts.Insert( DisplayState::SELECTED,    style.font);
    s_fonts.Insert( DisplayState::HIGHLIGHTEN, style.font_hilite);
    s_fonts.Insert( DisplayState::PRESSED,     style.font_hilite);

    return CaptionStyle( style.skinman,
                         s_fonts,
                        (style.x_offset==-1)? INNER : INNER_LEFT,
                        Pos( (style.x_offset==-1)? 0 : style.x_offset, 0 ) );
}

CaptionStyle TextBarWidget::ConvertStyle( int w, int h, const SkinMan &skins, const Font &title_font )
{
    static FontSet s_fonts; 
    
    s_fonts = FontSet();
    s_fonts.Insert( TriggerLogic::NORMAL, title_font);

    return CaptionStyle( skins, s_fonts, INNER );
}

};//namespace Useless
