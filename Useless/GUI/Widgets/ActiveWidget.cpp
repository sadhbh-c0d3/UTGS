#include "UselessPch.h"

#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Display/HintMan.h"
#include "Useless/GUI/DebugGUI.h"
#include "Useless/GUI/Form.h"
#include "Useless/System/KeyCodes.h"

namespace Useless {

	INIT_PREINITIALIZER_ABSTRACT( ActiveWidget, ActiveWidget, USELESS_API );
    
	MethodPool<ActiveWidget, const Widget::Request > *ActiveWidget::_handlers;

    void ActiveWidget::Init()
    {
        _handlers = new MethodPool<ActiveWidget, const Widget::Request >;
        _handlers->Add( &ActiveWidget::AcceptInputCursor,    InputCursor()  );
        _handlers->Add( &ActiveWidget::AcceptInputButton,    InputButton()  );
        _handlers->Add( &ActiveWidget::AcceptDoubleClick,    DoubleClick()  );
        _handlers->Add( &ActiveWidget::AcceptInputWheel ,    InputWheel()   );
        _handlers->Add( &ActiveWidget::AcceptInputReset ,    InputReset()   );
        _handlers->Add( &ActiveWidget::AcceptInputKeyboard , InputKeyboard());
        _handlers->Add( &ActiveWidget::AcceptInputChar ,     InputChar()    );
        _handlers->Add( &ActiveWidget::AcceptInputFocus,     InputFocus()   );
        _handlers->Add( &ActiveWidget::AcceptInputAccel,     InputAccel()   );
    }

    void ActiveWidget::Destroy()
    {
        delete _handlers;
    }
    //////////////////////////////////////////////////////////////////////////

    /* Simple helper (maybe it is only W/A) functor which sends
     * actor movement notifications to parent who sends it to its parent an so on...
     */
    struct FocussIt : public PackedFunc
    {
        virtual void operator()() throw()
        {
            ptr->GetParent()->OnActorMove.Send_1( r + ptr->GetPosition() ); 
        }

        FocussIt( Widget *ptr, const VariableHolder<Rect> &r ): ptr(ptr), r(r) {}
        Widget *ptr; VariableHolder<Rect> r;
    };


    ActiveWidget::ActiveWidget()
    {
        int i;
        for (i=0; i!=NUM_BUTTONS; ++i)
        {
            _was_touched[i] = false;
            _is_dragging[i] = false;
            _mb_down[i] = false;
        }
        _was_over = false;
        _unsent_outside = false;
        _drag_radius = 2;
        _period = 10;
        _delay  = 100;
        _cursor = Pos(-1,-1);
        _likes_focus = false;
        _likes_chars = false;
        _activation_key = -1;
        _result = false;
        Activate();
        Tie2Signal( OnIdle, this, &ActiveWidget::Idle );

        /* Init Hints
           ----------------------------------------------------------*/
        //Tie2Signal( OnMoveOver,    this, &ActiveWidget::ShowHint );
        //Tie2Signal( OnMoveOutside, this, &ActiveWidget::HideHint );

        /* Default action if actor moves
           ----------------------------------------------------------*/
        OnActorMove.Tie( PkFunc(new FocussIt(this, OnActorMove.GetVar1())) );

        for (i=0; i!=NUM_BUTTONS; ++i)
            Tie2Signal( OnMoveOut[i], this, &ActiveWidget::HideHint );
    }

    void ActiveWidget::Setup()
    {
        assert( GetEnvironment() );
        GetEnvironment()->GetForm().RegisterAccelKey( _activation_key, this );
    }

    void ActiveWidget::Cleanup()
    {
        assert( GetEnvironment() );
        GetEnvironment()->GetForm().RegisterAccelKey( _activation_key, NULL );
    }

    void ActiveWidget::SetActivationKey( int key_code )
    {
        if ( GetEnvironment() )
        {
            GetEnvironment()->GetForm().RegisterAccelKey( key_code, this );
        }
        _activation_key = key_code;
    }

