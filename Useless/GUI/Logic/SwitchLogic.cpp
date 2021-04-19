#include "UselessPch.h"

#include "Useless/GUI/Logic/SwitchLogic.h"

namespace Useless {

namespace {
enum 
{
    SWITCHED = DisplayState::MARKED,
    HIGHLITEN = DisplayState::HIGHLIGHTEN,
    DISABLED = DisplayState::DISABLED,
    SWITCHED_HIGHLITEN = SWITCHED | HIGHLITEN,
    SWITCHED_DISABLED = SWITCHED | DISABLED 
};
};//namespace

SwitchLogic::SwitchLogic()
{
    Tie2Signal( OnPress[0]  , this, &SwitchLogic::Press );
    Tie2Signal( OnMoveOver,   this, &SwitchLogic::Highlite );
    Tie2Signal( OnMoveOutside, this,&SwitchLogic::Unhighlite );
    Tie2Signal( OnActivate,    this,&SwitchLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

bool SwitchLogic::IsDisabled() const
{
    return GetStateA()==DISABLED || GetStateA()==SWITCHED_DISABLED;
}

bool SwitchLogic::IsSwitched() const
{
    return GetStateA()==SWITCHED || GetStateA()==SWITCHED_HIGHLITEN || GetStateA()==SWITCHED_DISABLED;
}

int SwitchLogic::GetSelected() const
{
    return IsSwitched();
}

void SwitchLogic::SetSelected(int s)
{
    if ( (bool)s!=IsSwitched() )
    {
        if ( IsActive() )
        {
            if (s) { SetState( GetDP().CpSet(SWITCHED) ); }
            else   { SetState( GetDP().CpClr(SWITCHED) ); }
        }
        else
        {
            if (s) { SetStateA(SWITCHED_DISABLED); }
            else   { SetStateA(DISABLED); }
        }
        OnChange.Send_1( (bool)s );
    }
}


void SwitchLogic::Press()
{
     if ( GetDP().Get(SWITCHED) ) { SetState( GetDP().CpClr(SWITCHED) ); OnChange.Send_1(false); }
     else                         { SetState( GetDP().CpSet(SWITCHED) ); OnChange.Send_1(true); }
}


void SwitchLogic::Highlite()
{
    if (GetStateA()==SWITCHED ) { SetStateA(SWITCHED_HIGHLITEN); } else 
    if (GetStateA()==NORMAL )   { SetStateA(HIGHLITEN); }
}

void SwitchLogic::Unhighlite()
{
    if (GetStateA()==HIGHLITEN )          { SetStateA(NORMAL);   } else 
    if (GetStateA()==SWITCHED_HIGHLITEN ) { SetStateA(SWITCHED); }
}

void SwitchLogic::Enable(bool t)
{
    if (!t)
    {
        if ( GetDP().Get(SWITCHED) ) { SetStateA(SWITCHED_DISABLED); }
        else { SetStateA(DISABLED); }
    }
    else
    {
        if (GetDP().Get(SWITCHED)) { SetStateA(SWITCHED); }
        else { SetStateA(NORMAL); }
    }
}



};//namespace Useless
