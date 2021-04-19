#include "UselessPch.h"

#include "Useless/GUI/Widgets/FormWidget.h"
#include "Useless/Functional/DelayedExecutor.h"
#include "Useless/System/KeyCodes.h"

namespace Useless {

FormWidget::FormWidget()
{
    Tie2Signal( OnFocus, this,  &FormWidget::AssignFocus );
    Tie2Signal( OffFocus, this, &FormWidget::UnassignFocus );
    Tie2Signal( OnKeyPress, &OnKey, &SignalSwitch<int>::Switch );
    Tie2Signal( OnKeyPress, this, &FormWidget::HandleKey );
    //Tie2Signal( WhileKey, &OnKey, &SignalSwitch<int>::Switch );
    Tie2Signal( WhileKey, this, &FormWidget::HandleKey );
    _dont_cycle = false;
    _focus_locked = false;
    _form_enabled = true;
    _actual_focus = NULL;
    _stored_focus = NULL;
    _default_focus = NULL;
    _submit_key = -1;
    _cancel_key = -1;
    EnableOnPathKey( Keys::UP );
    EnableOnPathKey( Keys::DOWN );
    EnableOnPathKey( Keys::LEFT );
    EnableOnPathKey( Keys::RIGHT );
    EnableOnPathKey( Keys::TAB ); //testing phase - still doesn't work
}

FormWidget::~FormWidget()
{
}

void FormWidget::Setup()
{
    if ( _submit_key<0 ) SetSubmitKey( Keys::RET );
    if ( _cancel_key<0 ) SetCancelKey( Keys::ESC );
    ActiveWidget::Setup();
}

void FormWidget::EnableForm( bool t )
{
    if (! (_form_enabled = t ) && HasFocus() )
        { FocusRelease(); }
}

//-- Probe focus for each widget, till some get it (forward scan)
void FormWidget::CycleFocus( int mode )
{
    if (!!_cycler && !_dont_cycle )
    {
        if ( mode==NEXT || mode==PREVIOUS )
        {
            Widget *begin = _cycler->Who();
            bool accepted = false;
            int  loops=2;//W/A @todo
            int  dbg_iterations = 0;
            int  dbg_n_visible = 0;
            do {
                switch (mode)
                {
                    case NEXT:     loops -= !_cycler->CycleForward(); break;
                    case PREVIOUS: loops -= !_cycler->CycleBack();    break;
                }
                if ( !loops ) { break; }
                
                if ( !_cycler->Who() ) { loops=0; break; }// Break if cycler is not valid
                
                if ( _cycler->Who()->IsRealyVisible() ) // Widget must be visible
                {
                    Widget *next = _cycler->Who();
                    if ( &next->GetEnvironment()->GetForm() == this )
                    accepted = next->Accept( InputFocus() ); 
                    
                    ++dbg_n_visible;
                }
                ++dbg_iterations;

            } while ( !accepted && begin!=_cycler->Who() && loops>0 );

            if ( accepted )
            {
                C_assert( _actual_focus == _cycler->Who());
            }
            else
            {
                if (!loops) { CycleParentForm( mode ); }
                else
                {
                    assert( begin==_cycler->Who() );
                    if ( begin->IsRealyVisible() && begin!=this )
                    {
                        accepted = begin->Accept( InputFocus() );
                    }
                    else
                    {
                        FocusRelease();
                    }
                }
            }
        }// end if (mode==NEXT || mode==PREVIOUS)
    }
}


//-- Mark given widget as focussed
void FormWidget::SetFocussed( Widget *widget )
{
    C_assert( &widget->GetEnvironment()->GetForm() == this );
    //C_assert( _actual_focus != widget);
    _dont_cycle = true; 
    Widget *previous = _actual_focus;
    if ( _cycler->Set(widget) && GetEnvironment() && widget!=_actual_focus )
    {
        if ( !HasFocus() )
        {
            Form &parent_form= GetEnvironment()->GetForm();
            parent_form.SetFocussed(this); 
        }
        _actual_focus = _cycler->Who();
        _actual_focus->OnFocus.Send();
        C_assert( _actual_focus != previous);
        if ( previous ) { previous->OffFocus.Send(); }
    }
    _dont_cycle = false;
}

//-- Mark focus invalid. New focus will be assigned after idle calls.
void FormWidget::InvalidateFocus( Widget *sender )
{
    if ( _actual_focus )
    {
        _focus_locked=true; _actual_focus->OffFocus.Send();
        _actual_focus=NULL; _focus_locked=false;
    }
    if ( sender && _actual_focus==sender )
    {
        OnTimer::Instance().Schedule(
            PackFunctor( UnaryCall( this, &FormWidget::CycleFocus ), NEXT )
            );
    }
    else
    {
        FocusRelease();
    }
}

//-- Return information about who has actual focus in whole GUI
Widget* FormWidget::GetFocussed() const
{
    return (HasFocus())?  ( (_focus_locked)? (Widget*)this : _actual_focus) : NULL;
}


//-- Add InputFocus handler
//-- FormWidget overloads Accept to accept focuses
//-- It also bypasses any other Requests to both
//-- Compound and Active parts of this widget
bool FormWidget::Accept( const Request& request )
{
    if ( typeid(request)==typeid(InputFocus) )
    {
        if ( _form_enabled )
            { FocusIntercept(); }
        
        if( HasFocus() )
            { AssignFocus(); return true; }
        else
            { return false; }
    }
    else
    {
        if ( !CompoundWidget::Accept( request ) && 
             !ActiveWidget::Accept( request ) )
            return false;
        else
            return true;
    }
}

//-- Set environment for this Form, and modify it for children
//-- if 'enviro' is NULL then Cleanup() occures
void FormWidget::SetEnvironment( const WidgetEnvironment *enviro )
{
    if (enviro)
    {
        _child_env = *enviro;
        _child_env.p_Form = this;
        _cycler = new WidgetCycler(this);
    }
    CompoundWidget::SetEnvironment( enviro );
}

//-- Obtain children environmnet
//-- method used by CompoundWidget when setting env. for each child
const WidgetEnvironment* FormWidget::GetChildrenEnvironment() const
{
    return (!GetEnvironment())? NULL : &_child_env; 
}


//-- Assign focus to any widget
//-- This action is being taken when this Form gains focus
void FormWidget::AssignFocus()
{
    assert( _form_enabled );

    if ( _actual_focus && !_actual_focus->IsRealyVisible())
    {
        InvalidateFocus(_actual_focus);
    }
    
    if ( !_actual_focus && !_dont_cycle )
    {
        if ( !_cycler || !_stored_focus )
        {
            if ( !_default_focus || !_default_focus->Accept(InputFocus()) )
            {
                _cycler = new WidgetCycler(this);
                CycleFocus( NEXT );
            }
            else 
            {
                C_assert( _default_focus == _cycler->Who() ); 
            }
        }
        else if ( _stored_focus ) // check if stored ptr is valid
        {
            _cycler = new WidgetCycler(this); 
            
            do if ( _cycler->Who()==_stored_focus )
            {
                SetFocussed(_stored_focus);
                break;
            }
            while ( _cycler->CycleForward());
          
            _stored_focus=NULL;
        }
    }
}

//-- Unassign focus
//-- This action is being taken when this Form loses focus
void FormWidget::UnassignFocus()
{
    if ( _actual_focus )
    {
        _actual_focus->OffFocus.Send();
        _stored_focus=_actual_focus;
        _actual_focus=NULL;
    }
}

//-- Tell parent form to focus other widget
//-- Method releases focus
void FormWidget::CycleParentForm( int mode )
{
    Form *pform = &GetEnvironment()->GetForm();
    pform->CycleFocus( mode );
}

void FormWidget::SetDefaultFocus( Widget *sender )
{
    _default_focus = sender;
}

void FormWidget::SetSubmitKey( int key_code )
{
    if ( _submit_key!=_cancel_key ) { ActiveWidget::EnableKey( _submit_key, false ); }
    ActiveWidget::EnableKey( _submit_key = key_code, true );
}

void FormWidget::SetCancelKey( int key_code )
{
    if ( _cancel_key!=_submit_key ) { ActiveWidget::EnableKey( _cancel_key, false ); }
    ActiveWidget::EnableKey( _cancel_key = key_code, true );
}

void FormWidget::RegisterAccelKey( int key_code, ActiveWidget *sender )
{
    AccelMap::ConstIterator i;
    if ( _accelerators.Find( key_code, &i ) ||
         _accelerators.FindID( sender, &i ) )
    {
        EnableOnPathKey( i.Key(), false );
        _accelerators.Remove(i);
    }
    if ( !!sender )
    {
        _accelerators.Insert( key_code, sender );
        EnableOnPathKey( key_code );
    }
}

void FormWidget::HandleKey( int key_code )
{
    if ( Widget **p = _accelerators.Find(key_code) )
    {
        if ( (*p)->IsRealyVisible() )
        {
            (*p)->Accept( Widget::InputAccel(key_code) );
        }
    }

    if ( key_code == _submit_key )
    {
        OnSubmit.Switch( true );
    }
    else if ( key_code == _cancel_key )
    {
        OnSubmit.Switch( false );
    }

    else if ( key_code == Keys::TAB )
    {
        bool backwards = IsKeyPressed( Keys::LSHIFT ) || IsKeyPressed( Keys::RSHIFT );
        bool context = IsKeyPressed( Keys::LCTRL ) || IsKeyPressed( Keys::RCTRL );
        Form *pform = (context)? &GetEnvironment()->GetForm() : this;
        assert( pform );
        pform->CycleFocus( (backwards)? PREVIOUS : NEXT );
    }
    else if ( key_code==Keys::UP || key_code==Keys::LEFT )
    {
        CycleFocus( PREVIOUS );
    }
    else if ( key_code == Keys::DOWN || key_code==Keys::RIGHT )
    {
        CycleFocus( NEXT );
    }
}

};//namespace Useless
