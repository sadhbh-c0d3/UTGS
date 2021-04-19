#include "UselessPch.h"

#include "Useless/GUI/Widgets/ListWidget.h"

namespace Useless {

ListWidget::ListWidget( const ListStyle2 &style )
                        : ListedTextItems( style.fonts, style.skins, style.shift )
{}

ListWidget::ListWidget( const Font &fn,
                        const Font &fh,
                        const Skin &sk,
                               int  shift
                       ): ListedTextItems( CreateFontSet(fn,fh), CreateSkinSet(sk), shift )
{}


ListWidget::ListWidget( const ListStyle &style )
 :ListedTextItems( CreateFontSet(style.normal,style.highliten), CreateSkinSet(style.selector_skin), style.shift )
{}

Text ListWidget::GetText( int item_no ) const
{
    return (item_no<0)? ListedTextItems::GetText(GetSelected())
                       :ListedTextItems::GetText(item_no);
}

FontSet ListWidget::CreateFontSet( const Font &n, const Font &hl )
{
    FontSet fonts;
    fonts.Insert( ListLogic::NORMAL,    n );
    fonts.Insert( ListLogic::HIGHLITEN, hl );
    fonts.Insert( ListLogic::SELECTED,  n );
    fonts.Insert( ListLogic::HIGHLITEN | ListLogic::SELECTED, hl );

    return fonts;
}

SkinSet ListWidget::CreateSkinSet( const Skin &sk)
{
    SkinSet skins;
    skins.Insert( ListLogic::SELECTED, sk );
    skins.Insert( ListLogic::HIGHLITEN| ListLogic::SELECTED, sk );
    skins.Insert( ListLogic::DISABLED | ListLogic::SELECTED, sk );
    return skins;
}


};//namespace Useless
