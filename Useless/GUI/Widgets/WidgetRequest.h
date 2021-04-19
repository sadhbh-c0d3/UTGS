#ifndef __INCLUDED_USELESS_WIDGET_H__
    #error "This file is included by in Widget.h.\nPlease don't include this file on your own"
#endif

#ifndef __INCLUDED_USELESS_WIDGET_REQUESTS_H__
#define __INCLUDED_USELESS_WIDGET_REQUESTS_H__
namespace Useless {


///////////////////////////////////////////////////////////////////
//  Default request
//
class USELESS_API Widget::Request
{
public:
    Request(): _from(0), _to(0), _tell_childs(1), _tell_parent(0) {}
    Request(Widget* from, Widget* to=NULL, bool tell_childs=false, bool tell_parent=false):
            _from(from), _to(to), _tell_childs(tell_childs), _tell_parent(tell_parent) {}

    virtual ~Request()          {}

    Widget*  Sender()     const { return _from;}
    Widget*  Reciever()   const { return _to; }
       bool  TellChilds() const { return _tell_childs; }
       bool  TellParent() const { return _tell_parent; }

private:
    Widget   *_from;
    Widget   *_to;
      bool    _tell_childs;
      bool    _tell_parent;
};


///////////////////////////////////////////////////////////////////
//  InputCursor
//
class Widget::InputCursor : public Widget::Request
{
public:
    InputCursor() {}
    InputCursor(const Pos &p, const Pos &dp, int cond): Request(NULL), _position(p), _move(dp), _condition(cond) {}
    virtual ~InputCursor() {}

    Pos Position() const { return _position; }
    Pos Move()     const { return _move;     }
    int Cond()     const { return _condition;}

private:
   Pos _position;
    Pos _move;
    int _condition;
};


///////////////////////////////////////////////////////////////////
//  InputButton
//
class Widget::InputButton : public Widget::Request
{
public:
    InputButton() {}
    InputButton(int status, int changed, int cond): Request(NULL), _status(status), _changed(changed), _condition(cond) {}
    virtual ~InputButton() {}

    int Status()   const { return _status;   }
    int Changed()  const { return _changed;  }
    int Cond()     const { return _condition;}

private:
    int _status;
    int _changed;
    int _condition;
};


///////////////////////////////////////////////////////////////////
//  DoubleClick
//
class Widget::DoubleClick : public Widget::Request
{
public:
    DoubleClick( int button_no=0, int cond=0 ): Request(NULL), _button_no(button_no), _condition(cond) {}
    virtual ~DoubleClick() {}

    int ButtonNo()   const { return _button_no; }
    int Cond()       const { return _condition; }

private:
    int _button_no;
    int _condition;
};


///////////////////////////////////////////////////////////////////
//  InputWheel
//
class Widget::InputWheel : public Widget::Request
{
public:
    InputWheel() {}
    InputWheel(int z, int dz, int cond): _zpos(z), _dz(dz), _condition(cond) {}
    virtual ~InputWheel() {}

    int Z   () const { return _zpos; }
    int Dz  () const { return _dz; }
    int Cond() const { return _condition; }

private:
    int _zpos;
    int _dz;
    int _condition;

};


///////////////////////////////////////////////////////////////////
//  InputReset
//
class Widget::InputReset : public Widget::Request
{
public:
    InputReset() {}
    InputReset(Widget *sender): Request(sender,NULL,true,false) {}
    virtual ~InputReset() {}
};


///////////////////////////////////////////////////////////////////
//  InputKeyboard
//
class Widget::InputKeyboard : public Widget::Request
{
public:
    InputKeyboard() {}
    InputKeyboard(int key_code, bool pressed, int cond): _key_code(key_code), _pressed(pressed), _condition(cond) {}
    virtual ~InputKeyboard() {}

    int KeyCode() const { return _key_code; }
   bool Pressed() const  { return _pressed; }
    int Cond() const { return _condition; }

private:
    int _key_code;
   bool _pressed;
    int _condition;
};


///////////////////////////////////////////////////////////////////
//  InputChar
//
class Widget::InputChar : public Widget::Request
{
public:
    InputChar() {}
    InputChar(int char_code, int cond): _char_code(char_code), _condition(cond) {}
    virtual ~InputChar() {}

    int CharCode() const { return _char_code; }
    int Cond() const { return _condition; }

private:
    int _char_code;
    int _condition;
};


///////////////////////////////////////////////////////////////////
//  InputFocus
//
class Widget::InputFocus : public Widget::Request
{
public:
    InputFocus(): Request(0,0,0,0) {}
    virtual ~InputFocus() {}
};



///////////////////////////////////////////////////////////////////
//  InputAccel
//
class Widget::InputAccel : public Widget::Request
{
public:
    InputAccel( int key_code=0): _key_code(key_code) {}
    virtual ~InputAccel() {}

    int KeyCode() const { return _key_code; }

private:
    int _key_code;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_WIDGET_REQUESTS_H__
