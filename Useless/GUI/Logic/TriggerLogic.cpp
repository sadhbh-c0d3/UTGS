#include "UselessPch.h"

#include "Useless/GUI/Logic/TriggerLogic.h"

namespace Useless {

namespace {
enum 
{
    SELECTED = DisplayState::MARKED,
    HIGHLITEN = DisplayState::HIGHLIGHTEN,
    DISABLED = DisplayState::DISABLED,
    SELECTED_HIGHLITEN = SELECTED | HIGHLITEN,
    SELECTED_DISABLED = SELECTED | DISABLED 
};
};//namespace


TriggerLogic::TriggerLogic()
{
    Tie2Signal( OnPress[0]  , this, &TriggerLogic::Press );
    Tie2Signal( OnMoveOver,   this, &TriggerLogic::Highlite );
    Tie2Signal( OnMoveOutside, this,&TriggerLogic::Unhighlite );
    Tie2Signal( OnActivate,    this,&TriggerLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

bool TriggerLogic::IsDisabled() const
{
    return GetStateA()==DISABLED || GetStateA()==SELECTED_DISABLED;
}

bool TriggerLogic::IsSelected() const
{
    return GetStateA()==SELECTED || GetStateA()==SELECTED_HIGHLITEN || GetStateA()==SELECTED_DISABLED;
}

int TriggerLogic::GetSelected() const
{
    return IsSelected();
}

void TriggerLogic::SetSelected(int s)
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


void TriggerLogic::Press()
{
     if (GetStateA()==HIGHLITEN) { SetStateA(SELECTED_HIGHLITEN); OnChange.Send_1(true); }
}


void TriggerLogic::Highlite()
{
    if (GetStateA()==SELECTED ) { SetStateA(SELECTED_HIGHLITEN); } else 
    if (GetStateA()==NORMAL )   { SetStateA(HIGHLITEN); }
}

void TriggerLogic::Unhighlite()
{
    if (GetStateA()==HIGHLITEN )          { SetStateA(NORMAL);   } else 
    if (GetStateA()==SELECTED_HIGHLITEN ) { SetStateA(SELECTED); }
}

void TriggerLogic::Enable(bool t)
{
    if (!t)
    {
        if (GetStateA()==SELECTED || GetStateA()==SELECTED_HIGHLITEN) { SetStateA(SELECTED_DISABLED); }
        else if (GetStateA()==NORMAL || GetStateA()==HIGHLITEN)       { SetStateA(DISABLED); }
    }
    else
    {
        if (GetStateA()==SELECTED_DISABLED) { SetStateA(SELECTED); } else 
        if (GetStateA()==DISABLED)          { SetStateA(NORMAL); }
    }
}



};//namespace Useless
