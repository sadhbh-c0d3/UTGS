#include "UselessPch.h"

#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/GUIMaster.h"
#include "Useless/GUI/Display/ScreenMan.h"
#include "Useless/GUI/Widgets/NullWidget.h"

#include "Useless/ErrorConfig.h"
#if !defined(__GUI_DEBUG_LEVEL_NONE__)
#   define   BIGLOG(e) HUGE_LOG(e, "Workspace")
#   define    WSLOG(e) SMALL_LOG(e, "Workspace")
#else
#   define   BIGLOG(e)
#   define    WSLOG(e)
#endif
#define   WSWARN(e) APP_WARNING(e, "Workspace")

namespace Useless {

Workspace::Workspace( GUIMaster &gui )
    : _gui( gui )
	, _desktop_layer( 0, 0 )
	, _popups_layer( 0, 0 )
	, _menus_layer( 0, 0 )
	, _hints_layer( 0, 0 )
    , _dirty_lock(0)
    , _input_intercepting_layer(0)
    , _refresh_pointer_accepts(0)
{}

void Workspace::SetEnvironment( const WidgetEnvironment *s_env )
{
    if ( s_env )
    {
        const WidgetEnvironment &env = *s_env; //gui.GetEnvironment();

        WidgetEnvironment *penv;
        _desktop_layer.Posses( penv = new WidgetEnvironment(env) );
        penv->p_WidgetAllowedToSend = &_desktop_layer;
        penv->p_SendDirtyOutside    = UnaryCall( this, &Workspace::AcceptDirtyDesktop );
        _desktop_layer.SetEnvironment( penv );

        _popups_layer.Posses( penv = new WidgetEnvironment(env) );
        penv->p_WidgetAllowedToSend = &_popups_layer;
        penv->p_SendDirtyOutside    = UnaryCall( this, &Workspace::AcceptDirtyPopups );
        _popups_layer.SetEnvironment( penv );

        _menus_layer.Posses( penv = new WidgetEnvironment(env) );
        penv->p_WidgetAllowedToSend = &_menus_layer;
        penv->p_SendDirtyOutside    = UnaryCall( this, &Workspace::AcceptDirtyMenus );
        _menus_layer.SetEnvironment( penv );

        _hints_layer.Posses( penv = new WidgetEnvironment(env) );
        penv->p_WidgetAllowedToSend = &_hints_layer;
        penv->p_SendDirtyOutside    = UnaryCall( this, &Workspace::AcceptDirtyHints );
        _hints_layer.SetEnvironment( penv );
    }
    else
    {
        _hints_layer.SetEnvironment(NULL);
        _menus_layer.SetEnvironment(NULL);
        _popups_layer.SetEnvironment(NULL);
        _desktop_layer.SetEnvironment(NULL);
    }

    _p_focussed = NULL;
}

Workspace::~Workspace()
{
    SetEnvironment(NULL);
}

void Workspace::AddToDesktop ( Widget *widget, const Pos &p )
{
    _desktop_layer.Insert( widget, p.x, p.y );
}

void Workspace::AddPopup ( Popup *popup, const Pos &p  )
{
    popup->Hide(); _popups_layer.Insert( popup, p.x, p.y );
}

void Workspace::AddMenu ( Widget *menu_widget, const Pos &p )
{
    _menus_layer.Insert( menu_widget, p.x, p.y );
}

void Workspace::AddHint ( Widget *hint_widget, const Pos &p )
{
    _hints_layer.Insert( hint_widget, p.x ,p.y );
}

void Workspace::Remove ( Widget *widget )
{
    if (!_desktop_layer.Remove(widget))
    if (!_popups_layer.Remove(widget))
    if (!_menus_layer.Remove(widget))
    if (!_hints_layer.Remove(widget))
    WSWARN( Error("Widget %0x does not belong to this workspace", widget) );
}

Widget* Workspace::Retrieve ( Widget *widget )
{
    if (!_desktop_layer.Retrieve(widget))
    if (!_popups_layer.Retrieve(widget))
    if (!_menus_layer.Retrieve(widget))
    if (!_hints_layer.Retrieve(widget))
       { WSWARN( Error("Widget %0x does not belong to this workspace", widget) ); return 0; }
    return widget;
}

Rect Workspace::GetWorkspaceRect() const
{
    return _desktop_layer.GetBoundingRect();
}


void  Workspace::Repaint()
{
    _desktop_layer.GetEnvironment()->GetTrashCan().Purify();
    if ( _desktop_layer.IsVisible() )
    _desktop_layer.Repaint( _gui.GetScreenMan() );
     _popups_layer.Repaint( _gui.GetScreenMan() );
      _menus_layer.Repaint( _gui.GetScreenMan() );
      _hints_layer.Repaint( _gui.GetScreenMan() );
}

void  Workspace::Resize ( int w, int h )
{
    _desktop_layer.Resize( w, h );
    _popups_layer.Resize( w, h );
    _menus_layer.Resize( w, h );
    _hints_layer.Resize( w, h );
}
    
int Workspace::GetWidth() const
{
    assert( _desktop_layer.GetWidth() == _popups_layer.GetWidth() &&
            _desktop_layer.GetWidth() == _menus_layer.GetWidth() &&
            _menus_layer.GetWidth() == _hints_layer.GetWidth() );
    return _desktop_layer.GetWidth();
}

int Workspace::GetHeight() const
{
    assert( _desktop_layer.GetHeight() == _popups_layer.GetHeight() &&
            _desktop_layer.GetHeight() == _menus_layer.GetHeight() &&
            _menus_layer.GetHeight() == _hints_layer.GetHeight() );
    return _desktop_layer.GetHeight();
}

void  Workspace::SetDirty ( const Rect &dirty )
{
#if !defined(__GUI_DEBUG_LEVEL_NONE__) // Used to debug window repaint
    if(!! dirty) { BIGLOG( Error("SetDirty [ %i %i %i %i]", dirty.GetX(), dirty.GetY(), dirty.GetW(), dirty.GetH() ) ); }
    else         { WSLOG ( Error("SetDirty [ %i %i %i %i]", dirty.GetX(), dirty.GetY(), dirty.GetW(), dirty.GetH() ) ); }
#endif
    Rect d = (!dirty)? _desktop_layer.GetBoundingRect() : dirty;

    ResendDirty( d, DESKTOP | POPUPS | MENUS | HINTS | SCREEN );
}

void  Workspace::ResendDirty( const Rect &dirty, int flags )
{
    if (!_dirty_lock++)
    {
     if ( flags & DESKTOP)  _desktop_layer.SetDirty(dirty);
     if ( flags & POPUPS )   _popups_layer.SetDirty(dirty);
     if ( flags & MENUS  )    _menus_layer.SetDirty(dirty);
     if ( flags & HINTS  )    _hints_layer.SetDirty(dirty);
     if ( flags & SCREEN ) _gui.GetScreenMan().AcceptDirtyRect(dirty);
     _dirty_lock = false;
    }
}

void  Workspace::AcceptDirtyDesktop( const Rect &dirty )
{
    ResendDirty( dirty, POPUPS | MENUS | HINTS | SCREEN );
}

void  Workspace::AcceptDirtyPopups ( const Rect &dirty )
{
    ResendDirty( dirty, DESKTOP | MENUS | HINTS | SCREEN  );
}

void  Workspace::AcceptDirtyMenus ( const Rect &dirty )
{
    ResendDirty( dirty, DESKTOP | POPUPS | HINTS | SCREEN  );
}

void  Workspace::AcceptDirtyHints ( const Rect &dirty )
{
    ResendDirty( dirty, DESKTOP | POPUPS | MENUS | SCREEN  );
}

bool Workspace::ResendRequest( const Widget::Request &request, const Widget::Request &alt_request, int flags )
{
    _hints_accept_status = _menus_accept_status = _popups_accept_status = _desktop_accept_status = false;
    bool done, h = false;

    if ( AnyVisibleMenus() ) { flags = MENUS | HINTS; }

    //( !flags || flags & HINTS   ) {   _hints_accept_status = _hints_layer.Accept(request); 
    //_hints_layer.GetAcceptStatus(&done,0); h|=done; } //@todo: hints

    _menus_accept_status = _menus_layer.Accept(((!flags || flags&MENUS)&& !h)? request:alt_request);
    _menus_layer.GetAcceptStatus(&done,0); h|=done;

    _popups_accept_status = _popups_layer.Accept(((!flags || flags&POPUPS)&& !h)? request:alt_request);
    _popups_layer.GetAcceptStatus(&done,0); h|=done;

    if ( _desktop_layer.IsVisible() )
    {
        _desktop_accept_status = _desktop_layer.Accept(((!flags || flags&DESKTOP)&& !h)? request:alt_request);
        _desktop_layer.GetAcceptStatus(&done,0); h|=done;
    }

    return _hints_accept_status | _menus_accept_status | _popups_accept_status | _desktop_accept_status;
}

void Workspace::RestoreFocus()
{
    bool forceRestore = !_p_focussed || !_p_focussed->IsRealyVisible();
    
    if ( AnyVisibleMenus())
    {
        if (forceRestore || _p_focussed==&_menus_layer && 0==_desktop_layer.GetFocussed())
        {
            _menus_layer.Accept( Widget::InputFocus());
        }
    }
    else if ( AnyVisiblePopups())
    {
        if (forceRestore || _p_focussed==&_popups_layer && 0==_desktop_layer.GetFocussed())
        {
            _popups_layer.Accept( Widget::InputFocus());
        }
    }
    else
    {
        if ( forceRestore || _p_focussed==&_desktop_layer && 0==_desktop_layer.GetFocussed())
        {
            _desktop_layer.Accept( Widget::InputFocus());
        }
    }
}

void  Workspace::AcceptInputCursor ( const Pos &pos, const Pos &dpos )
{
    RestoreFocus();
    Widget::InputCursor     request( pos, dpos, Widget::NORMAL);
    Widget::InputCursor alt_request( pos, dpos, (!_input_intercepting_layer ? Widget::OFFSIDE : Widget::INACTIVE) );
    ResendRequest(request, alt_request, (_input_intercepting_layer)? _input_intercepting_layer | MENUS | HINTS : 0 );
    return;
}

void  Workspace::AcceptInputButton ( int status, int changed )
{
    Widget::InputButton     request( status,changed, Widget::NORMAL);
    Widget::InputButton alt_request( status,changed, (!_input_intercepting_layer ? Widget::OFFSIDE : Widget::INACTIVE) );
    ResendRequest(request, alt_request, (_input_intercepting_layer)? _input_intercepting_layer | MENUS | HINTS : 0 );

    if (status&changed) { HideHints(); }
    bool k; _menus_layer.GetAcceptStatus(&k,0); if ( (status&changed) && !k ) { HideMenus(); }

    return;
}

void  Workspace::AcceptDoubleClick ( int button_no )
{
    Widget::DoubleClick     request( button_no, Widget::NORMAL);
    Widget::DoubleClick alt_request( button_no, (!_input_intercepting_layer ? Widget::OFFSIDE : Widget::INACTIVE) );
    ResendRequest(request, alt_request, (_input_intercepting_layer)? _input_intercepting_layer | MENUS | HINTS : 0 );

    return;
}


void  Workspace::AcceptInputWheel ( int zpos, int dz )
{
    Widget::InputWheel     request( zpos, dz, Widget::NORMAL);
    Widget::InputWheel alt_request( zpos, dz, (!_input_intercepting_layer ? Widget::OFFSIDE : Widget::INACTIVE) );
    ResendRequest(request, alt_request, (_input_intercepting_layer)? _input_intercepting_layer | MENUS | HINTS : 0 );
    return;
}

void  Workspace::AcceptInputChar ( int code )
{
    Widget::InputChar     request( code, Widget::NORMAL);
    Widget::InputChar alt_request( code, Widget::INACTIVE );
    ResendRequest(request, alt_request, _focus_holding_layer );
    return;
}

void  Workspace::AcceptInputKey ( int key, bool pressed )
{
    if ( pressed ) { _pressed_keys_map.SetBit( key ); }
    else           { _pressed_keys_map.ClearBit( key ); }

    Widget::InputKeyboard     request( key, pressed, Widget::FOCUSED );
    Widget::InputKeyboard alt_request( key, pressed, Widget::INACTIVE );
    bool status =
        ResendRequest(request, alt_request, _focus_holding_layer );

    if ( pressed && !status ) { OnKey.Switch(key); }
    return;
}

void  Workspace::AcceptIdleCalls ( int T, int Dt )
{
    RestoreFocus();
    if ( _refresh_pointer_accepts )
    {
        _refresh_pointer_accepts = false;
        AcceptInputCursor( WhereIsPointer(), Pos() );
    }
    _desktop_layer.OnIdle.Send_2( T, Dt);
     _popups_layer.OnIdle.Send_2( T, Dt);
      _menus_layer.OnIdle.Send_2( T, Dt);
      _hints_layer.OnIdle.Send_2( T, Dt);
}

void  Workspace::NoticeInputIntercept( bool t, Widget *sender )
{
    if (t)
    {
        if ( _input_intercepting_layer!=InWhichLayer(sender) ) switch (_input_intercepting_layer)
        {
            /* NullWidget::InstancePtr() forces complete (non-decremental) InputRelease
            ---------------------------------------------------------------------------*/
            case DESKTOP: _desktop_layer.InputRelease(NullWidget::InstancePtr()); break;
            case POPUPS :  _popups_layer.InputRelease(NullWidget::InstancePtr()); break;
            case MENUS:     _menus_layer.InputRelease(NullWidget::InstancePtr()); break;
            case HINTS:     _hints_layer.InputRelease(NullWidget::InstancePtr()); break;
        }
    
        if (sender==&_desktop_layer) _input_intercepting_layer = DESKTOP; else
        if (sender==&_popups_layer ) _input_intercepting_layer = POPUPS;  else
        if (sender==&_menus_layer  ) _input_intercepting_layer = MENUS;   else
        if (sender==&_hints_layer  ) _input_intercepting_layer = HINTS;   else return;
    }
    else
    {
        switch (_input_intercepting_layer)
        {
            case DESKTOP: 
                if (sender==&_desktop_layer){ _input_intercepting_layer=false; } break;
            case POPUPS:  
                if (sender==&_popups_layer) { _input_intercepting_layer=false; } break;

            /* Menus may be a part of modal poups, after closing them we need to make those popups modal
            --------------------------------------------------------------------------------------------*/
            case MENUS:   
                if (sender==&_menus_layer)  {_input_intercepting_layer=false; ReactivatePopup(); } break;
            case HINTS:   
                if (sender==&_hints_layer)  {_input_intercepting_layer=false; ReactivatePopup(); } break;
        }
    }
}

void Workspace::NoticeWidgetMovement( Widget *sender )
{
    Pos p = WhereIsPointer();
    Rect bound = sender->GetAbsoluteRect( sender->GetBoundingRect() );
    if ( bound.IsInside(p.x,p.y) )
    {
        _refresh_pointer_accepts = true;
    }
}

void  Workspace::SetPreviousCursor()
{
    _gui.GetScreenMan().SetDefaultCursor();
}

void  Workspace::SetCursor( const MouseCursor &mc)
{
    _gui.GetScreenMan().SetCursor(mc);
}

const MouseCursor& Workspace::GetCursor() const
{
    return _gui.GetScreenMan().GetCursor();
}

void  Workspace::ShowCursor()
{
    _gui.GetScreenMan().SetCursorVisible(true);
}

void  Workspace::HideCursor()
{
    _gui.GetScreenMan().SetCursorVisible(false);
}

bool  Workspace::IsCursorShown() const
{
    return _gui.GetScreenMan().IsCursorVisible();
}

void  Workspace::HideMenus()
{
    Widget::RefIter it = _menus_layer.FirstChild();
    it.EvalFwd( PackFunctor( VoidCall( it, &Widget::Hide ) ) );
    OnHideMenus.Send();
}

void  Workspace::HideHints()
{
    Widget::RefIter it = _hints_layer.FirstChild();
    it.EvalFwd( PackFunctor( VoidCall( it, &Widget::Hide ) ) );
    OnHideHints.Send();
}

bool  Workspace::AnyVisibleHints()
{
    Widget::RefIter it = _hints_layer.FirstChild();
    for( ; !!it; ++it ) { if ( it->IsVisible() ) { return true; } }
    return false;
}

bool  Workspace::AnyVisibleMenus()
{
    Widget::RefIter it = _menus_layer.FirstChild();
    for( ; !!it; ++it ) { if ( it->IsVisible() ) { return true; } }
    return false;
}

bool  Workspace::AnyVisiblePopups()
{
    Widget::RefIter it = _popups_layer.FirstChild();
    for( ; !!it; ++it ) { if ( it->IsVisible() ) { return true; } }
    return false;
}

bool Workspace::IsKeyPressed( int key_code ) const 
{
    return _pressed_keys_map.GetBit( key_code ); 
}

Pos Workspace::WhereIsPointer() const
{
    return _gui.GetScreenMan().WhereIsPointer();
}

void  Workspace::ShowAtPointer( Widget *widget )
{
    ShowAtPosition( WhereIsPointer(), widget );
}

void  Workspace::ShowAtPosition( const Pos &p, Widget *widget )
{
    Pos rel_pos = widget->GetRelativePos( p );
    widget->Move( rel_pos );
    Rect client_area = widget->GetParent()->GetClientRect(widget);
    Rect widget_area = widget->GetBoundingRect() + widget->GetPosition();
    if ( (client_area&widget_area) != widget_area )
    {
        Pos repos;
        if ( widget_area.GetX1() < client_area.GetX1() ) { repos.x -= widget_area.GetX1() - client_area.GetX1(); } else
        if ( widget_area.GetX2() > client_area.GetX2() ) { repos.x -= widget_area.GetX2() - client_area.GetX2(); }
        if ( widget_area.GetY1() < client_area.GetY1() ) { repos.y -= widget_area.GetY1() - client_area.GetY1(); } else
        if ( widget_area.GetY2() > client_area.GetY2() ) { repos.y -= widget_area.GetY2() - client_area.GetY2(); }
        widget->Move( repos );
    }
    widget->Show();
}


int Workspace::InWhichLayer( Widget *widget ) const
{
    if ( (Widget*)(&_desktop_layer)==widget) { return DESKTOP_LAYER; } else
    if ( (Widget*)(&_popups_layer)==widget)  { return POPUPS_LAYER; } else
    if ( (Widget*)(&_menus_layer)==widget)   { return MENUS_LAYER; } else
    if ( (Widget*)(&_hints_layer)==widget)   { return HINTS_LAYER; } else

    if ( _desktop_layer.IsChild(widget)) { return DESKTOP_LAYER; } else
    if ( _popups_layer.IsChild(widget))  { return POPUPS_LAYER; } else
    if ( _menus_layer.IsChild(widget))   { return MENUS_LAYER; } else
    if ( _hints_layer.IsChild(widget))   { return HINTS_LAYER; } else
    return NO_LAYER;
}

void Workspace::SetFocussed( Widget *widget )
{
    assert( widget );
    if ( widget==&_desktop_layer )
    {
        _focus_holding_layer = DESKTOP;
    }
    else if ( widget==&_popups_layer )
    {
        _focus_holding_layer = POPUPS;
    }
    else
    {
        _focus_holding_layer = MENUS;
    }
    if ( _p_focussed && widget!=_p_focussed) { _p_focussed->OffFocus.Send(); }
    _p_focussed = widget;
    _p_focussed->OnFocus.Send();
}

void Workspace::InvalidateFocus( Widget *sender )
{
    if ( !sender || sender!=_p_focussed ) 
    {
        _focus_holding_layer = false;
        _p_focussed = NULL;
        return;
    }

    if ( _p_focussed ) { _p_focussed->OffFocus.Send(); }

    if ( _focus_holding_layer==MENUS )
    {
        if ( !AnyVisibleMenus() )
        {
            _focus_holding_layer = POPUPS;
            _p_focussed = &_popups_layer;
            _popups_layer.OnFocus.Send();
        }
    }
    else if ( _focus_holding_layer==POPUPS )
    {
        if ( !AnyVisiblePopups() )
        {
            _focus_holding_layer = DESKTOP;
            _p_focussed = &_desktop_layer;
            _desktop_layer.OnFocus.Send();
        }
    }
    else
    {
        _focus_holding_layer = false;
        _p_focussed = NULL;
    }
}

Widget* Workspace::GetFocussed() const
{
    return _p_focussed;
}


};//namespace Useless
