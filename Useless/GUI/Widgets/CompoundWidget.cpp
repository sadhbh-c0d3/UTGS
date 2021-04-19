#include "UselessPch.h"

#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Widgets/CompoundWidget.h"
#include "Useless/System/TypeInfo.h"

#define __ERROR_LEVEL_5__
#define   WIDGET_NOTICE1(e) SMALL_LOG(e, "CompoundWidget")
#define   WIDGET_NOTICE2(e) MEDIUM_LOG(e, "CompoundWidget")

#include "Useless/GUI/DebugGUI.h"

namespace Useless {

extern bool g_PaintWidgetBoundaries;

///////////////////////////////////////////////////////////////////////////////
//  INIT Stuff
//
INIT_PREINITIALIZER_ABSTRACT( CompoundWidget, CompoundWidget, USELESS_API );
MethodPool<CompoundWidget, const Widget::Request > *CompoundWidget::_handlers;

void CompoundWidget::Init()
{
    _handlers = new MethodPool<CompoundWidget, const Widget::Request >;
    _handlers->Add( &CompoundWidget::IterInputCursor, InputCursor() );
    _handlers->Add( &CompoundWidget::IterInputButton, InputButton() );
    _handlers->Add( &CompoundWidget::IterDoubleClick, DoubleClick() );
    _handlers->Add( &CompoundWidget::IterInputWheel , InputWheel() );
    _handlers->Add( &CompoundWidget::IterInputKeyboard , InputKeyboard() );
    _handlers->Add( &CompoundWidget::IterInputChar , InputChar() );
}

void CompoundWidget::Destroy()
{
    delete _handlers;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//  Basic Methods
//
CompoundWidget::CompoundWidget(): _input_interceptor(0)
{
    Tie2Signal( OnIdle,  this, &CompoundWidget::DispatchIdle );
}

void CompoundWidget::Repaint(const WidgetPainter& painter )
{
    if ( 0 == OnPaint.Size() )
    {
        DoRepaint( painter );
    }
    else
    {
        OnPaint.Send_1(painter); 
    }
}

void CompoundWidget::DoRepaint(const WidgetPainter &painter)
{
     RepaintAction(painter);
    RefIter it = FirstChild();
    it.EvalFwd( PackFunctor( BinaryCall( this, &CompoundWidget::IterRepaint ), it, &painter ) );
}

void CompoundWidget::GetAcceptStatus(bool *pointer_was_over_widget, bool *widget_accepted_request)
{
    if (pointer_was_over_widget) *pointer_was_over_widget = _was_top;
    if (widget_accepted_request) *widget_accepted_request = _handled;
}

bool CompoundWidget::Accept(const Request &request)
{
    _handled = false;
    _was_top = false;

    if ( typeid(request) == typeid(InputCursor) )
    {
        InputCursor input_cursor = ((InputCursor&)request);
        _cursor = input_cursor.Position();
    }
    else if ( typeid(request) == typeid(InputReset) )
    {
        Widget::DoInputReset(this);
        _input_interceptor = NULL;
    }
    else if ( typeid(request) == typeid(InputFocus) )
    {
        return false;
    }
    
    RefIter it = LastChild();
    it.EvalBwd( PackFunctor( BinaryCall( this, &CompoundWidget::IterAccept ), it, &request ) );
    return _handled;
}

void CompoundWidget::InputIntercept(Widget *sender)
{
    if ( sender==this || !sender )
    {
        if ( (0== (!!_input_interceptor)+(_input_intercept++)) && GetParent() ) 
        { 
            GetParent()->InputIntercept(this); 
        }
        _input_interceptor = NULL;
    }
    else if ( IsChild(sender) )
    {
        assert( !sender->IsPhantom() );
        if (_input_interceptor && sender!=_input_interceptor)
        {
            _input_interceptor->InputRelease(this);
        }
        if ( (0== (!!_input_interceptor)+(_input_intercept)) && GetParent() )
        { 
            GetParent()->InputIntercept(this); 
        }
        _input_interceptor = sender;
    }
    DBG_IntCur;

    if ( GetEnvironment() && !*GetParent() ) 
    {
        GetEnvironment()->GetWorkspace().NoticeInputIntercept(true,this);
    }
}

void CompoundWidget::InputRelease(Widget *sender)
{
    if ( sender==this || !sender )
    {
        if ( (0== (!!_input_interceptor)+( _input_intercept>0 ? --_input_intercept : 0)) && GetParent() ) 
        { 
            GetParent()->InputRelease(this); 
        }
    }
    else if ( IsChild(sender) )
    {
        if (_input_interceptor && sender!=_input_interceptor)
        {
            _input_interceptor->InputRelease(this);
        }
        if ( (0== _input_intercept) && GetParent() )
        { 
            GetParent()->InputRelease(this); 
        }
        _input_interceptor = NULL;
    }
    else if ( sender==GetParent() )
    {
        if ( _input_interceptor) { _input_interceptor->InputRelease(this); }
        _input_intercept = NULL;
        _input_interceptor = NULL;
    }
    DBG_RelCur;

    if ( GetEnvironment() && !*GetParent() ) 
    {
        GetEnvironment()->GetWorkspace().NoticeInputIntercept(false,this);
    }
}

//! if enviro is NULL Cleanup() occures !
void CompoundWidget::SetEnvironment( const WidgetEnvironment *enviro)
{
   Widget::SetEnvironment( enviro );
   RefIter it = FirstChild();
   it.EvalFwd( PackFunctor( UnaryCall(this, &CompoundWidget::IterSetEnvironment), it ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Method called to Repaint every single widget
//
void CompoundWidget::IterRepaint( Widget *child, const WidgetPainter* p_painter)
{
    WidgetPainter its_painter( *p_painter, *child);

    if ( child->IsVisible() )
    {
        if ( its_painter.Intersect() )
        {
            its_painter.MultiplyAlpha( child->GetOpacity() );

            child->Repaint(its_painter);

            if ( g_PaintWidgetBoundaries )
            {
                its_painter.PaintBoundary( child->GetBoundingRect(),
                        0xAA0099, 0xAA0099, 0xAA0099, 0xAA0099 );
            }

            its_painter.RestoreClipper();
            
            if ( child->IsAlwaysDirty() )
            {
                child->SetDirty();
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Handlers called ForEach() widget
//
void CompoundWidget::IterInputCursor( const InputCursor &input)
{
    Widget *who = _last_who;
    Pos pos = who->GetPosition();
    InputCursor request( input.Position()-pos, input.Move(), GetCondition( who, input.Cond() ) );
    if (request.Cond()>-1)
    {   _handled |= who->Accept(request);
    }
}

void CompoundWidget::IterInputButton( const InputButton &input)
{
    Widget *who = _last_who;
    InputButton request( input.Status(), input.Changed(), GetCondition( who, input.Cond() ) );
    if (request.Cond()>-1)
    {   _handled |= who->Accept(request);
    }
}

void CompoundWidget::IterDoubleClick( const DoubleClick &input)
{
    Widget *who = _last_who;
    DoubleClick request( input.ButtonNo(), GetCondition( who, input.Cond() ) );
    if (request.Cond()>-1)
    {   _handled |= who->Accept(request);
    }
}


void CompoundWidget::IterInputWheel ( const InputWheel &input)
{
    Widget *who = _last_who;
    InputWheel request( input.Z(), input.Dz(), GetCondition( who, input.Cond() ) );
    if (request.Cond()>-1)
    {   _handled |= who->Accept(request);
    }
}

void CompoundWidget::IterInputKeyboard( const InputKeyboard &input)
{
    Widget *who = _last_who;
    int condition = GetCondition( who, input.Cond() );
    condition = who->OnFocusWay() ? Widget::FOCUSED : condition;
    InputKeyboard request( input.KeyCode(), input.Pressed(), condition );
    if (request.Cond()>-1) _handled |= who->Accept(request);
}

void CompoundWidget::IterInputChar( const InputChar &input)
{
    Widget *who = _last_who;
    int condition = GetCondition( who, input.Cond() );
    condition = who->OnFocusWay() ? Widget::FOCUSED : condition;
    InputChar request( input.CharCode(), condition );
    if (request.Cond()>-1) _handled |= who->Accept(request);
}

/////////////////////////////////////////////////////////////////////////////////////
//  Get Input Event Condidtion based on previous-condidtion and current-input-focus
//
int CompoundWidget::GetCondition( Widget *addr, int previous_cond)
{
    Pos     pos  = addr->GetPosition();

    bool inside_allowed_area = GetClientRect(addr).IsInside(_cursor.x, _cursor.y);
    inside_allowed_area = addr->IsVisible() ?  inside_allowed_area : false;
    bool is_over_child  = addr->IsOver(_cursor-pos);
    bool is_not_phantom = !addr->IsPhantom();
    bool is_inside_over = ( inside_allowed_area && is_over_child );

    bool top = _was_top;
    int condition;

    _was_top |= ( is_inside_over && is_not_phantom );

    if ( !top && (_was_top || is_inside_over) || addr->InputIsEager() || addr==_input_interceptor )
    {
        if ( previous_cond==Widget::INACTIVE || (_input_interceptor!=addr && _input_interceptor) )
            condition = Widget::INACTIVE;

        else if ( (!top && (_was_top || is_inside_over) || addr==_input_interceptor) && previous_cond==Widget::NORMAL )
            condition = Widget::NORMAL;

        else
            condition = Widget::OFFSIDE;
    }
    else
    {
        condition = -1;
    }
    return condition;
}

///////////////////////////////////////////////////////////////////////////////
//  Iterational Accept() is called ForEach() widget
//
void CompoundWidget::IterAccept( Widget *who, Request const* p_request)
{
    _last_who=who;

    if ( !(*_handlers)( *this, *p_request) )
    {
        if ( p_request->TellChilds() )
        {
            who->Accept(*p_request);
        }
        else if ( p_request->TellParent() && GetParent() )
        {
            GetParent()->Accept(*p_request);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool CompoundWidget::IsFullSolid   () const
{
    return true;
}

void CompoundWidget::GetSolidArea  ( RectList &rects )
{
    rects.push_back( GetBoundingRect() );
}



///////////////////////////////////////////////////////////////////////////////
//
//
void CompoundWidget::IterSetEnvironment( Widget *who )
{
    who->SetEnvironment( GetChildrenEnvironment() );
}

void CompoundWidget::IterIdleCalls( Widget *who, int t, int dt)
{
    if( who->IsVisible() )
    {
        _handled |= who->OnIdle.Send_2(t,dt);
    }
}

bool CompoundWidget::DispatchIdle(int t, int dt)
{
    _handled = false;
    /*if (_input_interceptor)
    {
        _handled |= _input_interceptor->OnIdle.Send_2(t,dt);
        return _handled;
    }*/

    RefIter it = FirstChild();
    it.EvalFwd( PackFunctor( TernaryCall( this, &CompoundWidget::IterIdleCalls ), it, t, dt ) );
    return _handled;
}

CompoundWidget* CompoundWidget::Ancestor( Widget *whose, Widget* &child)
{
    Widget *q, *p = whose->GetParent(); q = whose;
    while (p)
    {
        if ( CompoundWidget *cw = dynamic_cast<CompoundWidget*>(p) )
        {
            child = q;
            return cw;
        }
        q = p;
        p = p->GetParent();
    }
    child = NULL;
    return NULL;
}

CompoundWidget& CompoundWidget::SafeAncestor( Widget &whose, Widget* &child)
{
    CompoundWidget *cw = Ancestor( &whose, child);
    if (!cw) { APP_ERROR(Error("SafeAncestor(): No CompoundWidget ancestors found"),"CompoundWidget"); }
    return *cw;
}

};//namespace Useless

#undef WIDGET_NOTICE2
#undef WIDGET_NOTICE1
