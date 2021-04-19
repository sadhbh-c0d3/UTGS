#ifndef __INCLUDED__USELESS_FORM_H__
#define __INCLUDED__USELESS_FORM_H__

#include "Useless/Functional/SignalSwitch.h"

namespace Useless {

class ActiveWidget; //W/A

/*! Form manages input focus.
 */
class USELESS_API Form
{
public:
    virtual ~Form() {}

    enum { DEFAULT_MODE };

    /*! Needed overloads */
    virtual void SetFocussed    ( Widget *widget ) = 0;
    virtual void InvalidateFocus( Widget *sender ) = 0;
    virtual Widget* GetFocussed () const = 0;

    /* Optional overloads */
    virtual void CycleFocus( int mode )            { assert("Don't use this method, since it has no effect"); }
    virtual void SetDefaultFocus( Widget *sender ) { assert("Don't use this method, since it has no effect"); }
    virtual void SetSubmitKey( int key_code )      { assert("Don't use this method, since it has no effect"); }
    virtual void SetCancelKey( int key_code )      { assert("Don't use this method, since it has no effect"); }
    
    /* Let form assign focus to its children */
    virtual void EnableForm( bool t = true )       { assert("Don't use this method, since it has no effect"); }

    /* Used internally by ActiveWidget::SetEnvironment(p!=NULL)
     * When sender==NULL accel key is being unregistered  */
    virtual void RegisterAccelKey( int key_code, ActiveWidget *sender ) {}

    /*! \warning
     * In case of FormWidget after: p_form->OnKey.Tie...(key,...)
     * you have to add: p_form->EnableKey(key);
     */
    SignalSwitch<int>  OnKey;

    SignalSwitch<bool> OnSubmit; // true: submit, false: cancel
};

};//namespace Useless
#endif//__INCLUDED__USELESS_FORM_H__
