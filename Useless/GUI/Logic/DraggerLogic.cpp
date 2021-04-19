#include "UselessPch.h"

#include "Useless/GUI/Logic/DraggerLogic.h"

namespace Useless {

DraggerLogic::DraggerLogic()
{
    Tie2Signal( OnPress[0]  , this, &DraggerLogic::SetPressed );
    Tie2Signal( OnRelease[0], this, &DraggerLogic::SetHighliten );
    Tie2Signal( OnMoveOver,   this, &DraggerLogic::SetHighliten );
    Tie2Signal( OnMoveOutside, this,&DraggerLogic::SetNormal );
    Tie2Signal( OnActivate,    this,&DraggerLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

void DraggerLogic::SetHighliten ()
{
    if (!IsDisabled()) { SetState(DisplayState::HIGHLIGHTEN); }
}

void DraggerLogic::SetPressed ()
{
    if (!IsDisabled() ) { SetState(DisplayState::PRESSED); }
}

void DraggerLogic::SetNormal  ()
{
    if (!IsDisabled() )  { SetState(DisplayState::NORMAL); }
}

void DraggerLogic::SetDisabled()
{
    if (!IsDisabled() ) { SetState(DisplayState::DISABLED); }
}

void DraggerLogic::Enable( bool t)
{
    if (t) { SetState(DisplayState::NORMAL); } 
    else   { SetState(DisplayState::DISABLED); }
}

bool DraggerLogic::IsDisabled() const
{
    return GetState()==DisplayState::DISABLED;
}

};//namespace Useless
