#include "UselessPch.h"

#include "Useless/GUI/Widgets/MenuWidget.h"

namespace Useless {

void MenuWidget::Init()
{
    Tie2Signal( _option_list->OnResize, this, &MenuWidget::UpdateSize );
    _option_list->SetFollowMode(ListWidget::DRAG_FOLLOW);
    _option_list->HideSelector();
}

void MenuWidget::Resize(int w, int h)
{
    SetMenuWidth(w);
}

void MenuWidget::SetMenuWidth(int w)
{
    Rect interior = GetClientRect( _option_list );
    Rect boundry  = GetBoundingRect();
    int dw = boundry.GetW() - interior.GetW();
    UpdateSize( _fixed_width = w-dw, _option_list->GetHeight() );
}

void MenuWidget::UpdateSize(int w, int h) //< w,h are list's dimentions
{
    w = ( w >  _fixed_width )? w : _fixed_width;

    Rect interior = GetClientRect( _option_list );
    Rect boundry  = GetBoundingRect();
    Rect diffrect = boundry-interior;
    Rect new_interior( interior.GetX1(), interior.GetY1(), w, h);
    Rect new_boundry = new_interior + diffrect;
    SetPosition( new_boundry.GetPos() );
    FrameWidget::Resize( new_boundry.GetW(), new_boundry.GetH() );
    _option_list->Fill(new_interior);
    _option_list->SetSelectorSize( w, _option_list->GetSelectorSize().y );
}


};//namespace Useless