    bool ActiveWidget::AcceptInputCursor( const InputCursor &cur)
    {
        DBG_AcceptC
            int i;
        _cursor=cur.Position();
        _result=false;
        bool drag = false;
        bool was_over = IsOver(_cursor);

        if ( !IsActive() )
        {
            PointerIdle( _cursor, false );
            _was_over = was_over;
        }
        else if ( !IsOver(_cursor) || cur.Cond()==Widget::NORMAL )
        {
            for (i=0; i!=NUM_BUTTONS; ++i)
            {
                if (_mb_down[i]) { _result |= PointerMove(_cursor,i); drag=true; }
                if (_is_dragging[i]) { _result |= OnDrag[i].Send_3(_cursor,cur.Move(),_touch_pos[i]); }
            }
            if (!drag)
            {
                _result |= PointerIdle(_cursor, drag);
                _result |= WhileMoving.Schedule_3(_cursor,cur.Move(),_was_over);
            }
            _was_over = was_over; // prevent from changing shape "on-events"
        }
        else
        {
            if ( _was_over )
            {
                DBG_Text("------");
                for (i=0; i!=NUM_BUTTONS; ++i)
                {
                    if (_mb_down[i] && cur.Cond()==Widget::OFFSIDE ) 
                    { 
                        _result |= OnMoveOut[i].Schedule_1(_cursor); InputDecEager(this);
                        DBG_Text("MoveOut+");
                    }
                    else if ( cur.Cond()==Widget::INACTIVE )
                    {
                        if (_mb_down[i]) InputDecEager(this);
                        _is_dragging[i] = _mb_down[i] = false;
                        DBG_Text("ForceOut");
                    }
                }

                _result |= OnMoveOutside.Schedule_1(_cursor); HideHint();

                InputDecEager(this);
                _was_over = false;
                DBG_Text("OffSide");
            }

            if ( cur.Cond()==Widget::OFFSIDE )
            {
                _result |= WhileMoving.Schedule_3( _cursor, cur.Move(), _was_over);
            }
        }
        return ( _result = _was_over && (cur.Cond()==Widget::NORMAL) );
    }

    bool ActiveWidget::AcceptInputWheel( const InputWheel &wheel)
    {
        // Wheel has some idiotic interaction with middle button
        //if ( NoButtons() ) while (_input_intercept) { InputRelease(); }

        if (!IsActive()) return false;

        if ( wheel.Cond()==Widget::NORMAL ) 
        { 
            DBG_Text("OnWheel"); _result = OnWheel.Schedule_2( wheel.Z(), wheel.Dz() ); 
        }
        else
        { 
            DBG_Text("OffWheel"); _result = OffWheel.Schedule_2( wheel.Z(), wheel.Dz() ); 
        }
        return ( _result = _was_over && (wheel.Cond()==Widget::NORMAL) );//_result;
    }

    bool ActiveWidget::AcceptInputButton( const InputButton &mb)
    {
        if (!IsActive()) return false;
        DBG_AcceptB
            _result = false;

        int i;
        for (i=0; i!=NUM_BUTTONS; ++i)
        {
            if ( mb.Changed()&(1<<i) )
            {
                if ( mb.Status()&(1<<i) )
                {
                    if ( 0 == _mb_down[i] ) //< also set by keyboard
                    {
                        if ( mb.Cond()==Widget::NORMAL )
                        {
                            _mb_down[i] = true;
                            _result |= PointerDown(_cursor,i);
                        }
                        else
                        {
                            DBG_Text(">  .  <");
                            _result |= OffPress[i].Schedule_1(_cursor);
                            _was_touched[i]=false;
                        }
                    }
                }
                else
                {
                    if ( 1 == _mb_down[i] )
                    {
                        _mb_down[i] = false;
                        _result |= PointerUp(_cursor,i);
                    }
                }
            }
        }
        return (_result = _was_over && (mb.Cond()==Widget::NORMAL) );//_result;
    }

    bool ActiveWidget::AcceptDoubleClick( const DoubleClick &db_clk )
    {
        _result = false;
        if ( db_clk.Cond() == Widget::NORMAL )
        {
            _result |= OnDoubleClick[ db_clk.ButtonNo() ].Send_1(_cursor);
        }
        return (_result = _was_over && (db_clk.Cond()==Widget::NORMAL) );//_result;
    }


