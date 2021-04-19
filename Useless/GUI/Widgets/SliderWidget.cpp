#include "UselessPch.h"

#include "Useless/GUI/Widgets/SliderWidget.h"
#include "Useless/GUI/Skins/NullSkin.h"

namespace Useless {

SliderWidget::SliderWidget( int size, const SkinSet &skins ):
    DraggerWidget( ExtractLaneSkin(skins) ), _num_states(1)
{
    Init( size, skins );
}

SliderWidget::SliderWidget( const SliderStyle &style):
    DraggerWidget( ExtractLaneSkin(style.skins) ), _num_states(1)
{
    Init( style.size, style.skins );
}

void SliderWidget::Init( int size, const SkinSet &skins )
{
    /* Create grip
    ------------------------------------------------------------------------------*/   
    SkinSet ss_grip;
    assert( skins.Exists(GRIP) );
    ss_grip.Insert( DraggerLogic::NORMAL, skins.GetSkin(GRIP) );

    if ( skins.Exists(GRIP_PRESSED) ) 
        { ss_grip.Insert( DisplayState::PRESSED, skins.GetSkin(GRIP_PRESSED) ); }
    else if ( skins.Exists(GRIPHL) ) 
        { ss_grip.Insert( DisplayState::PRESSED, skins.GetSkin(GRIPHL) ); }
    else
        { ss_grip.Insert( DisplayState::PRESSED, skins.GetSkin(GRIP) );}

    if ( skins.Exists(GRIPHL) ) 
        { ss_grip.Insert( DisplayState::HIGHLIGHTEN, skins.GetSkin(GRIPHL) ); }

    if ( skins.Exists(GRIPDIS) ) 
        { ss_grip.Insert( DisplayState::DISABLED, skins.GetSkin(GRIPDIS) ); }
    else
        { ss_grip.Insert( DisplayState::DISABLED, NullSkin() ); }

    _grip = BridgeWidget::WAttach( new DraggerWidget(ss_grip) );

    /* Setup lane
    ------------------------------------------------------------------------------*/   
    Pos  lane_dir = skins.GetDirection(LANE)*skins.GetDirMask(LANE);
    Pos  grip_dir = skins.GetDirection(GRIP)*skins.GetDirMask(GRIP);
    bool horizontal = lane_dir.x && grip_dir.x;
    bool   vertical = lane_dir.y && grip_dir.y;
    
    assert( horizontal || vertical 
            && "SliderWidget::SliderWidget(): Incompatibile LANE and GRIP orientations");

    _horizontal = horizontal;

    if (_horizontal)
        { DraggerWidget::Resize( size, DraggerWidget::GetHeight() ); }
    else 
        { DraggerWidget::Resize( DraggerWidget::GetWidth(), size ); }

    BridgeWidget::Resize( DraggerWidget::GetWidth(), DraggerWidget::GetHeight() );

    /* Tie on signals
    ------------------------------------------------------------------------------*/
    Tie2Signal( _grip->OnDrag[0],    this, &SliderWidget::GripSlide );
    Tie2Signal( _grip->OnRelease[0], this, &SliderWidget::Snap );
    Tie2Signal( OnPress[0],   this, &SliderWidget::GotoPosition );
    Tie2Signal( OnDrag[0],    this, &SliderWidget::GotoPosition );
    Tie2Signal( OnRelease[0], this, &SliderWidget::Snap );
    Tie2Signal( OnActivate, _grip, &ActiveWidget::SetActive );

      _grip->OnPress[0].TieUnary( this, &SliderWidget::CaptureGrip, true );
    _grip->OnRelease[0].TieUnary( this, &SliderWidget::CaptureGrip, false );

    _dragging_grip = 0;
    varSelector = 0;
}

SliderWidget::~SliderWidget()
{
}

void SliderWidget::Repaint(const WidgetPainter &painter )
{
    DraggerWidget::Repaint( painter );
    BridgeWidget::Repaint( painter );
}

void SliderWidget::Resize (int w, int h)
{
    DraggerWidget::Resize( w, h);
    BridgeWidget::Resize( DraggerWidget::GetWidth(), DraggerWidget::GetHeight() );
    Snap();
}

int SliderWidget::GetSelected() const
{
    return varSelector;
}

void SliderWidget::SetSelected(int s)
{
    varSelector = s;
    Snap();
}

bool SliderWidget::Accept ( const Request& request)
{
    bool accepted = BridgeWidget::Accept(request);

    if ( !_dragging_grip || typeid(request)!=typeid(Widget::InputCursor) )
    { accepted |= ActiveWidget::Accept(request); }

    return accepted;
}

void SliderWidget::CaptureGrip( bool t)
{
    if (_dragging_grip=t) { }
    else                  { }
}

void SliderWidget::GripSlide(const Pos &pos)
{
    GotoPosition( _grip->GetPosition() + pos );
}

void SliderWidget::GotoPosition(const Pos &pos)
{
    int length   = GetLength();
    int p        = IsHorizontal() ? pos.x : pos.y;
    int gripsize = IsHorizontal() ? _grip->GetWidth() : _grip->GetHeight();
    int halfgrip = gripsize/2;
    int slide_length = length - gripsize;
    int d = slide_length / (2*_num_states);

    if (p < halfgrip)                 { p =halfgrip; }
    if (p > length-gripsize+halfgrip) { p =length-gripsize+halfgrip; }

    int x = ((p-halfgrip+d)* (_num_states-1) )/slide_length;

    SetGripPosition(p-halfgrip);

    varSelector = x;
}


void SliderWidget::Snap()
{
    int length = GetLength();
    int gripsize = IsHorizontal() ? _grip->GetWidth() : _grip->GetHeight();
    int halfgrip = gripsize/2;
    int slide_length = length - gripsize;
    int p = halfgrip + (slide_length * (int)varSelector) / (_num_states-1);

    SetGripPosition(p-halfgrip);
    OnConfirm.Send_1( GetSelected() );
}

void SliderWidget::SetGripSize(int s)
{
    IsHorizontal() ? _grip->Resize(s, _grip->GetHeight()) : _grip->Resize(_grip->GetWidth(),s);
}

void SliderWidget::SetGripPosition(int p)
{
    if ( IsHorizontal() )
    {
        int y = ( GetHeight() - _grip->GetHeight() )/2;
        _grip->SetPosition( Pos(p,y) );
    }
    else
    {
        int x = ( GetWidth() - _grip->GetWidth() )/2;
        _grip->SetPosition( Pos(x,p) );
    }
}

void SliderWidget::SetNumStates(int n)
{
    _num_states = n;
    Snap();
}

SkinSet SliderWidget::ExtractLaneSkin( const SkinSet &skins )
{
    SkinSet ss_lane;

    assert( skins.Exists(LANE) );

    ss_lane.Insert( DraggerLogic::NORMAL, skins.GetSkin(LANE) );

    if ( skins.Exists(LANE_PRESSED) )
        { ss_lane.Insert( DisplayState::PRESSED,   skins.GetSkin(LANE_PRESSED) ); }
    else if ( skins.Exists(LANEHL) )
        { ss_lane.Insert( DisplayState::PRESSED,   skins.GetSkin(LANEHL) ); }
    else
        { ss_lane.Insert( DisplayState::PRESSED,   skins.GetSkin(LANE) ); }

    if ( skins.Exists(LANEHL) )
        { ss_lane.Insert( DisplayState::HIGHLIGHTEN, skins.GetSkin(LANEHL) ); }

    if ( skins.Exists(LANEDIS) )
        { ss_lane.Insert( DisplayState::DISABLED, skins.GetSkin(LANEDIS) ); }

    return ss_lane;
}


};//namespace Useless
