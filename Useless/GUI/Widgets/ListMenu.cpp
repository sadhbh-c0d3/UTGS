#include "UselessPch.h"

#include "Useless/GUI/Widgets/ListMenu.h"
#include "Useless/GUI/Widgets/AdvListWidget.h"
#include "Useless/GUI/Misc/DataTypes.h" // DT_Text

namespace Useless {

ListMenu::ListMenu( const CaptionStyle &style, IWList *p_items, bool items_above )
    : _p_items(p_items), DropDown( style, items_above )
{
    Tie2Signals(_p_items);
}

ListMenu::ListMenu( const CaptionStyle &style1, const AdvListStyle2 &style2, bool list_above )
    : DropDown( style1, list_above )
{
    _p_items = new AdvListWidget(style2,style2,style2);
    Tie2Signals(_p_items);
}


ListMenu::~ListMenu()
{}

DataPointer ListMenu::GetSelectedData() const
{
    return new DT_Text( _p_items->Items().GetText(), _placement );
}

void ListMenu::SetMenuHeight  ( int h)
{
    if ( _p_items->AtomPtr() != _p_items->Items().AtomPtr() )
    {
        int  ih = _p_items->Items().GetHeight();
        Rect ir = _p_items->Items().GetParent()->GetClientRect( &_p_items->Items() );
        Rect lr = _p_items->Atom().GetBoundingRect();
        int  dy = lr.GetH() - ir.GetH();
        int  max_h = ih + ((dy>0)? dy:0);
        h = (h>max_h)? max_h : h;

        _p_items->Atom().Resize( _p_items->Atom().GetWidth(), h );
    }
}


};//namespace Useless
