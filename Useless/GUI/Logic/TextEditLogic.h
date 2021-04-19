#ifndef __INCLUDED__USELESS__TEXT_EDIT_LOGIC_H__
#define __INCLUDED__USELESS__TEXT_EDIT_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Text/Text.h"
#include <set>

namespace Useless {

struct CharValidator
{
    virtual bool Validate( int &ch ) const = 0;
};

class USELESS_API TextEditLogic : virtual public ActiveWidget, virtual public LogicWidget
{
public:
    TextEditLogic( const Text & = Text() );
    virtual ~TextEditLogic();

    void SetCharLimit( int max_chars );
    int  GetCharLimit() const { return _char_limit; }

    void SetAllowedChars( const std::set< UniCode > &chars, bool enable = true );
    std::set< UniCode > GetAllowedChars() const { return _allowed_chars; }

    void SetCharValidator( SPointer< CharValidator > validator );
    SPointer< CharValidator > GetCharValidator() const { return _char_validator; }

    //-- Client Methods
    enum Status { NORMAL=1, FOCUSSED=2, DISABLED=3 };
    virtual bool SetState( int ) = 0;       //< Set visual state
    virtual bool HasState( int ) const = 0;//< Is visual state available
    virtual int  GetState() const = 0;    //< Get current visual state
    virtual int  GetLetterAt( const Pos& ) const = 0; //< Requires valid index number
    virtual bool Validate( int& ) const; //< Validates incoming characters

    Signal_1<int>   OnCarMove;    //!< Notifies about carrige movement
    Signal_1<Text>  OnTextChange; //!< Notifies when text has changed
    Signal_1<Text>  OnConfirm;    //!< Notifies if return were pressed

    //-- Server methods
    void SetText( const Text& );
    const Text& GetText() const { return _text; }

    void PutChar( int );
    void Backspace();
    void Delete();
    void CarBack();
    void CarForward();
    void CarHome();
    void CarEnd();
    void Confirm();
    void BeginKeySelect();
    void LeaveKeySelect();
    void BeginSelect( const Pos & );
    void SelectMore( const Pos & );
    void ResetSelection();
    void SelectAll();

    int Begin() const { return _begin; }//< Retrive selection begin
    int End() const   { return _end; } //< Retrive selection end

private:
    // selection data
    int  _begin;
    int  _end;   
    int  _start;
    int  _tail;
    bool _key_select;
    
    Text _text;
    int  _char_limit;
    bool _enable_allowed_chars;
    std::set< UniCode > _allowed_chars;
    SPointer< CharValidator > _char_validator;

    void SetFocussed( bool );
    void SetEnabled( bool );
};

};//namespace Useless
#endif//__INCLUDED__USELESS__TEXT_EDIT_LOGIC_H__

