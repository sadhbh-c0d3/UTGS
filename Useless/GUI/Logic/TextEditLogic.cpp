#include "UselessPch.h"


#include "Useless/Functional/ConditionalSignal.h"
#include "Useless/Functional/SignalSwitch.h"
#include "Useless/GUI/Logic/TextEditLogic.h"
#include "Useless/GUI/Misc/KeyPredicates.h"
#include "Useless/System/KeyCodes.h"

namespace Useless {

TextEditLogic::TextEditLogic( const Text &text )
    : _begin(0), _end(0), _start(0), _tail(0), _key_select(0), _text(text), _char_limit(-1)
{
    OnKeyPress.Tie( MakeCondFunc( 
        WhenState( this, (WhenKey(Keys::LSHIFT) || WhenKey(Keys::RSHIFT)) && 
                         (  WhenKey(Keys::LEFT) || WhenKey(Keys::RIGHT) ||
                            WhenKey(Keys::HOME) || WhenKey(Keys::END)   ) ),
        this, &TextEditLogic::BeginKeySelect ) );

    SignalSwitch<int>::Ptr sspSwitch3 = new SignalSwitch<int>;
    sspSwitch3->TieVoid(Keys::LSHIFT, this, &TextEditLogic::LeaveKeySelect );
    sspSwitch3->TieVoid(Keys::RSHIFT, this, &TextEditLogic::LeaveKeySelect );
    Tie2Signal( OnKeyRelease, sspSwitch3, &SignalSwitch<int>::Switch );


    SignalSwitch<int>::Ptr sspSwitch = new SignalSwitch<int>;
    sspSwitch->TieVoid(Keys::RET, this, &TextEditLogic::Confirm);
    sspSwitch->TieVoid(Keys::BS, this, &TextEditLogic::Backspace);
    sspSwitch->TieVoid(Keys::DEL, this, &TextEditLogic::Delete);
    sspSwitch->TieVoid(Keys::LEFT, this, &TextEditLogic::CarBack);
    sspSwitch->TieVoid(Keys::RIGHT, this, &TextEditLogic::CarForward);
    sspSwitch->TieVoid(Keys::HOME, this, &TextEditLogic::CarHome);
    sspSwitch->TieVoid(Keys::END, this, &TextEditLogic::CarEnd);
    Tie2Signal( OnKeyPress, sspSwitch, &SignalSwitch<int>::Switch );
    Tie2Signal( WhileKey,   sspSwitch, &SignalSwitch<int>::Switch );


    Tie2Signal( OnChar,     this, &TextEditLogic::PutChar );
    Tie2Signal( OnPress[0], this, &TextEditLogic::BeginSelect);    // Set carret position
    Tie2Signal( OnPress[0], this, &TextEditLogic::FocusIntercept );// Select all
    Tie2Signal( OnCatch[0], this, &TextEditLogic::BeginSelect);    // Cancel & start new selection
    Tie2Signal( OnDrag[0],  this, &TextEditLogic::SelectMore);     // Update selection

    Tie2Signal(  OnFocus, this, &TextEditLogic::SelectAll );
    Tie2Signal( OffFocus, this, &TextEditLogic::ResetSelection );

    OnFocus.TieUnary( this, &TextEditLogic::SetFocussed, true );
    OffFocus.TieUnary( this, &TextEditLogic::SetFocussed, false );
    Tie2Signal( OnActivate, this, &TextEditLogic::SetEnabled );

    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }

    EnableKey(Keys::RET);
    EnableKey(Keys::BS);
    EnableKey(Keys::DEL);
    EnableKey(Keys::LEFT);
    EnableKey(Keys::RIGHT);
    EnableKey(Keys::HOME);
    EnableKey(Keys::END);
    EnableKey(Keys::LSHIFT);
    EnableKey(Keys::RSHIFT);
    EnableCharInput();

    SetRepeatRate( 40, 250);
    ActiveWidget::SetFocusAdaptive( true );
}

TextEditLogic::~TextEditLogic()
{
}

void TextEditLogic::SetCharLimit( int max_chars )
{
    _char_limit = max_chars;
}

void TextEditLogic::SetAllowedChars( const std::set< UniCode > &chars, bool enable )
{
    if ( enable )
    {
        _enable_allowed_chars = true;
        _allowed_chars = chars;        
    }
    else
    {
        _enable_allowed_chars = false;
        _allowed_chars.clear();
    }
}

void TextEditLogic::SetCharValidator( SPointer< CharValidator > validator )
{
    _char_validator = validator;
}

void TextEditLogic::SetText(const Text &text)
{ 
    _text=text;
    OnTextChange.Send_1(_text);

    if ( _text.length() < _end )
    {
        OnCarMove.Send_1( _begin = _end = _text.length() );
    }
}

