#include "UselessPch.h"

#include "Useless/GUI/Relations/AppearRelation.h"

#include "Useless/ErrorConfig.h"

namespace Useless {

AppearRelation::AppearRelation( Widget &widget, bool input_eager, bool use_hard_appear): 
    _input_eager(input_eager), _pioneer(&widget), _use_hard_appear(use_hard_appear)
{
    if (use_hard_appear)
    {
        CompoundWidget *container = CompoundWidget::Ancestor( _pioneer, _widget);
        _bridge = dynamic_cast<BridgeWidget*>(container);
        _hub    = dynamic_cast<HubWidget*>(container);
        if ( !_bridge && !_hub )
        { 
            APP_ERROR( Error("Container must be one of HubWidget or BridgeWidget"), "AppearRelation"); 
        }
    }
    else
    {
        _bridge = NULL;
        _hub = NULL;
        _widget = &widget;
    }

    _appeared = true;
}

AppearRelation::AppearRelation( CompoundWidget &container, Widget &widget, bool input_eager): 
        _pioneer(&widget), _widget(&widget), _input_eager(input_eager), _use_hard_appear(true)
{
    _bridge = dynamic_cast<BridgeWidget*>(&container);
    _hub    = dynamic_cast<HubWidget*>(&container);
    if ( !_bridge && !_hub )
    { 
        APP_ERROR( Error("Container must be one of HubWidget or BridgeWidget"), "AppearRelation"); 
    }

    _appeared = container.IsChild(_widget);
}

AppearRelation::~AppearRelation()
{
    Reference< Signal::FuncID > ref_fid;
    for ( _on_appear.First(); ref_fid = _on_appear.Each(); )
    {
        ref_fid.Value().Untie();
    }
    if ( _use_hard_appear && !_appeared ) delete _widget;
}

void AppearRelation::AddSignal( Signal &signal, bool show, bool doXor)
{
    _on_appear.Insert( signal.TieBinary( this, &AppearRelation::Appear, show, doXor) );
}

void AppearRelation::Appear( bool show, bool doXor)
{
    assert( !_use_hard_appear || !_hub || !_bridge );
    CompoundWidget *container;
    if (_hub) { container = _hub; }  else { container = _bridge; }
    if (doXor) { show = !_appeared; }
    bool must_show = false;

    if ( _use_hard_appear )
    {
        if (show && !container->IsChild(_widget) )
        {
            must_show = true;
        }
        else if ( !show && container->IsChild(_widget) )
        { 
            _position = _widget->GetPosition(); 
            _z = (_hub)? _hub->GetChildZIndex( _widget) : 0;
        }
        else
        {
            APP_WARNING( Error("Widget is allready %s", show ? "shown" : "hidden"), "AppearRelation");
            return;
        }

        if ( _bridge )
        {
            if (!show) { _bridge->DetachWidget(); }
            else { _bridge->AttachWidget(_widget, _position.x, _position.y); }
        }
        else // if ( _hub )
        {
            if (!show) { _hub->Retrieve(_widget); }
            else { _hub->Insert(_widget, _position.x, _position.y, _z); }
        }
        if ( must_show && _input_eager )
        {
            _widget->InputIncEager();
        }
    }
    else
    {
        if ( !show && _widget->IsVisible() )
        {
            _widget->Hide(); if ( _input_eager ) { _widget->InputDecEager(); }
        }
        else if ( show && !_widget->IsVisible() )
        {
            _widget->Show(); if ( _input_eager ) { _widget->InputIncEager(); }
        }
         
    }

    _appeared = show;
    _appeared ? OnShow.Schedule() : OnHide.Schedule();
}


};//namespace Useless

