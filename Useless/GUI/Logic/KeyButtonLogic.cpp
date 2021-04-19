#include "UselessPch.h"

#include "Useless/GUI/Logic/KeyButtonLogic.h"
#include "Useless/GUI/Misc/KeyPredicates.h"

namespace Useless {

KeyButtonLogic::KeyButtonLogic( int key_code )
{
    Tie2Signal( OnFocus, this, &KeyButtonLogic::SetFocussed ); //since no clients implemented
    Tie2Signal( OffFocus, this, &KeyButtonLogic::SetNormal );  //we use highlight instead
    EnableFocussing( _code = key_code );
}

KeyButtonLogic::~KeyButtonLogic()
{
    DisableFocussing();
}

void KeyButtonLogic::EnableFocussing( int key_code)
{
    DisableFocussing();
    _code = key_code;

    _fids.push_back( Tie2Signal( OnPress[0], this, &KeyButtonLogic::LMBPress ));
    _fids.push_back( Tie2Signal( OffFocus, this, &KeyButtonLogic::FocLost ));
    _fids.push_back( OnMoveOutside.TieUnary( this, &Widget::InputRelease, (Widget*)0));
    _fids.push_back( Tie2Signal( OnKeyPress, this, &KeyButtonLogic::KbdPress ));
    _fids.push_back( Tie2Signal( WhileKey, this, &KeyButtonLogic::KbdHold ));
    _fids.push_back( Tie2Signal( OnKeyRelease, this, &KeyButtonLogic::KbdRelease ));

    ActiveWidget::SetFocusAdaptive( true );
    ActiveWidget::EnableKey( _code );
}

void KeyButtonLogic::DisableFocussing()
{
    ActiveWidget::EnableKey( _code, false );
    ActiveWidget::SetFocusAdaptive( false );
    _state = 0;
    
    std::vector< Signal::FuncID >::iterator
        it = _fids.begin(), end = _fids.end();
        
    for (; it!=end; ++it) {
        (*it).Untie();
    }
}

void KeyButtonLogic::LMBPress( const Pos &where )
{
    if ( IsFocusAdaptive() )
    {
        FocusIntercept();
    }
}

void KeyButtonLogic::FocLost()
{
    if ( !WasOver() )
    {
        OnMoveOutside.Send();
    }
}
    
void KeyButtonLogic::KbdPress( int code )
{
    if ( 0 == _state && code == _code && IsFocusAdaptive() && NoButtons() )
    {
        _state = 1;
        OnPress[0].Schedule();
    }
}


void KeyButtonLogic::KbdHold( int code )
{
    if ( 1 == _state && code == _code )
    {
        WhileHold[0].Schedule();
    }
}


void KeyButtonLogic::KbdRelease( int code )
{
    if ( 1 == _state && code == _code )
    {
        _state = 0;
        OnRelease[0].Schedule();
        OnClick[0].Schedule();
    }
}


void KeyButtonLogic::SetFocussed()
{
    SetState( GetDP().CpSet( DisplayState::FOCUSSED ) );
    if ( NoButtons()) {
        OnActorMove.Send_1( GetBoundingRect() );
    }
}

void KeyButtonLogic::SetNormal()
{
    SetState( GetDP().CpClr( DisplayState::FOCUSSED ) );
}

};//namespace Useless
