#include "UselessPch.h"

#include "Useless/GUI/Relations/FillRelation.h"

namespace Useless {

FillRelation::FillRelation( Widget &widget, const Rect &padding )
{   
    assert( widget.GetParent() && "FillRelation::FillRelation(): Not in group");
    widget.Fill( Rect(), padding );
    _on_client_area_resize = widget.GetParent()->OnResize.TieBinary( &widget, &Widget::Fill, Rect(), padding );
}

FillRelation::~FillRelation()
{
    _on_client_area_resize.Untie();
}

};//namespace Useless 
