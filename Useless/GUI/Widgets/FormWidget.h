#ifndef __INCLUDED__USELESS_GUI_FORM_WIDGET_H__
#define __INCLUDED__USELESS_GUI_FORM_WIDGET_H__

#pragma warning(disable:4250)
#include "Useless/GUI/Widgets/CompoundWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Misc/WidgetCycler.h"
#include "Useless/GUI/Form.h"
#include "Useless/GUI/WidgetEnvironment.h"

namespace Useless {

/*! FormWidget is a kind of Form which is also a Widget.
 */
class USELESS_API FormWidget : virtual public CompoundWidget, virtual public ActiveWidget, public Form
{
public:
    FormWidget();
    virtual ~FormWidget();
    virtual void Setup();

    /* Focus cycling modes */
    enum FocusMode { NEXT=DEFAULT_MODE, PREVIOUS, LEFT, RIGHT, ABOVE, BELOW };

    virtual void CycleFocus( int mode = NEXT );

    virtual void SetFocussed    ( Widget *widget );
    virtual void InvalidateFocus( Widget *sender );
    virtual Widget* GetFocussed() const;

    virtual void SetDefaultFocus( Widget *sender );
    virtual void SetSubmitKey( int key_code );
    virtual void SetCancelKey( int key_code );     
    virtual void RegisterAccelKey( int key_code, ActiveWidget *sender );

    virtual bool Accept(const Request& request);
    virtual void SetEnvironment( const WidgetEnvironment *enviro);
    virtual const WidgetEnvironment* GetChildrenEnvironment() const;

    virtual void EnableForm( bool t = true );

private:
    void AssignFocus();
    void UnassignFocus();
    void CycleParentForm( int mode );
    void HandleKey( int key_code );

    SPointer<WidgetCycler>   _cycler;
    Widget                  *_actual_focus;
    Widget                  *_stored_focus;

    WidgetEnvironment        _child_env;
    bool                     _dont_cycle;
    bool                     _focus_locked;
    bool                     _form_enabled;

    Widget                  *_default_focus;
    int                      _submit_key;
    int                      _cancel_key;

    typedef AssocVector< int, Widget* > AccelMap;
    AccelMap                 _accelerators;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_GUI_FORM_WIDGET_H__