    bool ActiveWidget::AcceptInputKeyboard( const InputKeyboard &key )
    {
        _result = false; //-- Key is handled till something handle it

        if ( !key.Pressed() && key.KeyCode()==_activation_key )
        {// magic sequence of signal sends - to give nice mouse button like effet
            if ( 1 == _mb_down[0] )
            {
                _mb_down[0] = false;
                OnRelease[0].Schedule();
                OnClick[0].Schedule();
                InputDecEager(); NotifyAncestors(this);
            }
        }

        if (!OnFocusWay()) //-- Only widget with focus operates with keyboard
        {
            _pressed_keys.Clear();//-- Clear stored keyboard status
            return false;
        }
        bool likes_this_key = _on_path_key_mask.GetBit( key.KeyCode() );
        if ( HasFocus() ) { likes_this_key |= _key_mask.GetBit( key.KeyCode() ); }

        //-- Local state may be invalid, since focus changes
        bool invalid_state = !IsKeyPressed(key.KeyCode()) && !WasKeyPressed(key.KeyCode());
        if ( invalid_state ) return _result=likes_this_key;

        //-- Store local status
        bool state_changed  = MarkKey( key.KeyCode(), key.Pressed() );

        if ( !likes_this_key ) //-- If key is not handled by derivate, then
        {
            return _result=HandleKeyPress( key.KeyCode(), key.Pressed() );//-- apply internal keyboard handling
        }

        if (!IsActive()) return false; //-- Only ActiveWidget may send signals

        _result = ( _likes_chars && Keys::IsChar(key.KeyCode()) )? true : false;

        if ( !likes_this_key ) { return _result; }
        //-- If key state has changed
        if ( state_changed )
        {   
            if ( key.Pressed() )
            {
                if ( key.Cond() == Widget::FOCUSED )
                {
                    _result=OnKeyPress.Schedule_1( key.KeyCode());
                }
                else
                {
                    _result=OffKeyPress.Schedule_1( key.KeyCode());
                }
            }
            else
            {
                if ( key.Cond() == Widget::FOCUSED )
                {
                    _result=OnKeyRelease.Schedule_1( key.KeyCode()); 
                }
            }
        }
        else //-- If key state repeats
        {
            assert( key.Pressed() );

            if ( key.Cond() == Widget::FOCUSED )
            { 
                _result=WhileKey.Schedule_1( key.KeyCode() ); 
            }
            else
            { 
                _result=WhileOffKey.Schedule_1( key.KeyCode() ); 
            }
        }

        return (_result = _result && (key.Cond() == Widget::FOCUSED) );
    }

    bool ActiveWidget::AcceptInputChar( const InputChar &ch)
    {
        if (!IsActive() || !_likes_chars ) return false;

        if ( ch.Cond() == Widget::FOCUSED )
        {
            _result=OnChar.Schedule_1( ch.CharCode());
        }
        else
            _result=OffChar.Schedule_1( ch.CharCode());

        return (_result = _result && (ch.Cond() == Widget::FOCUSED) );
    }

    bool ActiveWidget::AcceptInputFocus( const InputFocus& )
    {
        if ( IsActive() && _likes_focus && GetEnvironment() )
        {
            GetEnvironment()->GetForm().SetFocussed(this);
            return _result=true;
        }
        return _result=false;
    }

    bool ActiveWidget::AcceptInputAccel( const InputAccel &key )
    {
        //-- Simulate mouse button click for activation key

        if ( key.KeyCode() == _activation_key )
        {
            if ( IsKeyPressed(key.KeyCode()) )
            {
                if ( 0 == _mb_down[0] )
                {
                    _mb_down[0] = true;
                    OnPress[0].Schedule();
                    InputIncEager();
                }
            }
            return _result=true;
        }
        return _result=false;
    }

    bool ActiveWidget::Accept(const Request &request)
    {
        return (*_handlers)( *this, request ) && _result;
    }

    bool ActiveWidget::NoButtons() const
    {
        int i;
        for (i=0; i!=NUM_BUTTONS; ++i)
        {
            if (_mb_down[i]) { return false; }
        }
        return true;
    }

