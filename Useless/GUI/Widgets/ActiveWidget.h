#ifndef __INCLUDED_USELESS_ACTIVE_WIDGET_H__
#define __INCLUDED_USELESS_ACTIVE_WIDGET_H__

/*
 *    $Id: ActiveWidget.h,v 1.33 2004/07/02 18:06:19 koolas Exp $
 *
 *    NAME
 *        ActiveWidget
 *
 *    PURPOSE
 *        Widget who sends standard GUI signals for cursor events
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/Functional/MethodPool.h"
#include "Useless/Util/PreInitializer.h"
#include "Useless/System/Timer.h"
#include "Useless/Util/Storage/SortedVector.h"
#include "Useless/Util/Storage/BitVector.h"


namespace Useless {

/*! \ingroup Widgets
 *  A Widget which accept mouse events.
 */
class USELESS_API ActiveWidget : virtual public Widget, virtual public IWActive
{
public:
    ActiveWidget();
    virtual ~ActiveWidget() {}

    virtual Widget&       Atom()    { return *this; } //< To avoid dominance warrning messages
    virtual ActiveWidget& Control() { return *this; }

    enum { NUM_BUTTONS=3 };
    /////////////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME                      | ARGS
    //-----------------------------------------------------------------------
    mutable Signal_1<Pos>            OnPress   [NUM_BUTTONS];    // position
    mutable Signal_1<Pos>            OnRelease [NUM_BUTTONS];    // position
    mutable Signal_1<Pos>            OnClick   [NUM_BUTTONS];    // position
    mutable Signal_1<Pos>            OnDoubleClick[NUM_BUTTONS]; // position
    mutable Signal_1<Pos>            OnMoveIn  [NUM_BUTTONS];    // position
    mutable Signal_1<Pos>            OnMoveOut [NUM_BUTTONS];    // position
    mutable Signal_1<Pos>            OnMoveOver;                 // position
    mutable Signal_1<Pos>            OnMoveOutside;              // position
    mutable Signal_3<Pos,Pos,bool>   WhileMoving;                // P position, d-pos., is_inside
    mutable Signal_1<Pos>            OnCatch   [NUM_BUTTONS];    // position
    mutable Signal_3<Pos,Pos,Pos>    OnDrag    [NUM_BUTTONS];    // pos., d-pos., pick-pos.
    mutable Signal_2<Pos,Pos>        OnDrop    [NUM_BUTTONS];    // pos., pick-pos.
    mutable Signal_1<bool>           OnActivate;                 // T/F
    mutable Signal_1<Pos>            WhileHold [NUM_BUTTONS];    // position
    mutable Signal_1<Pos>            WhileDrag [NUM_BUTTONS];    // position
    mutable Signal_2<int,int>        OnWheel;                    // zpos, d-zpos
    mutable Signal_2<int,int>        OffWheel;                   // zpos, d-zpos
    mutable Signal_1<Pos>            OffPress  [NUM_BUTTONS];    // position
    mutable Signal_1<int>            OnChar;                     // character code
    mutable Signal_1<int>            OnKeyPress;                 // scan code
    mutable Signal_1<int>            OnKeyRelease;               // scan code
    mutable Signal_1<int>            WhileKey;                   // scan code
    mutable Signal_1<int>            WhileOffKey;                // scan code
    mutable Signal_1<int>            OffChar;                    // character code
    mutable Signal_1<int>            OffKeyPress;                // scan code
    //
    //  NOTE: const Widgets may also send signals
    /////////////////////////////////////////////////////////////////////////

    //-- Manage hints
    virtual void ShowHint();
    virtual void HideHint();
    //-- Register activation key
    virtual void Setup   ();
    virtual void Cleanup ();

    virtual bool InputIsEager    () const;
    virtual bool Accept(const Request &request);

    bool AcceptInputCursor  ( const InputCursor&  );
    bool AcceptInputButton  ( const InputButton&  );
    bool AcceptDoubleClick  ( const DoubleClick&  );
    bool AcceptInputWheel   ( const InputWheel&   );
    void AcceptInputReset   ( const InputReset&   );
    bool AcceptInputKeyboard( const InputKeyboard&);
    bool AcceptInputChar    ( const InputChar&    );
    bool AcceptInputFocus   ( const InputFocus&   );
    bool AcceptInputAccel   ( const InputAccel&   );

