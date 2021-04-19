#include "UselessPch.h"

#include "Useless/GUI/Widgets/NullWidget.h"
#include "Useless/GUI/Display/DisplayMan.h"

namespace Useless {

WidgetPainter::WidgetPainter(): _display(0), _widget( NullWidget::InstancePtr() )
{}

WidgetPainter::WidgetPainter( DisplayMan &display, const Pos &p )
    : Painter(display.GetPlane(), p), _display(&display), _widget( NullWidget::InstancePtr() )
{}

WidgetPainter::WidgetPainter(const WidgetPainter &wp, const Pos &p )
    : Painter(wp,p), _display(wp._display), _widget(wp._widget)
{
    _paintArea = _widget->GetBoundingRect();
    if ( Widget *parent = _widget->GetParent() )
    {
        _paintArea &= parent->GetClientRect(_widget) - _widget->GetPosition();
    }
    _paintArea += GetOffset();
}

WidgetPainter::WidgetPainter(const WidgetPainter &wp, Widget &w )
    : Painter(wp, w.GetPosition() ), _display(wp._display), _widget(&w)
{
    _paintArea = _widget->GetBoundingRect();
    if ( Widget *parent = _widget->GetParent() )
    {
        _paintArea &= parent->GetClientRect(_widget) - _widget->GetPosition();
    }
    _paintArea += GetOffset();
}

WidgetPainter& WidgetPainter::operator =( const WidgetPainter &wp )
{
    Painter::operator=(wp);
    _widget = wp._widget;
    _display = wp._display;
    return *this;
}

ImageBuffer WidgetPainter::GetImageRect( const Rect &area ) const
{
    assert(_display);
    return _display->GetImageRect( area + GetOffset() );
}

bool WidgetPainter::Intersect()
{
    //return true;

    Rect dest_rect = _widget->GetBoundingRect();
        dest_rect &= _widget->GetParent()->GetClientRect(_widget) - _widget->GetPosition();

    return Painter::Intersect( dest_rect );
}

void WidgetPainter::AllowFullArea()
{
    _display->SetClipRects( _display->GetDirtyRegion() );
}


};//namespace Useless