    void ActiveWidget::Activate() 
    {
        _is_active=true;
        OnActivate.Send_1(_is_active); 
    }

    void ActiveWidget::Deactivate() 
    {
        _is_active=false;
        Accept( InputReset() );
        OnActivate.Send_1(_is_active); 
    }


    bool ActiveWidget::PointerIdle(const Pos& p, bool drag)
    {
        bool result=false;
        if ( IsOver(p) && !_was_over )
        { 
            DBG_Text("----->");
            if ( !IsActive() ) { ShowHint(); }
            else if (!drag) { result |= OnMoveOver.Schedule_1(p); ShowHint(); }
            InputIncEager(this);
            if (!drag) DBG_Text("MoveOver");
        }
        else if( _was_over && !IsOver(p) || _unsent_outside ) 
        { 
            DBG_Text("<-----");
            if ( !IsActive() ) { HideHint(); }
            else if (!drag) { result |= OnMoveOutside.Schedule_1(p); HideHint(); }
            InputDecEager(this);
            if (!drag) DBG_Text("MoveSide");
            _unsent_outside = false;
        }
        return result;
    }

    bool ActiveWidget::PointerDown(const Pos& p, int btn_no)
    {
        if ( IsOver(p) ) 
        { 
            _touch_pos[btn_no] = p;
            _was_touched[btn_no]=true;
            OnPress[btn_no].Schedule_1(p); 
            DBG_Text(">>><<<");
            InputIncEager(this); // increase for press
            InputIncEager(this); // increase for move-in
            DBG_Text("Press");
            _last_time[btn_no] = Timer();
            _current_dt[btn_no] = _delay;
            return true;
        }
        else
        {
            DBG_Text(">  .  <");
            OffPress[btn_no].Schedule_1(p);
            _was_touched[btn_no]=false;
        }
        return false;
    }

    bool ActiveWidget::PointerMove(const Pos& p, int btn_no)
    {
        bool result=false;
        if (!_is_dragging[btn_no] && _was_touched[btn_no] )
        {
            Pos r = _touch_pos[btn_no]-p;
            if ( ( r.x*r.x + r.y*r.y ) > _drag_radius )  
            { 
                _is_dragging[btn_no] = true;
                OnCatch[btn_no].Schedule_1( _touch_pos[btn_no] );
                DBG_Text("Drag");
            }
        }

        if ( _was_touched[btn_no] && !_was_over && IsOver(p) )
        {
            result |= OnMoveIn[btn_no].Schedule_1(p);
            DBG_Text("-----}");
            InputIncEager(this);
            DBG_Text("MoveIn");
            _unsent_outside = false;
        }
        else if ( _was_touched[btn_no] && _was_over && !IsOver(p) )
        {
            result |= OnMoveOut[btn_no].Schedule_1(p);
            DBG_Text("{-----");
            InputDecEager(this);
            DBG_Text("MoveOut");
            _unsent_outside = true;
        }
        else if ( _was_touched[btn_no] ) { return true; }
        return result;
    }

    bool ActiveWidget::PointerUp(const Pos& p, int btn_no)
    {
        bool result=false;
        if ( _was_touched[btn_no] )
        {
            DBG_Text("<<<>>>");
            if ( _was_over && IsClickPossibile(p,btn_no) )
            {
                result |= OnClick[btn_no].Schedule_1(p);
                InputDecEager(this); // decrease for moveout
            }
            result |= OnRelease[btn_no].Schedule_1(p);
            result |= OnDrop[btn_no].Send_2(p, _touch_pos[btn_no]);
            InputDecEager(this); // decrease for release
            DBG_Text("Release");
        }

        ActiveWidget::AcceptInputCursor( InputCursor( p, Pos(), (_was_over)? Widget::NORMAL : Widget::OFFSIDE) );
        _is_dragging[btn_no]=false;
        _was_touched[btn_no]=false;
        return result;
    }

