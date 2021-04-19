#include "UselessPch.h"

#include "Useless/GUI/Widgets/FacedWidget.h"

namespace Useless {

#pragma warning(disable:4355)

FacedWidget::FacedWidget( const SkinMan &skinman ): _skin_manager( this, skinman )
{
    SetDimensions( _skin_manager.GetWidth(), _skin_manager.GetHeight() );
}

int FacedWidget::GetState () const
{
    return _skin_manager.GetState();
}

bool FacedWidget::SetState (int state)
{
    _skin_manager.Select(state);
    SetDimensions( _skin_manager.GetWidth(), _skin_manager.GetHeight() );
    return true;
}
   
bool FacedWidget::HasState(int state) const
{
    return _skin_manager.Get().Exists(state);
}

void FacedWidget::Repaint( const WidgetPainter &painter )
{
    _skin_manager.Paint( painter, Pos(), GetBoundingRect() );
}

void FacedWidget::Resize( int w, int h)
{
    _skin_manager.Get().SetSizeAll( Dim2i(w,h) );
    SetDimensions( _skin_manager.GetWidth(), _skin_manager.GetHeight() );
}

};//namespace Useless