    //-- Turn on/off ability of this widget to accept any input reuest
    bool IsActive() { return _is_active; }
    void Activate();
    void Deactivate();
    void SetActive(bool t) { if(t) Activate(); else Deactivate(); }
    
    //-- Overload this method to get modified OnClick events
    virtual bool IsClickPossibile( const Pos &p, int btn_no ) const { return true; }

    //-- Set distance pointer has to travel to trigger OnCatch na OnDrag signals
    void SetDragRadius(int r) { assert(r>=0); _drag_radius=r; }
    //-- Set mouse button hold repeat period and delay
    void SetRepeatRate( long int period, long int delay ) { _period=period; _delay=delay; }

    //-- Turn on/off ability of this widget to grab keyboard focus
    void SetFocusAdaptive(bool t) { if(! (_likes_focus=t) ) FocusRelease(); }
    bool IsFocusAdaptive() const { return _likes_focus; }

    //-- Set keyboard input scan-code mask ( accept=1 and do-not-accept=0 )
    void SetKeyboardMask( const BitVector &key_mask );
    //-- Set single bit in input scan-code mask
    void EnableKey( int key_code, bool t=true );        //< only if ActiveWidget has direct focus
    void EnableOnPathKey( int key_code, bool t=true ); //< if ActiveWidget is on focus path
    //-- Turn on/off ability of this widget to accept characters
    void EnableCharInput( bool t=true ); // will enable character input, and
                                        // AcceptInputKeyboard method will return 'true'
                                       // all character keys will be treated as accepted

    //-- Set shortcut key ( evaluates as OnClick[0] )
    void SetActivationKey( int key_code );
    virtual int GetActivationKey() const  { return _activation_key; }

    //-- Local ActiveWidget internal state
    bool  NoButtons() const;
    bool  WasOver() const { return _was_over; }
     Pos  GetCursorPos() const { return _cursor; }
     Pos  GetTouchPos (int btn_no) const { assert(btn_no<NUM_BUTTONS);return   _touch_pos[btn_no]; }
    bool  IsDragging  (int btn_no) const { assert(btn_no<NUM_BUTTONS);return _is_dragging[btn_no]; }
    bool  WasTouched  (int btn_no) const { assert(btn_no<NUM_BUTTONS);return _was_touched[btn_no]; }
    bool  WasKeyPressed(int key_code ) const { return !!_pressed_keys.FindID( key_code ); }
    //-- Global Workspace state
    bool  IsKeyPressed(int key_code ) const;
    
private:
    bool _is_active;
    // That's standard
    Pos  _cursor;
    bool _mb_down[NUM_BUTTONS];

    bool PointerIdle(const Pos& p, bool drag);
    bool PointerDown(const Pos& p, int btn_no);
    bool PointerMove(const Pos& p, int btn_no);
    bool PointerUp  (const Pos& p, int btn_no);

    // That's for onClick
    bool _was_touched[NUM_BUTTONS];
    bool _was_over;
    bool _unsent_outside;

    // That's for onDrag
    bool _is_dragging[NUM_BUTTONS];
    Pos  _touch_pos  [NUM_BUTTONS];
    int  _drag_radius;

    // That's for whileHold
    double   _delay;
    double   _period;
    Timer    _last_time [NUM_BUTTONS];
    double   _current_dt[NUM_BUTTONS];
    Timer    _last_time_idle;
    double   _current_idle_dt;
    void     Idle();

    // That's for onKeyRelease
    SortedVector<int> _pressed_keys;
    BitVector         _key_mask;
    BitVector         _on_path_key_mask;
    bool              _likes_chars;
    bool              _likes_focus;
    int               _activation_key;

    //-- Handle some default actions bound to keys
    bool HandleKeyPress( int key_code, bool press ); 
    bool MarkKey( int key_code, bool as_pressed );

    bool _result;
    static MethodPool<ActiveWidget, const Widget::Request > *_handlers;
    static void  Init();
    static void  Destroy();
    friend class PreInitializer<ActiveWidget>;
};

DECLARE_PREINITIALIZER( ActiveWidget, ActiveWidget, USELESS_API );

};//namespace Useless

#endif//__INCLUDED_USELESS_ACTIVE_WIDGET_H__