bool TextEditLogic::Validate( int &ch ) const
{
    if ( _enable_allowed_chars )
    {
        if ( _allowed_chars.find( ch ) == _allowed_chars.end() )
        {
            return false;
        }
    }

    if ( _char_limit >= 0 && _text.length() >= _char_limit )
    {
        return false;
    }

    if ( !!_char_validator )
    {
        return _char_validator->Validate( ch );
    }
    else
    {
        return (ch >= 0x20);
    }
}

void TextEditLogic::PutChar( int ch )
{
    _key_select = false;
    if ( Validate(ch) )
    {
        Text text = GetText();
        long last_char = text.Length()-1;
        Text new_text;
        if (_begin > 0 ) new_text += text( 0, _begin);
        new_text += ch;
        if (_end < last_char+1) new_text += text( _end, last_char+1);
        SetText( new_text);
        CarForward();
    }
}

void TextEditLogic::Backspace()
{
    Text text = GetText();
    long last_char = text.Length()-1;
    Text new_text;
    if ( _begin==_end )
    {
        if ( _begin>1 ) new_text += text( 0, _begin-1);
        if ( _end < last_char+1) new_text += text( _end, last_char+1);
    }
    else
    {
        if ( _begin>0 ) new_text += text( 0, _begin);
        if ( _end < last_char+1) new_text += text( _end, last_char+1);
    }
    CarBack();
    SetText( new_text);
}

void TextEditLogic::Delete()
{
    Text text = GetText();
    long last_char = text.Length()-1;
    Text new_text;
    if ( _begin==_end )
    {
        if ( _begin>0 ) new_text += text( 0, _begin);
        if ( _end < last_char) new_text += text( _end+1, last_char+1);
        _end = _begin;
    }
    else
    {
        if ( _begin>0 ) new_text += text( 0, _begin);
        if ( _end < last_char+1) new_text += text( _end, last_char+1);
        _end = _begin;
    }
    OnCarMove.Send_1( _start = _tail = _end );
    SetText( new_text);
}

void TextEditLogic::CarBack()
{
    if (_key_select)
    {
        _tail = _tail>0 ? --_tail : 0;
        ( _tail < _start ? _begin : _end) = _tail;
        ( _tail < _start ? _end : _begin) = _start;
    }
    else
    {
        if ( _begin==_end)
        {
            _begin = _begin>0 ? --_begin : 0;
        }
        _start = _tail = _end = _begin;
    }
    OnCarMove.Send_1( _tail );
}

void TextEditLogic::CarForward()
{
    long length = GetText().Length();

    if (_key_select)
    {
        _tail = _tail<length ? ++_tail : length;
        ( _tail < _start ? _begin : _end) = _tail;
        ( _tail < _start ? _end : _begin) = _start;
    }
    else
    {
        if ( _begin <_end) _end = _begin;
        _end = _end<length ? ++_end : length;
        _start = _tail = _begin = _end;
    }

    OnCarMove.Send_1( _tail );
}

void TextEditLogic::CarHome()
{
    _tail = _begin = 0;
    _end = _key_select ? _start : _begin;
    OnCarMove.Send_1( _tail );
}

void TextEditLogic::CarEnd()
{
    _tail = _end = GetText().Length();
    _begin = _key_select ? _start : _end;
    OnCarMove.Send_1( _tail );
}

void TextEditLogic::Confirm()
{   
    FocusRelease();
    OnConfirm.Send_1( GetText() );
}

void TextEditLogic::BeginKeySelect()
{
    if (!_key_select) 
    {
        _tail=_start=_begin=_end; 
    }
    _key_select = true;
}

void TextEditLogic::LeaveKeySelect()
{
    _key_select = false;
}

void TextEditLogic::ResetSelection()
{
    _key_select = false;
    _begin = _end = _start = _tail = 0;
    OnCarMove.Send_1( _start );
}

void TextEditLogic::SelectAll()
{
    _key_select = false;
    _begin = _start = 0;
    _end = _tail = GetText().length();
    OnCarMove.Send_1( _start );
}

void TextEditLogic::BeginSelect( const Pos &pos )
{
    if ( !_key_select )
    {
        int i = GetLetterAt( pos );
        _begin = _end = i;
        _start = i;
        OnCarMove.Send_1( _tail = _start );
    }
}

void TextEditLogic::SelectMore( const Pos &pos )
{
    int i = GetLetterAt( pos ); assert(i>=0);
    _begin = i < _start ? i : _start;
    _end = i > _start ? i   : _start;
    _tail = i;
    OnCarMove.Send_1( _tail );
}

void TextEditLogic::SetFocussed( bool t )
{
    if ( !t || !SetState( FOCUSSED ) )
    {
        if ( IsActive() )
        SetState( NORMAL );
    }
}

void TextEditLogic::SetEnabled( bool t )
{
    if ( t || !SetState( DISABLED ) )
    {
        SetState( NORMAL ); 
    }
}


};//namespace Useless
