#include "UselessPch.h"
#include "Useless/Util/CTC.h"
#include "Useless/Util/StaticCheck.h"
#include "Useless/GUI/Logic/ButtonLogic.h"

#include "Useless/GUI/Misc/KeyPredicates.h"
#include "Useless/Functional/ConditionalSignal.h"

namespace Useless {

ButtonLogic::ButtonLogic()
{
    ::CTC::Conversion<float,double>::exists;
    
    Tie2Signal( OnPress[0]  , this, &ButtonLogic::SetPressed );
    //Tie2Signal( OnRelease[0], this, &ButtonLogic::SetHighliten );
    Tie2Signal( OnMoveIn[0] , this, &ButtonLogic::SetPressed );
    Tie2Signal( OnMoveOut[0], this, &ButtonLogic::SetHighliten );
    Tie2Signal( OnMoveOver,   this, &ButtonLogic::SetHighliten );
    Tie2Signal( OnMoveOutside, this,&ButtonLogic::SetNormal );
    Tie2Signal( OnActivate,    this,&ButtonLogic::Enable );

    OnRelease[0].Tie( MakeCondFunc( 
        WhenState( this, WhenOver() ), this, &ButtonLogic::SetHighliten ) );
    OnRelease[0].Tie( MakeCondFunc( 
        WhenState( this, !WhenOver() ), this, &ButtonLogic::SetNormal ) );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

void ButtonLogic::SetHighliten ()
{
    if (!IsDisabled()) { SetStateA(DisplayState::HIGHLIGHTEN); }
}

void ButtonLogic::SetPressed ()
{
    if (!IsDisabled() ) { SetStateA(DisplayState::PRESSED); }
}

void ButtonLogic::SetNormal  ()
{
    if (!IsDisabled() )  { SetStateA(DisplayState::NORMAL); }
}

void ButtonLogic::SetDisabled()
{
    if (!IsDisabled() ) { SetStateA(DisplayState::DISABLED); }
}

void ButtonLogic::Enable( bool t)
{
    if (t) { SetStateA(DisplayState::NORMAL); } 
    else   { SetStateA(DisplayState::DISABLED); }
}

bool ButtonLogic::IsDisabled() const
{
    return GetDP().Get( DisplayState::DISABLED );
}

};//namespace Useless
