#include "UselessPch.h"

#pragma warning(disable:4101)

#include "Useless/GUI/Widgets/TextEditWidget.h"
#include "Useless/Functional/SignalSwitch.h"

#include "Useless/ErrorConfig.h"

#include "Useless/System/KeyCodes.h"
#include "Useless/GUI/Misc/KeyPredicates.h"
#include "Useless/Functional/ConditionalSignal.h"

namespace Useless {

TextEditWidget::TextEditWidget( int w, int h, int x, const Font &font, CopyOf<Skin> frame_skin, CopyOf<Skin> marker_skin, const Text &text):
    _font(font), _text(text), _sp_frame_skin(&frame_skin), _sp_marker_skin(&marker_skin),
    _begin(0), _end(0), _x_scroll(0), _key_select(0)
{
    if (!_sp_frame_skin->GetDirection().x)
    {
        APP_WARNING("TextEditWidget: Passed skin cannot resize horizontally","TextEditWidget");
    }
    _sp_frame_skin->SetSize( Dim2i(w,h) );
    _text_position.y = ( h-font.GetHeight() )/2;
    _text_position.x = x;

    OnKeyPress.Tie( MakeCondFunc( 
      WhenState( this, (WhenKey(Keys::LSHIFT) || WhenKey(Keys::RSHIFT)) && 
                             (WhenKey(Keys::LEFT) || WhenKey(Keys::RIGHT) ||
                              WhenKey(Keys::HOME) || WhenKey(Keys::END)) 
                ), this, &TextEditWidget::ToggleSelect, true ) );

    SignalSwitch<int>::Ptr sspSwitch = new SignalSwitch<int>;
    sspSwitch->TieVoid(Keys::RET, this, &TextEditWidget::Confirm);
    sspSwitch->TieVoid(Keys::BS, this, &TextEditWidget::BackSpace);
    sspSwitch->TieVoid(Keys::DEL, this, &TextEditWidget::Delete);
    sspSwitch->TieVoid(Keys::LEFT, this, &TextEditWidget::CarBack);
    sspSwitch->TieVoid(Keys::RIGHT, this, &TextEditWidget::CarForward);
    sspSwitch->TieVoid(Keys::HOME, this, &TextEditWidget::CarHome);
    sspSwitch->TieVoid(Keys::END, this, &TextEditWidget::CarEnd);

    Tie2Signal( OnKeyPress, sspSwitch, &SignalSwitch<int>::Switch );
    Tie2Signal( WhileKey,   sspSwitch, &SignalSwitch<int>::Switch );

    //SignalSwitch<int>::Ptr sspSwitch2 = new SignalSwitch<int>;
    //sspSwitch2->TieUnary(Keys::LSHIFT, this, &TextEditWidget::ToggleSelect, true);
    //sspSwitch2->TieUnary(Keys::RSHIFT, this, &TextEditWidget::ToggleSelect, true);
    //Tie2Signal( OnKeyPress, sspSwitch2, &SignalSwitch<int>::Switch );

    SignalSwitch<int>::Ptr sspSwitch3 = new SignalSwitch<int>;
    sspSwitch3->TieUnary(Keys::LSHIFT, this, &TextEditWidget::ToggleSelect, false);
    sspSwitch3->TieUnary(Keys::RSHIFT, this, &TextEditWidget::ToggleSelect, false);
    Tie2Signal( OnKeyRelease, sspSwitch3, &SignalSwitch<int>::Switch );

    Tie2Signal( OnChar, this, &TextEditWidget::PutChar );
    Tie2Signal( OnPress[0], this, &TextEditWidget::BeginSelect);
    Tie2Signal( OnDrag[0], this, &TextEditWidget::UpdateSelect);
    Tie2Signal( OnPress[0], this, &Widget::FocusIntercept );

    Tie2Signal(  OnFocus, this, (void (Widget::*)() )&Widget::SetDirty );
    Tie2Signal( OffFocus, this, (void (Widget::*)() )&Widget::SetDirty );
    OffFocus.TieUnary( this, &TextEditWidget::ToggleSelect, false );

    SetRepeatRate( 40, 250);
    _letters = _font.GetStats( _text );
    _car = 0;
    _car_w[0] = GetFont().GetWidth(0x1);
    _car_w[1] = GetFont().GetWidth(0x2);

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

    ActiveWidget::SetFocusAdaptive(true);

    SetDimensions(w,h);
}

TextEditWidget::~TextEditWidget()
{
}

void TextEditWidget::SetText(const Text &text)
{ 
    _text=text;
    _letters = _font.GetStats( _text );
    SetDirty(); 
}


void TextEditWidget::PutChar( char ch)
{
    _key_select = false;
    if (ch>=0x20)
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

void TextEditWidget::BackSpace()
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
    SetText( new_text);
    CarBack();
}

void TextEditWidget::Delete()
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
    SetText( new_text);
    UpdateCar();
}

