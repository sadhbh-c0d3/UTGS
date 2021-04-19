#include "UselessPch.h"

#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Widgets/Widget.h"

#include "Useless/System/TypeInfo.h"

#include "Useless/GUI/Widgets/NullWidget.h"
//#include "unused GUI/Widgets/BufferWidget.h" // workaroud? AlignTo

//#include "Useless/ErrorConfig.h"
#include "Useless/GUI/DebugGUI.h"

#ifdef DEBUG_CREATION
    #include <set>
    #include <stdio.h>
    std::list<int> __list_of_destructed_widgets;
    std::set<int>  __list_of_null_widgets;
    std::set<int>  __list_of_hints_widgets;
    #include "Useless/GUI/Widgets/SmallHint.h"
#endif

#include <cmath>

namespace {
    double as_double( int value ) { return (double)(value); }
};

namespace Useless {

int    Widget::_widget_last_id;

/*! Default widget ctor. Sets widget local flags and counters 
 *  (visibility flag, dirty couter, input-eager counter, input-intercept counter).
 *  It also sets default parent widget to NullWidget (its global instance).
 */
//-----------------------------------------------------------------
Widget::Widget()
    :_parent(NULL),
     _p_environment(NULL),
     _is_on_top(0),
     _dirty_lock(0),
     _always_dirty(0),
     _opacity(255),
     _is_phantom(0)
{
    _is_visible = true;
    _this_widget_id = ++_widget_last_id;

    _input_eager     = 0;
    _input_intercept = 0;
    _parent = NullWidget::InstancePtr();

    DBG_Create

#   ifdef DEBUG_CREATION
    FILE *pfile = fopen("Widgets.log","a+");
    fprintf( pfile, "Create: %i    ", _this_widget_id );
    fprintf( pfile, "\n" );
    fclose( pfile );
#   endif
}

/* Private constructor supplied only for NullWidget */
Widget::Widget(int): _parent(NULL), _p_environment(NULL), _is_on_top(0),
    _input_eager(0), _input_intercept(0), _is_visible(0), _dirty_lock(0)
{
    _this_widget_id = ++_widget_last_id;
    _parent = NULL;

    DBG_Create

#   ifdef DEBUG_CREATION
    FILE *pfile = fopen("Widgets.log","a+");
    fprintf( pfile, "Create NullWidget %i\n", _this_widget_id );
    fclose( pfile );
    __list_of_null_widgets.insert( _this_widget_id );
#   endif
}

Widget::Widget( const Widget & )
{
    assert( !"Widget cannot be copied !!!" );
}

Widget::~Widget()
{
    //assert( !*_parent && "Widget::~Widget(): Widget mustn't be attached");
    DBG_Destroy
    int debug_int = _this_widget_id;
    _personal_trash_can.Purify();

#   ifdef DEBUG_CREATION
    FILE *pfile = fopen("Widgets.log","a+");
    fprintf( pfile, "Destroy: %i    ", _this_widget_id );
    std::list<int>::iterator ite1 = __list_of_destructed_widgets.begin();
    std::list<int>::iterator ite2 = __list_of_destructed_widgets.end();
    for ( ;ite1!=ite2; ++ite1 )
    {
        if ( ( __list_of_null_widgets.find(*ite1) == __list_of_null_widgets.end() ) &&
             ( __list_of_hints_widgets.find(*ite1) == __list_of_hints_widgets.end() ) )
        { fprintf( pfile, " %i", *ite1 ); }
    }
    fprintf( pfile, "\n" );

    fclose( pfile );
    __list_of_destructed_widgets.push_back( _this_widget_id );
#   endif
}

void Widget::Setup()
{
    DBG_Text("Setup");

#   ifdef DEBUG_CREATION
    if ( typeid(*this)==typeid(SmallHint) ) __list_of_hints_widgets.insert( _this_widget_id );
#   endif
}

void Widget::Cleanup()
{
    DBG_Text("Cleanup");
    Accept( InputReset() );
    NotifyAncestors(false);
}


/*! Show widget. While it executes even if widget is allready shown,
 *  it may act like a refresh.
 */
void Widget::Show()
{
    _is_visible=true;
    if ( _is_on_top ) { GetParent()->BringOnTop(this); }

    NotifyAncestors(true);
    SetDirty( GetBoundingRect() );
    OnVisible.Send_1(true);
}

/*! Hide widget, which is not allready hidden.
 *  If widget is hidden, function does nothing.
 */
void Widget::Hide()
{ 
    if (IsVisible())
    {
        SetDirty(GetBoundingRect());
        _is_visible=false;
        Accept( InputReset(this) );
        NotifyAncestors(false);
        OnVisible.Send_1(false); 
    }
}

/*! Check if this widget is realy visibile on screen
 */
bool Widget::IsRealyVisible () const
{
    if ( _is_visible )
    {
        return (!*_parent)? true : _parent->IsRealyVisible();
    }
    return false;
}


/*! Mark widget to be placed on top whenever Show() is being executed.
 *  This is only temporary property of widget.
 */
void Widget::SetOnTop(bool t)
{
    if ( _is_on_top=t ) 
    {
        GetParent()->BringOnTop(this);
    }
}


// pos is absolute, method returns relative coords
Pos Widget::GetRelativePos(const Pos &pos ) const
{
    Pos offset = GetPosition();
        offset = _parent->GetAbsolutePos(offset); 
    return pos - offset;
}

// pos is relative, method returns absolute coords
Pos Widget::GetAbsolutePos(const Pos &pos ) const
{
    Pos offset = GetPosition();
        offset = _parent->GetAbsolutePos(offset); 
    return pos + offset;
}

// Gets Absolute Rectangle
Rect Widget::GetAbsoluteRect (const Rect &rect) const
{
    return rect + GetAbsolutePos( Pos(0,0) );
}

// Gets Relative Rectangle
Rect Widget::GetRelativeRect (const Rect &rect) const
{
    return rect + GetRelativePos( Pos(0,0) );
}


// Moves widget 
void Widget::Move(const Pos &dp)
{
    if ( GetParent() )
    {
        Pos p = GetPosition();
        SetPosition(p+dp);
    }
}

// Moves widget in Z direction
void Widget::Dive(int dz)
{
    GetParent()->Lift( -dz, this);
}

void Widget::Enlarge(const Pos &dsize)
{
    Pos p = GetPosition();
    int w = GetWidth()+dsize.x;
    int h = GetHeight()+dsize.y;
    if (w<0) { w=-w; p.x -= w; }
    if (h<0) { h=-h; p.y -= h; }
    
    SetPosition(p);
    Resize(w, h);
}

// Fills parent client-area with self
void Widget::Fill( const Rect& area, const Rect &padding )
{   
    Rect cr = area;
    if (!cr)
    {
        assert( _parent && "Widget::Fill(): Impossibile due no parent");
        cr = _parent->GetClientRect(this);
    }
    cr.x += padding.x;
    cr.y += padding.y;
    cr.w -= padding.w + padding.x;
    cr.h -= padding.h + padding.y;
    SetPosition( cr.GetPos() );
    Resize( cr.GetW(), cr.GetH() );
}

// Align Widget to another
void Widget::AlignTo( Widget const* widget, int alignment )
{
    //Rect rc = typeid(widget)!=typeid(BufferWidget) ? widget->GetBoundingRect() + widget->GetPosition() : widget->GetParent()->GetBoundingRect() + widget->GetParent()->GetPosition();
    Rect rc = widget->GetBoundingRect() + widget->GetPosition();
    Pos   p = GetPosition();

    int w=GetWidth(), h=GetHeight(), x=0, y=0, border=alignment&0x00FF, corner=alignment&0xFF00;
    
    switch( border )
    {
        case LEFT_BORDER:
            x = rc.GetX1() - w;
            y = corner==LU_CORNER ? rc.GetY1() : (corner==RB_CORNER ? rc.GetY2()-h : p.y);
            break;
        
        case RIGHT_BORDER:
            x = rc.GetX2();
            y = corner==LU_CORNER ? rc.GetY1() : (corner==RB_CORNER ? rc.GetY2()-h : p.y);
            break;

        case TOP_BORDER:
            y = rc.GetY1() - h;
            x = corner==LU_CORNER ? rc.GetX1() : (corner==RB_CORNER ? rc.GetX2()-w : p.x);
            break;

        case BOTTOM_BORDER:
            y = rc.GetY2();
            x = corner==LU_CORNER ? rc.GetX1() : (corner==RB_CORNER ? rc.GetX2()-w : p.x);
            break;
    }
    SetPosition( Pos(x,y) );
}

void Widget::Align( int inner_alignment)
{
    Rect client_area = GetParent()->GetClientRect( this);
    Rect bound = GetBoundingRect() + GetPosition();
    bound.Align( client_area, Rect::Alignment( inner_alignment));
    SetBoundingRect( bound);
}

/*! Place self inside parents-client area.
 *  Proportions are pos.{x|y} / base.{x|y}.
 *  If base.{x|y}==0 then position in {X|Y} direction will be preserved.
 */
void Widget::Place ( const Pos &pos, const Pos &base)
{
    Rect client_area = GetParent()->GetClientRect( this);
    Pos p = GetPosition();
    double x = p.x;
    double y = p.y;
    if( base.x > 0 ) { x = as_double( client_area.GetW() * pos.x ) / as_double( base.x ); }
    if( base.y > 0 ) { y = as_double( client_area.GetH() * pos.y ) / as_double( base.y ); }
    SetPosition( client_area.GetPos() + Pos( std::floor(x), std::floor(y) ));
}

/*! Place self inside parents-client area.
 *  Proportions are rect.{x|y|w|h} / base.{x|y|x|y}.
 *  If base.{x|y}==0 then position and size in {X|Y} direction will be preserved.
 */
void Widget::Place ( const Rect &rect, const Pos &base)
{
    Rect client_area = GetParent()->GetClientRect( this);
    Rect my_rect = GetBoundingRect() + GetPosition();
    double x = my_rect.GetX();
    double y = my_rect.GetY();
    double w = my_rect.GetW();
    double h = my_rect.GetH();
    if ( base.x > 0) { x = as_double( client_area.GetW() * rect.GetX() ) / as_double( base.x ); }
    if ( base.y > 0) { y = as_double( client_area.GetH() * rect.GetY() ) / as_double( base.y ); }
    if ( base.x > 0) { w = as_double( client_area.GetW() * rect.GetW() ) / as_double( base.x ); }
    if ( base.y > 0) { h = as_double( client_area.GetH() * rect.GetH() ) / as_double( base.y ); }
    SetPosition( client_area.GetPos() + Pos( std::floor(x), std::floor(y) ));
    Resize( std::ceil(w), std::ceil(h) );
}

void Widget::AlignSize ( const Rect &offsetAndSize, const Pos &placement )
{
    Rect my_rect = GetBoundingRect() + GetPosition();
    Rect rect = my_rect;
    // align width
    if ( offsetAndSize.w > 0 )
    {
        rect.w -= offsetAndSize.x;
        rect.w /= offsetAndSize.w;
        rect.w *= offsetAndSize.w;
        rect.w += offsetAndSize.x;
    }
    // align height
    if ( offsetAndSize.h > 0 )
    {
        rect.h -= offsetAndSize.y;
        rect.h /= offsetAndSize.h;
        rect.h *= offsetAndSize.h;
        rect.h += offsetAndSize.y;
    }
    rect.x = my_rect.x + ((my_rect.w - rect.w) * placement.x / 100);
    rect.y = my_rect.y + ((my_rect.h - rect.h) * placement.y / 100);
    // apply new size
    Resize( rect.w, rect.h );
    // apply new position
    SetPosition( rect.GetPos() );
}

// Methos allocates space for widget
void Widget::SetBoundingRect( const Rect &r )
{
    Rect previous = _bound;
    _bound = r;
    bool anything_changed=false;

    if ( GetEnvironment() )
    {
        _parent->SetDirty( previous | _bound );
    }
    if ( _bound.GetX() != previous.GetX() || _bound.GetY() != previous.GetY() )
    {
        OnReposition.Send_1( Pos( _bound.GetX(), _bound.GetY() ));
        anything_changed=true;
    }
    if ( _bound.GetW() != previous.GetW() || _bound.GetH() != previous.GetH() )
    {
        OnResize.Send_2( _bound.GetW(), _bound.GetH() );
        anything_changed=true;
    }
    if ( anything_changed )
        NotifyAncestors(true);
}

// Method calculates area that is dirty and sets it dirty
void Widget::SetExposed( const Rect &area_before_changes )
{
    Rect current_area = GetBoundingRect();
    if ( (current_area & area_before_changes)!= area_before_changes )
    {
        SetDirty( current_area | area_before_changes );
        OnResize.Send_2( current_area.GetW(), current_area.GetH() );
    }
    else
    {   SetDirty( current_area );
    }
}

// Method sends dirty rectangle to parent, and eventually to environment
void Widget::SetDirty( const Rect &dirty )
{
    DBG_Dirty; assert( _dirty_lock>= 0 );

    if ( _dirty_lock == 0 )
    {
        // send dirty to environment
        if (!!_p_environment) 
        { 
            _p_environment->SendDirtyOutside( dirty, this ); 
        }
        // send dirty to parent
        if ( _parent && _parent->_p_environment && _parent->IsVisible() )
        { 
            Rect dirty_parent = ( dirty + GetPosition() ) & _parent->GetClientRect(this);
            _parent->SetDirty( dirty_parent );
        }
    }
}


void Widget::InputIncEager(Widget *sender)
{
    assert ( sender!=_parent && "Widget::InputIncEager(): Cycle detected");
    if ( 0 == _input_eager++ && _parent && sender!= _parent )
    {
        if (_parent) _parent->InputIncEager( this );
    }
    DBG_IncEager
}

void Widget::InputDecEager  (Widget *sender)
{
    assert ( sender!=_parent && "Widget::InputDecEager(): Cycle detected");
    if (_input_eager<=0 ) { DBG_Error return; }
    if ( 0 == --_input_eager && _parent && sender!= _parent )
    {
        if (_parent) _parent->InputDecEager( this );
    }
    DBG_DecEager
}

void Widget::InputIntercept (Widget *sender)
{
    if ( sender==_parent )
    { 
        DBG_Text("InputIntercept");
        DBG_Error return; 
    }
    else if ( !_is_phantom )
    {
        if ( 0 == _input_intercept++ )
        { 
            if ( _parent && sender!=_parent )
            {
                if ( _parent ) { _parent->InputIntercept( this ); }
            }
        }
    }
    DBG_IntCur
}

void Widget::InputRelease (Widget *sender)
{
    assert(_parent);
    if ( sender==_parent || _is_phantom )
    {
        _input_intercept = 0;
        DBG_Text("ForcedRelease");
        return;
    }
    else if (_input_intercept<=0 ) 
    { 
        DBG_RelCur;
        DBG_Error return; 
    }
    else if ( --_input_intercept==0 )
    { 
        _parent->InputRelease( this );
    }
    DBG_RelCur
}

void Widget::DoInputReset(Widget *sender)
{
    assert(_parent);
    DBG_Text("DoInputReset");
    if (_input_intercept>0) 
    {
        _input_intercept=0;
        _parent->InputRelease( this );
    }
    if (_input_eager>0)
    {
        _input_eager=0;
        _parent->InputDecEager( this );
    }
    if ( HasFocus() )
    {
        FocusRelease();
    }
}

void Widget::FocusIntercept()
{
    if (_p_environment)
    {
        _p_environment->GetForm().SetFocussed(this);
        if ( !HasFocus() ) 
        { APP_WARNING(Error("Widget %i could not intercept focus", _this_widget_id),"Widget"); 
        }
    }
}

void Widget::FocusRelease()
{
    if (_p_environment)
    {
        _p_environment->GetForm().InvalidateFocus( this );
    }
}

bool Widget::HasFocus() const
{
    if (_p_environment)
    {
        return this == _p_environment->GetForm().GetFocussed();
    }
    return false;
}

bool Widget::OnFocusWay() const
{
    if (_p_environment)
    {
        Widget *p = _p_environment->GetForm().GetFocussed();

        for ( ; !!p && !!*p && this!=p; ) 
        {
            p=p->GetParent();
        }
        return p==this;
    }
    return false;
}


void Widget::Attach()
{
    DBG_Attach;
}

void Widget::Detach()
{
    DBG_Detach;
    Accept( InputReset(this) );
}


bool Widget::Accept(const Request &request)
{
    #ifndef NDEBUG
         if ( typeid(request)==typeid(InputCursor) ) DBG_AcceptC
    else if ( typeid(request)==typeid(InputButton) ) DBG_AcceptB
    #endif

    if ( typeid(request)==typeid(InputReset) )
    {
        DBG_Text("Accept(InputReset)");
        DoInputReset( request.Sender() );
    }
    else if ( typeid(request)==typeid(InputFocus) )
    {
        return false;
    }
    if ( request.TellParent() && _parent ) 
    { 
        _parent->Accept(request); 
    }
    return false;
}

void Widget::SetMouseCursor(MouseCursor &mc) const
{
    assert(_p_environment); _p_environment->GetWorkspace().SetCursor(mc);
}

void Widget::RestoreMouseCursor () const
{
    assert(_p_environment); _p_environment->GetWorkspace().SetPreviousCursor();
}

const MouseCursor& Widget::GetMouseCursor() const
{
    assert(_p_environment); return _p_environment->GetWorkspace().GetCursor();
}

void Widget::SetEnvironment( const WidgetEnvironment *enviro)
{
    if ( _p_environment && !enviro ) { Cleanup(); }
    if ( _p_environment=enviro )     { Setup();   }
}

void Widget::SetParent(Widget* parent)
{   
    assert(_parent&&"Widget::SetParent(): Parent must be Widget or NullWidget");
    assert( !( parent&&(!!*parent)&&(!!*_parent) ) && "Widget::SetParent(): Allready has parent");
    
    // Detach only if current parent wasn't a NullWidget
    if ( !!*_parent ) 
    { 
        Detach(); 
    }

    // Parent must be Widget or NullWidget, it mustn't be NULL
    _parent = parent ? parent : NullWidget::InstancePtr();

    // Attach only if new parent is not a NullWidget
    if ( !!*_parent ) 
    { 
        Attach(); 
    }
}

void Widget::NotifyAncestors( bool I_exist )
{
    assert( _parent);
    if ( _p_environment && !!*_parent )
    {
        _p_environment->GetWorkspace().NoticeWidgetMovement( this );
    }
}


Widget::RefIter Widget::FirstChild() const 
{ 
    return new Iterator; 
}

Widget::RefIter Widget::LastChild () const 
{ 
    return new Iterator; 
}

};// namespace Useless
