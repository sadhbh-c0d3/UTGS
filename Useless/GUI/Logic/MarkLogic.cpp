#include "UselessPch.h"

#include "Useless/GUI/Logic/MarkLogic.h"

namespace Useless {

namespace {
enum 
{
    MARKED = DisplayState::MARKED,
    HIGHLITEN = DisplayState::HIGHLIGHTEN,
    DISABLED = DisplayState::DISABLED,
    MARKED_HIGHLITEN = MARKED | HIGHLITEN,
    MARKED_DISABLED = MARKED | DISABLED 
};
};//namespace

MarkLogic::MarkLogic()
{
    _status = NORMAL;
    Tie2Signal( OnPress[0]  , this, &MarkLogic::Press );
    Tie2Signal( OnClick[0]  , this, &MarkLogic::Change );
    Tie2Signal( OnMoveIn[0] , this, &MarkLogic::Press );
    Tie2Signal( OnMoveOut[0], this, &MarkLogic::Release );
    Tie2Signal( OnMoveOver,   this, &MarkLogic::Highlite );
    Tie2Signal( OnMoveOutside, this,&MarkLogic::Unhighlite );
    Tie2Signal( OnActivate,    this,&MarkLogic::Enable );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }
}

bool MarkLogic::IsDisabled() const
{
    return GetDP().Get( DISABLED );
}

bool MarkLogic::IsMarked() const
{
    return GetDP().Get( MARKED );
}

int MarkLogic::GetSelected() const
{
    return IsMarked();
}

void MarkLogic::SetSelected(int s)
{
    if ( (bool)s!=IsMarked() )
    {
        if ( IsActive() )
        {
            if (s) { _status=MARKED; }
            else   { _status=NORMAL; }
        }
        else
        {
            if (s) { _status=MARKED_DISABLED; }
            else   { _status=DISABLED; }
        }
        SetStateA(_status);
        OnChange.Send_1( (bool)s );
    }
}


void MarkLogic::Press()
{
     if (GetDP().Get(MARKED)) { SetState( GetDP().CpClr(MARKED)); }
     else { SetState( GetDP().CpSet(MARKED)); }
}

void MarkLogic::Release()
{
    SetStateA( (_status & MARKED)? MARKED_HIGHLITEN : HIGHLITEN );
}

void MarkLogic::Change()
{
    _status = ( GetStateA()==MARKED || GetStateA()==MARKED_HIGHLITEN)? MARKED_HIGHLITEN : HIGHLITEN;
    OnChange.Send_1( IsMarked() );
}

void MarkLogic::Highlite()
{
    if (GetStateA()==MARKED ) { SetStateA(MARKED_HIGHLITEN); } else 
    if (GetStateA()==NORMAL ) { SetStateA(HIGHLITEN); }
}

void MarkLogic::Unhighlite()
{
    if (GetStateA()==HIGHLITEN )        { SetStateA(NORMAL); } else 
    if (GetStateA()==MARKED_HIGHLITEN ) { SetStateA(MARKED); }
}

void MarkLogic::Enable(bool t)
{
    if (!t)
    {
        if ( GetDP().Get(MARKED) ) { SetStateA(MARKED_DISABLED); }
        else { SetStateA(DISABLED); }
    }
    else
    {
        if ( GetDP().Get(MARKED) ) { SetStateA(MARKED); }
        else { SetStateA(NORMAL); }
    }
}



};//namespace Useless
