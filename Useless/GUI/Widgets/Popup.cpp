#include "UselessPch.h"

#include "Popup.h"
#include "Useless/GUI/Relations/FillRelation.h"
#include "Useless/GUI/Workspace.h"

namespace Useless {

Popup::Popup( Widget *content,
              const CaptionStyle &title_style,
              const Text &title,
              int title_height,
              bool modal ): _modal( modal )
{
    int w = content->GetWidth();
    int h = content->GetHeight();

    _p_titlebar = new Titlebar( title, title_style );

    if ( !title_height )
    {
        title_height = _p_titlebar->GetHeight();
    }

    _p_titlebar->Resize( w, title_height );

    _p_content = content;

    SetDimensions( w, h + title_height );

    HubWidget::Insert( _p_titlebar );
    HubWidget::Insert( _p_content );

    InitPopup();

    _p_content_fill = new FillRelation(*_p_content);
}

Popup::Popup( int w, int h,
              const CaptionStyle &title_style,
              const Text &title,
              int title_height,
              bool modal ): _modal( modal ), _p_content(NULL), _p_content_fill(NULL)
{
    _p_titlebar = new Titlebar( title, title_style );

    if ( !title_height )
    {
        title_height = _p_titlebar->GetHeight();
    }

    _p_titlebar->Resize( w, title_height );

    SetDimensions( w, h + title_height );
    HubWidget::Insert( _p_titlebar );
    InitPopup();
}

void Popup::InitPopup()
{
    _p_titlebar->Posses( new FillRelation(*_p_titlebar) );
    _p_titlebar->OnDrag[0].TieUnary( this, &Widget::Move, _p_titlebar->OnDrag[0].GetVar2() );
    _p_titlebar->OnPress[0].TieVoid( this, &Popup::NotifyCycler );
    Tie2Signal( OnVisible, this, &Popup::NotifyCycler );

    FormWidget::OnSubmit.TieVoid( false, this, &Widget::Hide );
    FormWidget::OnSubmit.TieVoid( true, this, &Widget::Hide );
}

Popup::~Popup()
{
    delete _p_content_fill;
}

void Popup::Setup()
{
    HubWidget::Setup();
    FormWidget::Setup();
}

Rect Popup::GetClientRect( const Widget *sender ) const
{
    Rect rc = GetBoundingRect();
    int title_height = _p_titlebar->GetHeight();

    if ( sender == _p_titlebar )
    {
        rc.SetH( title_height );
    }
    else //assert ( sender == _p_content )
    {
        rc.SetY1( title_height );
    }
    return rc;
}

void Popup::SetTitle( const Text &title )
{
    _p_titlebar->SetCaption( title );
}

Text Popup::GetTitle() const
{
    return _p_titlebar->GetCaption();
}

void Popup::ActivatePopupImp()
{
    _p_titlebar->SetSelected( true );
    GetParent()->BringOnTop( this );

    if ( _modal && (_input_intercept<1 || !!_input_interceptor && _input_intercept<2) ) 
    { 
        InputIntercept();
    }
    EnableForm( true );
    FocusIntercept();
}

void Popup::DeactivatePopupImp()
{
    EnableForm( false );
    if ( _modal ) { InputRelease(); }
    _p_titlebar->SetSelected( false );
}

void Popup::NotifyCycler()
{
    if ( GetEnvironment() )
    {
        GetEnvironment()->GetWorkspace().VisibleNotify( this );
    }
}

void Popup::SetContent( Widget *p_content )
{
    assert( !_p_content );
    _p_content = p_content;
    HubWidget::Insert( _p_content );
    _p_content_fill = new FillRelation(*_p_content);
}

Widget* Popup::RetrieveContent()
{
    delete _p_content_fill;
    _p_content_fill = NULL;
    HubWidget::Retrieve( _p_content );
    return _p_content;
}

};//namespace Useless