    void ActiveWidget::Idle()
    {
        if ( IsActive() )
        {
            Timer actual_time;
            int i;
            for (i=0; i!=NUM_BUTTONS; ++i)
            {
                if ( _was_touched[i] && ( actual_time - _last_time[i] >= _current_dt[i]*0.001 ) )
                {
                    _current_dt[i] = _period;
                    _last_time[i]  = actual_time;
                    if ( IsOver(_cursor) ) { WhileHold[i].Schedule_1(_cursor); }
                    WhileDrag[i].Schedule_1(_cursor);
                    //DBG_Info_0("WhileDrag");
                }
            }
        }
    }

    bool ActiveWidget::InputIsEager () const
    {
#ifndef NDEBUG
        if ( (!_input_eager) && (_was_over || !NoButtons()) )
        {
            DBG_Text("!!!Eager");
        }
#endif

        return _input_eager>0 || _was_over || !NoButtons();
    }

    void ActiveWidget::AcceptInputReset( const InputReset &request)
    {
        Widget *sender = request.Sender();
        _cursor = Pos(-1,-1);
        int i;

        for (i=0; i!=NUM_BUTTONS; ++i)
        {
            if ( _was_touched[i] ) 
            { 
                if (_was_over) OnMoveOut[i].Schedule_1( _cursor );
                OnRelease[i].Schedule_1( _cursor ); 
            }
            _was_touched[i] = false;
            _is_dragging[i] = false;
            _mb_down[i] = false;
        }

        for (i=0; i!=_pressed_keys.Size(); ++i)
        {
            bool likes_this_key = false;
            likes_this_key |= _on_path_key_mask.GetBit( _pressed_keys[i] );
            likes_this_key |= HasFocus() && _key_mask.GetBit( _pressed_keys[i] );
            if ( likes_this_key )
            {
                OnKeyRelease.Schedule_1( _pressed_keys[i]);
            }
        }
        _pressed_keys.Clear();

        if (_was_over || _unsent_outside)
        {
            OnMoveOutside.Schedule_1( _cursor ); HideHint();
        }

        _was_over = false;
        _unsent_outside = false;
        _result = false;
        Widget::Accept( InputReset(this) );
    }


    void ActiveWidget::SetKeyboardMask( const BitVector &key_mask )
    {
        _key_mask = key_mask;
    }

    void ActiveWidget::EnableKey( int key_code, bool t )
    {
        if (t) { _key_mask.SetBit( key_code ); }
        else   { _key_mask.ClearBit( key_code ); }
    }

    void ActiveWidget::EnableOnPathKey( int key_code, bool t )
    {
        if (t) { _on_path_key_mask.SetBit( key_code ); }
        else   { _on_path_key_mask.ClearBit( key_code ); }
    }

    void ActiveWidget::EnableCharInput( bool t )
    {
        _likes_chars = t;
    }

    void ActiveWidget::ShowHint()
    {
        if ( GetEnvironment() )
        {
            assert( !!GetEnvironment()->GetHintMan() );
            GetEnvironment()->GetHintMan()->ShowHint(this);
        }
    }

    void ActiveWidget::HideHint()
    {
        if ( GetEnvironment() )
        {
            assert( !!GetEnvironment()->GetHintMan() );
            GetEnvironment()->GetHintMan()->HideHint(this);
        }
    }

    /* Obsolete method - is being moved to Form
     * current status: testing form (nearly to remove)
     */
    bool ActiveWidget::HandleKeyPress( int key_code, bool press )
    {
        return false;
    }

    bool ActiveWidget::MarkKey( int key_code, bool as_pressed )
    {
        if ( !_pressed_keys.FindID(key_code) )
        {
            if ( as_pressed )
            {
                _pressed_keys.Insert(key_code);
                InputIncEager( this );
                return true;
            }
        }
        else
        {
            if ( !as_pressed )
            {
                _pressed_keys.Remove();
                InputDecEager( this );
                return true;
            }
        }
        return false;
    }

    //-- Retrive global (real) keyboard status
    bool ActiveWidget::IsKeyPressed(int key_code ) const
    {
        if ( GetEnvironment() )
        {
            return GetEnvironment()->GetWorkspace().IsKeyPressed( key_code );
        }
        else
        {
            return false;
        }
    }

};//namespace Useless
