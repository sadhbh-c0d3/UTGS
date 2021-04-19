#include "UselessPch.h"

#include "Useless/GUI/Widgets/ScrollbarWidget.h"
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Skins/NullSkin.h"
#include "Useless/GUI/Widgets/NullWidget.h"

namespace Useless {

ScrollbarWidget::ScrollbarWidget( int size, const SkinSet &skins, int size_limit ):
                 BridgeWidget( 0,0 ),
                 NoKeyButton( ExtractPageSkin(skins) ),
                 _advance(0), OnAdvance(_advance.OnUpdate),
                 _size(0),       OnSize(_size.OnUpdate),
                 _full(0),      OnRange(_full.OnUpdate)
{
    Init( size, skins, size_limit );
}

ScrollbarWidget::ScrollbarWidget( const ScrollbarStyle &style):
                 BridgeWidget( 0,0 ),
                 NoKeyButton( ExtractPageSkin(style.skins) ),
                 _advance(0), OnAdvance(_advance.OnUpdate),
                 _size(0),       OnSize(_size.OnUpdate),
                 _full(0),      OnRange(_full.OnUpdate)
{
    Init( style.size, style.skins, style.size_limit );
}

void ScrollbarWidget::Init( int size, const SkinSet &skins, int size_limit )
{
    /* Create pager
    ------------------------------------------------------------------------------*/   
    SkinSet ssPager;
   
    assert( skins.Exists(PAGER) );
        ssPager.Insert( DraggerLogic::NORMAL, skins.GetSkin(PAGER) );

    if ( skins.Exists(PAGER_PRESSED) ) 
        { ssPager.Insert( DisplayState::PRESSED, skins.GetSkin(PAGER_PRESSED) ); }
    else if ( skins.Exists(PAGERHL) ) 
        { ssPager.Insert( DisplayState::PRESSED, skins.GetSkin(PAGERHL) ); }
    else
        { ssPager.Insert( DisplayState::PRESSED, skins.GetSkin(PAGER) );}

    if ( skins.Exists(PAGERHL) ) 
        { ssPager.Insert( DisplayState::HIGHLIGHTEN, skins.GetSkin(PAGERHL) ); }

    if ( skins.Exists(PAGERDIS) ) 
        { ssPager.Insert( DisplayState::DISABLED, skins.GetSkin(PAGER) ); }
    else
        { ssPager.Insert( DisplayState::DISABLED, NullSkin() ); }

    _pager = BridgeWidget::WAttach( new DraggerWidget(ssPager) );

    

    /* Setup page
    ------------------------------------------------------------------------------*/   
    Pos  page_dir = skins.GetDirection(PAGE)*skins.GetDirMask(PAGE);
    Pos pager_dir = skins.GetDirection(PAGER)*skins.GetDirMask(PAGER);
    bool horizontal = page_dir.x && pager_dir.x;
    bool   vertical = page_dir.y && pager_dir.y;
    
    assert( horizontal || vertical 
            && "ScrollbarWidget::ScrollbarWidget(): Incompatibile PAGER and PAGE orientations");

    if (size < size_limit)
    {
        APP_WARNING( Error("Scrollbar size(%i) cannot be smaller than minimum pager size(%i)",
                            size, size_limit), "ScrollbarWidget");
        size = size_limit;
    }

    _size_limit = size_limit;
    _horizontal = horizontal;

    if (_horizontal)
        { NoKeyButton::Resize( size, NoKeyButton::GetHeight() ); }
    else 
        { NoKeyButton::Resize( NoKeyButton::GetWidth(), size ); }

    BridgeWidget::Resize( NoKeyButton::GetWidth(), NoKeyButton::GetHeight() );

    /* Tie on signals
    ------------------------------------------------------------------------------*/   
    Tie2Signal( _advance.OnUpdate, this, &ScrollbarWidget::UpdateScrollbar );
    Tie2Signal(    _size.OnUpdate, this, &ScrollbarWidget::UpdateScrollbar );
    Tie2Signal(    _full.OnUpdate, this, &ScrollbarWidget::UpdateScrollbar );
    Tie2Signal( _pager->OnDrag[0], this, &ScrollbarWidget::Drag );
    Tie2Signal(      WhileHold[0], this, &ScrollbarWidget::Page );

    Tie2Signal( OnActivate, _pager, &ActiveWidget::SetActive );

      _pager->OnPress[0].TieUnary( this, &ScrollbarWidget::OnPager, true );
    _pager->OnRelease[0].TieUnary( this, &ScrollbarWidget::OnPager, false );

    SetRepeatRate( 100, 0);
    _dragging_pager = false;
}

ScrollbarWidget::~ScrollbarWidget()
{}

void ScrollbarWidget::Resize(int w, int h)
{
    NoKeyButton::Resize( w, h);
    BridgeWidget::Resize( NoKeyButton::GetWidth(), NoKeyButton::GetHeight() );

    if ( ((_horizontal)? w:h) < _size_limit )
    {
        if ( _pager->IsVisible() ) { _pager->Hide(); }
    }
    else
    {
        UpdateScrollbar();
        if ( !_pager->IsVisible() ) { _pager->Show(); }
    }
}

void ScrollbarWidget::Repaint(const WidgetPainter &painter )
{
    NoKeyButton::Repaint( painter );
    BridgeWidget::Repaint( painter );
}


void ScrollbarWidget::SetAdvance(int advance)
{
    if (_full==0 || _size>_full) { _advance=0; }
    else
    {
        _advance = (advance<0)? 0 : (_size+advance < _full)? advance : _full-_size;
    }
}

void ScrollbarWidget::SetSize(int size)
{
    assert(size >= 0 && "ScrollbarWidget::SetSize()");
    _size = size;
}

void ScrollbarWidget::SetRange(int full)
{
    assert(full >= 0 && "ScrollbarWidget::SetRange()");
    _full = full;
}

void ScrollbarWidget::UpdateScrollbar()
{
    int h = (_horizontal)? GetWidth()+1 : GetHeight()+1;

    if ( h < _size_limit ) 
    {
        if ( _pager->IsVisible() ) { _pager->Hide(); }
    }
    else
    {
        int p = 0;

        if (_full==0 || _size>_full)
        {
            SetPGSize(h);
        }
        else
        {
            int s = (h*_size)/_full;
            int es = (s<_size_limit)? _size_limit:s;
            SetPGSize(es);
            p = ( (h+s-es)*_advance)/_full;
        }
        double w1 = (_horizontal)? _pager->GetHeight() : _pager->GetWidth();
        double w2 = (_horizontal)? GetHeight() : GetWidth();
        double x = (w2 - w1) /2;

        _pager->SetPosition( Pos( (_horizontal)? p:x, (_horizontal)? x:p ) );

        if ( !_pager->IsVisible() ) { _pager->Show(); }
    }
}

bool ScrollbarWidget::Accept( const Request& request)
{
    bool accepted = BridgeWidget::Accept(request);

    if ( !_dragging_pager || typeid(request)!=typeid(Widget::InputCursor) )
    { accepted |= ActiveWidget::Accept(request); }

    return accepted;
}

void ScrollbarWidget::Drag(const Pos& pos, const Pos& dp, const Pos& pick)
{
    Pos p = _pager->GetPosition()+pos-pick;
    double h = (_horizontal)? GetWidth() : GetHeight();
    double x = (_horizontal)? p.x : p.y;

    double s  = (h*_size)/_full;
    double es = (s<_size_limit)? _size_limit:s;
    double a = x * (_full/(h+s-es));
    SetAdvance( a );
}

void ScrollbarWidget::Page(const Pos& p)
{
    if ( !_dragging_pager )
    {
        Pos c = _pager->GetPosition();
        double w = (_horizontal)? _pager->GetWidth() : _pager->GetHeight();
        double u = (_horizontal)? c.x : c.y;
        double x = (_horizontal)? p.x : p.y;

        double h = (_horizontal)? _pager->GetHeight() : _pager->GetWidth();
        double y = (_horizontal)? p.y : p.x;
        if (y>0 && y<h)
        {
            if ( x < u ) SetAdvance( _advance-_size/2 );
            else if ( x > u+w ) SetAdvance( _advance+_size/2 );
        }
    }
}

void ScrollbarWidget::OnPager(bool cought)
{
    (_dragging_pager = cought)?  ActiveWidget::OnPress[0].Send() :
                                 ActiveWidget::OnRelease[0].Send();
}


SkinSet ScrollbarWidget::ExtractPageSkin(const SkinSet &skins)
{
    SkinSet ss_page;

    assert( skins.Exists(PAGE) );

    ss_page.Insert( ButtonLogic::NORMAL, skins.GetSkin(PAGE) );

    if ( skins.Exists(PAGE_PRESSED) )
        { ss_page.Insert( DisplayState::PRESSED,   skins.GetSkin(PAGE_PRESSED) ); }
    else if ( skins.Exists(PAGEHL) )
        { ss_page.Insert( DisplayState::PRESSED,   skins.GetSkin(PAGEHL) ); }
    else
        { ss_page.Insert( DisplayState::PRESSED,   skins.GetSkin(PAGE) ); }

    if ( skins.Exists(PAGEHL) )
        { ss_page.Insert( DisplayState::HIGHLIGHTEN, skins.GetSkin(PAGEHL) ); }

    if ( skins.Exists(PAGEDIS) )
        { ss_page.Insert( DisplayState::DISABLED, skins.GetSkin(PAGEHL) ); }

    return ss_page;
}

};//namespace Useless
