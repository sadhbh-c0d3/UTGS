#include "UselessPch.h"

#include "Useless/GUI/Logic/HiliteLogic.h"

namespace Useless {

HiliteLogic::HiliteLogic()
{
    Tie2Signal( OnMoveOver,   this, &HiliteLogic::Highlite );
    Tie2Signal( OnMoveOutside, this,&HiliteLogic::Unhighlite );
    Tie2Signal( OnActivate,    this,&HiliteLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

bool HiliteLogic::IsDisabled() const
{
    return GetState()==DisplayState::DISABLED;
}


void HiliteLogic::Highlite()
{
    if (GetState()==DisplayState::NORMAL ) { SetState(DisplayState::HIGHLIGHTEN); }
}

void HiliteLogic::Unhighlite()
{
    if (GetState()==DisplayState::HIGHLIGHTEN )  { SetState(DisplayState::NORMAL);   }
}

void HiliteLogic::Enable(bool t)
{
    if (!t)
    {
        SetState(DisplayState::DISABLED);
    }
    else
    {
        SetState(DisplayState::NORMAL);
    }
}



};//namespace Useless