void TextEditWidget::CarBack()
{
    if (_key_select)
    {
        _sel_car = _sel_car>0 ? --_sel_car : 0;
        ( _sel_car < _click ? _begin : _end) = _sel_car;
        ( _sel_car < _click ? _end : _begin) = _click;
    }
    else
    {
        if ( _begin==_end)
        {
            _begin = _begin>0 ? --_begin : 0;
        }
        _end = _begin;
    }
    UpdateCar();
}

void TextEditWidget::CarForward()
{
    long length = GetText().Length();

    if (_key_select)
    {
        _sel_car = _sel_car<length ? ++_sel_car : length;
        ( _sel_car < _click ? _begin : _end) = _sel_car;
        ( _sel_car < _click ? _end : _begin) = _click;
    }
    else
    {
        if ( _begin <_end) _end = _begin;
        _end = _end<length ? ++_end : length;
        _begin = _end;
    }

    UpdateCar();
}

void TextEditWidget::CarHome()
{
    _sel_car = _begin = 0;
    _end = _key_select ? _click : _begin;
    UpdateCar();
}

void TextEditWidget::CarEnd()
{
    _sel_car = _end = GetText().Length();
    _begin = _key_select ? _click : _end;
    UpdateCar();
}

void TextEditWidget::Confirm()
{   
    FocusRelease();
    OnConfirm.Send_1( GetText() );
}

void TextEditWidget::ToggleSelect(bool t)
{
    if (!_key_select && t) { _sel_car=_click=_begin=_end; }
    _key_select = t;
}

void TextEditWidget::BeginSelect( const Pos &press_pos)
{
    int i,n,w; _key_select=false;
    
    for( i=0, w=0, n=_letters.Size(); i<n && w<=press_pos.x-_text_position.x+_x_scroll-_car_w[0]/2; ++i)
    {
        w += _letters[i];
    }
    if ( w > press_pos.x+_x_scroll ) { --i; }
    _begin = _end = i;
    _click = i;
    UpdateCar();
}

void TextEditWidget::UpdateSelect( const Pos &pos)
{
    int i,n,w;
    for ( i=0, w=0, n=_letters.Size(); i<n && w<pos.x-_text_position.x+_x_scroll ; ++i)
    {
        w += _letters[i];
    }
    if ( w > pos.x+_x_scroll ) { --i; }
    _begin = i < _click ? i : _click;
    _end = i > _click ? i   : _click;
    UpdateCar();
}

void TextEditWidget::UpdateCar()
{
    int i,w, n = _letters.Size();
    for ( i=0, w=0; i<_begin && i<n; ++i)
    {
        w += _letters[i];
    }
    _begin=i;
    _car = w;
    for ( ; i<_end && i<n; ++i)
    {
        w += _letters[i];
    }
    _end=i;
    _car_end = w;
    int actor = _click==_end ? _car : _car_end;
    while ( actor-_x_scroll > GetWidth() - 2*_car_w[0] ) { _x_scroll += GetWidth()/4 ;}
    while ( actor-_x_scroll < 0 ) { _x_scroll = _x_scroll<GetWidth()/4 ? 0 : _x_scroll-GetWidth()/4; }
    SetDirty();
}

void TextEditWidget::Repaint( const WidgetPainter &delivered_painter )
{
    _sp_frame_skin->Paint(delivered_painter);
    WidgetPainter painter(delivered_painter);
    if ( painter.Intersect( Rect( 2, 0, GetWidth()-4, GetHeight() ) ) )
    {
        if ( HasFocus() )
        {
            if ( _begin!=_end )
            {
                _sp_marker_skin->SetSize( Dim2i( _car_end-_car, GetFont().GetHeight() ) );
                _sp_marker_skin->Paint( painter, _text_position+Pos( _car -_x_scroll,0) );
                GetFont().Paint( painter, Text(0x2), _text_position+Pos( _car_end-_x_scroll-_car_w[1]+1,0) );
            }
            GetFont().Paint( painter, Text(0x1), _text_position+Pos( _car-_x_scroll-_car_w[0]/2,0) );
        }
        GetFont().Paint( painter, GetText(), _text_position-Pos(_x_scroll,0) );
        painter.RestoreClipper();
    }
}

};//namespace Useless
