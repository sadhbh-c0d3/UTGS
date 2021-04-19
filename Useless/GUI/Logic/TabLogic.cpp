#include "UselessPch.h"

#include "Useless/GUI/Logic/TabLogic.h"

namespace Useless {

namespace {
enum 
{
    SELECTED = DisplayState::MARKED,
    HIGHLITEN = DisplayState::HIGHLIGHTEN,
    DISABLED = DisplayState::DISABLED,
    PRESSED = DisplayState::PRESSED,
    SELECTED_DISABLED = SELECTED | DISABLED 
};
};//namespace


TabLogic::TabLogic()
{
    Tie2Signal( OnClick[0]  ,  this,&TabLogic::Select );
    Tie2Signal( OnPress[0],    this,&TabLogic::Press );
    Tie2Signal( OnMoveIn[0],   this,&TabLogic::Press );
    Tie2Signal( OnMoveOut[0],  this,&TabLogic::Unpress );
    Tie2Signal( OnMoveOver,    this,&TabLogic::Highlite );
    Tie2Signal( OnMoveOutside, this,&TabLogic::Unhighlite );
    Tie2Signal( OnActivate,    this,&TabLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

bool TabLogic::IsDisabled() const
{
    return GetStateA()==DISABLED || GetStateA()==SELECTED_DISABLED;
}

bool TabLogic::IsSelected() const
{
    return GetStateA()==SELECTED || GetStateA()==SELECTED_DISABLED;
}

int TabLogic::GetSelected() const
{
    return IsSelected();
}

void TabLogic::SetSelected(int s)
{
    if ( (bool)s!=IsSelected() )
    {
        if ( IsActive() )
        {
            if (s) { SetStateA(SELECTED); }
            else   { SetStateA(NORMAL); }
        }
        else
        {
            if (s) { SetStateA(SELECTED_DISABLED); }
            else   { SetStateA(DISABLED); }
        }
        OnChange.Send_1( (bool)s );
    }
}

bool TabLogic::IsClickPossibile( const Pos &, int btn_no ) const
{
    return ( btn_no!=0 || GetStateA()==PRESSED );
}

void TabLogic::Press()
{
    if ( GetStateA()==HIGHLITEN || GetStateA()==NORMAL ) SetStateA(PRESSED);
}

void TabLogic::Unpress()
{
    if ( GetStateA()==PRESSED ) SetStateA(HIGHLITEN);
}

void TabLogic::Select()
{
    SetStateA( SELECTED );
    OnChange.Send_1(true);
}

void TabLogic::Highlite()
{
    if ( GetStateA()==NORMAL ) SetStateA(HIGHLITEN);
}

void TabLogic::Unhighlite()
{
    if ( GetStateA()==HIGHLITEN || GetStateA()==PRESSED ) SetStateA(NORMAL);
}

void TabLogic::Enable(bool t)
{
    if (!t)
    {
        if (GetStateA()==SELECTED ) { SetStateA(SELECTED_DISABLED); }
        else if (GetStateA()==NORMAL || GetStateA()==HIGHLITEN)   { SetStateA(DISABLED); }
    }
    else
    {
        if (GetStateA()==SELECTED_DISABLED) { SetStateA(SELECTED); } else 
        if (GetStateA()==DISABLED)        { SetStateA(NORMAL); }
    }
}



};//namespace Useless
