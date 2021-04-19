#include "UselessPch.h"

#include "Useless/GUI/Logic/RadioLogic.h"

namespace Useless {

namespace {
enum 
{
    RADIO = DisplayState::MARKED,
    HIGHLITEN = DisplayState::HIGHLIGHTEN,
    DISABLED = DisplayState::DISABLED,
    RADIO_DISABLED = RADIO | DISABLED 
};
};//namespace

RadioLogic::RadioLogic()
{
    Tie2Signal( OnPress[0]  , this, &RadioLogic::Press );
    Tie2Signal( OnMoveOver,   this, &RadioLogic::Highlite );
    Tie2Signal( OnMoveOutside, this,&RadioLogic::Unhighlite );
    Tie2Signal( OnActivate,    this,&RadioLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }

}

bool RadioLogic::IsDisabled() const
{
    return GetStateA()==DISABLED || GetStateA()==RADIO_DISABLED;
}

bool RadioLogic::IsRadio() const
{
    return GetStateA()==RADIO || GetStateA()==RADIO_DISABLED;
}

int RadioLogic::GetSelected() const
{
    return IsRadio();
}

void RadioLogic::SetSelected(int s)
{
    if ( (bool)s!=IsRadio() )
    {
        if ( IsActive() )
        {
            if (s) { SetStateA(RADIO); }
            else   { SetStateA(NORMAL); }
        }
        else
        {
            if (s) { SetStateA(RADIO_DISABLED); }
            else   { SetStateA(DISABLED); }
        }
        OnChange.Send_1( (bool)s );
    }
}

bool RadioLogic::IsClickPossibile( const Pos &, int btn_no ) const
{
    return true;//btn_no!=0;
}

void RadioLogic::Press()
{
     if (!IsDisabled() && GetStateA()!=RADIO)
     {
        SetStateA(RADIO); OnChange.Send_1(true);
     }
}


void RadioLogic::Highlite()
{
    if (GetStateA()==NORMAL )   { SetStateA(HIGHLITEN); }
}

void RadioLogic::Unhighlite()
{
    if (GetStateA()==HIGHLITEN ) { SetStateA(NORMAL); }
}

void RadioLogic::Enable(bool t)
{
    if (!t)
    {
        if (GetStateA()==RADIO )  { SetStateA(RADIO_DISABLED); }
        else if (GetStateA()==NORMAL || GetStateA()==HIGHLITEN) { SetStateA(DISABLED); }
    }
    else
    {
        if (GetStateA()==RADIO_DISABLED) { SetStateA(RADIO); } else 
        if (GetStateA()==DISABLED)       { SetStateA(NORMAL);}
    }
}



};//namespace Useless
