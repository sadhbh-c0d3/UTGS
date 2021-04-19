#include "UselessPch.h"

#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/GUI/Widgets/NullWidget.h"
#include "Useless/GUI/WidgetEnvironment.h"

namespace Useless {

BridgeWidget::BridgeWidget(int w, int h, Widget* widget): _widget(widget)
{
    AttachWidget(widget);
    SetDimensions( w, h);
}

BridgeWidget::~BridgeWidget()
{
    //_widget->SetParent(NULL); W/A prevent pure virtual fcn calls
    delete _widget;
}

void BridgeWidget::AttachWidget( Widget* widget, int x, int y)
{
    if (_widget) 
    {
        _widget->SetParent(NULL);
        _widget->SetEnvironment(0);
        if ( GetEnvironment() ) GetEnvironment()->GetTrashCan().Trash(_widget);
        else delete _widget;
    }
    _widget = widget ? widget : new NullWidget;
    if (_widget)
    {
        _widget->SetEnvironment( GetChildrenEnvironment() );
        _widget->SetPosition( Pos(x,y) );
        _widget->SetParent( this );
        if (!!*GetParent()) { _widget->SetDirty(); }
    }
}

Widget* BridgeWidget::DetachWidget()
{
    _widget->SetParent(NULL);
    Widget *widget = _widget;
    _widget = new NullWidget;
    return widget;
}

void BridgeWidget::Hug()
{
    Widget::Fill( _widget->GetBoundingRect() + _widget->GetPosition() );
}


void BridgeWidget::Resize(int w, int h)
{
    SetDimensions( w, h );
}


};//namespace Useless
