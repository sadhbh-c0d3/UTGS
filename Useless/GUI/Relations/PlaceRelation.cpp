#include "UselessPch.h"

#include "Useless/GUI/Relations/PlaceRelation.h"

namespace Useless {

PlaceRelation::PlaceRelation( Widget &widget, const Pos &pos, const Pos &base, const Rect &alignSize, const Pos &alignPlace )
{
    widget.Place( pos, base);
    widget.GetParent()->OnResize.TieBinary( &widget,
            (void (Widget::*)(const Pos&, const Pos&))&Widget::Place,
            pos, base);
    
    if ( alignSize.w > 0 || alignSize.h > 0 )
    {
        widget.AlignSize( alignSize, alignPlace );
        widget.GetParent()->OnResize.TieBinary( &widget, &Widget::AlignSize, alignSize, alignPlace );
    }
}

PlaceRelation::PlaceRelation( Widget &widget, const Rect &rect, const Pos &base, const Rect &alignSize, const Pos &alignPlace )
{
    widget.Place( rect, base);
    widget.GetParent()->OnResize.TieBinary( &widget,
            (void (Widget::*)(const Rect&, const Pos&))&Widget::Place,
            rect, base);

    if ( alignSize.w > 0 || alignSize.h > 0 )
    {
        widget.AlignSize( alignSize, alignPlace );
        widget.GetParent()->OnResize.TieBinary( &widget, &Widget::AlignSize, alignSize, alignPlace );
    }
}

PlaceRelation::~PlaceRelation()
{
    _on_resize.Untie();
}


};//namespace Useless
