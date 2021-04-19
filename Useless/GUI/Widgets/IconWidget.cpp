#include "UselessPch.h"

#include "Useless/GUI/Widgets/IconWidget.h"

namespace Useless {

#pragma warning (disable:4355)

IconWidget::IconWidget( const SkinMan &skins,
                        const SkinMan &icons,
                        int   distance  ):
    _skins( this, skins),
    _icons( this, icons),
    _distance(distance)
{
    SetDimensions( _skins.GetWidth(), _skins.GetHeight() );
}

IconWidget::IconWidget( const IconStyle &style ):
    _skins( this, style.skins), 
    _icons( this, style.icons), 
    _distance(style.distance)
{
    SetDimensions( _skins.GetWidth(), _skins.GetHeight() );
}

IconWidget::~IconWidget()
{
}


void IconWidget::Repaint( const WidgetPainter &painter )
{
    int icon_w = _icons.GetWidth();
    int icon_h = _icons.GetHeight();
    _skins.Paint( painter, Pos(), GetBoundingRect() );
    _icons.Paint( painter, Pos((GetWidth()-icon_w)/2, (GetHeight()-icon_h)/2) );
}

void IconWidget::Resize(int w, int h)
{
    _skins.Get().SetSizeAll( Dim2i(w,h) );
    SetDimensions( w, h);
}

int IconWidget::GetState () const
{
    assert( _skins.GetState() == _icons.GetState() );
    return _skins.GetState();
}

bool IconWidget::SetState (int state)
{
    _skins.Select( state );
    _icons.Select( state );
    Widget::SetDirty();
    return true;
}

bool IconWidget::HasState (int state) const
{
    return _skins.Get().Exists(state) || _icons.Get().Exists(state);
}


};//namespace Useless
