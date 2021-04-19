#include "UselessPch.h"

#include "Useless/GUI/Widgets/LoomWidget.h"

namespace Useless {

LoomWidget::LoomWidget( Technique technique, double loom_duration ): 
    _loom_duration(loom_duration), _technique(technique), _min_visible_pixels(0),
    _running(false), _loom_mode(false), _stage(0)
{
    _one_more_dirty = 0;
}

LoomWidget::~LoomWidget()
{
}

void LoomWidget::Repaint( const WidgetPainter &painter )
{
    BridgeWidget::Repaint( painter );

    Timer now;
    double delta_time = now - _loom_time; _loom_time = now;
    double d_stage = delta_time /_loom_duration;

    //if (_one_more_dirty) { Widget::SetDirty(); _one_more_dirty=false; } // Workaround

    if ( _loom_mode && _stage<1 && _running )        
    { 
        _stage += d_stage; if (_stage>1) { _stage=1; _running=false; _one_more_dirty=true; OnLoomStop.Send(); }
        UpdateStage();
    }
    else if ( !_loom_mode && 0<_stage && _running )
    { 
        _stage -= d_stage; if (_stage<0) { _stage=0; _running=false; _one_more_dirty=true; OnVanishStop.Send(); }
        UpdateStage();
    }
}

void LoomWidget::UpdateStage()
{
    Pos dir;
    double full;

    double w = BridgeWidget::GetWidth();
    double h = BridgeWidget::GetHeight();

    switch(_technique)
    {
        case SLIDE_LEFT:
            dir = Pos(1,0);
            full = w;
        break;

        case SLIDE_RIGHT:
            dir = Pos(-1,0);
            full = w;
        break;

        case SLIDE_UP:
            dir = Pos(0,1);
            full = h;
        break;

        case SLIDE_DOWN:
            dir = Pos(0,-1);
            full = h;
        break;
    }
    
    int visible_pixels = full * _stage;
    double begin = full - _min_visible_pixels;

    if ( visible_pixels >= _min_visible_pixels )
    {
        
        GetAttached()->SetPosition( dir * full*(1-_stage) );
    }
    else
    {
        _stage = 1 - begin/full;
        GetAttached()->SetPosition( dir*begin );
        if (!_loom_mode) { _running=false; _one_more_dirty=true; OnVanishStop.Send(); }
    }
}

void LoomWidget::AttachWidget( Widget *widget, int x, int y)
{
    BridgeWidget::AttachWidget( widget, x ,y );
    UpdateSize();
    _on_resize = widget->OnResize.TieVoid( this, &LoomWidget::UpdateSize );
}

Widget* LoomWidget::DetachWidget()
{
    _on_resize.Untie();
    BridgeWidget::Resize(0,0);
    return BridgeWidget::DetachWidget();
}

void LoomWidget::LoomUp()
{
    _loom_mode = true;
      _running = true;
    _loom_time = Timer();
    Widget::SetDirty();
}

void LoomWidget::Vanish()
{
    _loom_mode = false;
      _running = true;
    _loom_time = Timer();
    Widget::SetDirty();
}

void LoomWidget::SetMinVisible( int min_visible_pixels )
{
    _min_visible_pixels = min_visible_pixels;
    UpdateStage();
}

void LoomWidget::UpdateSize()
{
    Resize( GetAttached()->GetWidth(), GetAttached()->GetHeight() );
    UpdateStage();
}

bool LoomWidget::IsUp() const
{
    return _loom_mode;
}

void LoomWidget::LoomVanish()
{
    if (IsUp()) { Vanish(); }
    else        { LoomUp(); }
}


bool LoomWidget::IsOver(const Pos &pos) const
{
    Pos position = GetAttached()->GetPosition();
    return Rect( position.x, position.y,GetWidth(), GetHeight() ).IsInside(pos.x, pos.y);
}

};//namespace Useless
