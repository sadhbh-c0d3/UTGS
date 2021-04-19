#include "UselessPch.h"

#include "Useless/GUI/Widgets/AdvListWidget.h"
#include "Useless/GUI/Relations/AdvScrollRelation.h"

namespace Useless {


AdvListWidget::AdvListWidget( const ListStyle      &list_style,
                              const FrameStyle     &frm_style,
                              const ScrollbarStyle &vsb_style )
{
    _pi_scrollbar = new ScrollbarWidget( vsb_style );
    _p_frame      = new FrameWidget(frm_style);
    _p_list       = new ListWidget(list_style);
    _p_scroller   = new ScrolledWidget;
    Init();
}

AdvListWidget::AdvListWidget( const ListStyle2     &list_style,
                              const FrameStyle     &frm_style,
                              const ScrollbarStyle &vsb_style )
{
    _pi_scrollbar = new ScrollbarWidget( vsb_style );
    _p_frame      = new FrameWidget(frm_style);
    _p_list       = new ListWidget(list_style);
    _p_scroller   = new ScrolledWidget;
    Init();
}
    
AdvListWidget::AdvListWidget( const ListStyle         &list_style,
                              const FrameStyle        &frm_style,
                              const AdvScrollbarStyle &vsb_style )
{
    _pi_scrollbar = new AdvScrollbarWidget( vsb_style );
    _p_frame      = new FrameWidget(frm_style);
    _p_list       = new ListWidget(list_style);
    _p_scroller   = new ScrolledWidget;
    Init();
}

AdvListWidget::AdvListWidget( const ListStyle2        &list_style,
                              const FrameStyle        &frm_style,
                              const AdvScrollbarStyle &vsb_style )
{
    _pi_scrollbar = new AdvScrollbarWidget( vsb_style );
    _p_frame      = new FrameWidget(frm_style);
    _p_list       = new ListWidget(list_style);
    _p_scroller   = new ScrolledWidget;
    Init();
}

void AdvListWidget::Init()
{
    _sb_width  = _pi_scrollbar->Atom().GetWidth();
    _p_frame->Insert( _p_scroller );
    _p_scroller->Fill();
    _p_scroller->AttachWidget(_p_list);
    _pi_scrollbar->Atom().Resize( _sb_width, _p_frame->GetHeight() );
    
    HubWidget::Resize( _sb_width+_p_frame->GetWidth(), _p_frame->GetHeight() );
    HubWidget::Insert( _p_frame );
    HubWidget::Insert( _pi_scrollbar->AtomPtr(), _p_frame->GetWidth() );
    
    _pi_scrollbar->Atom().Posses
    ( 
        new AdvScrollRelation( _p_scroller->Scrollee(), *_pi_scrollbar, true ) 
    );

    Tie2Signal( _p_list->OnResize, this, &AdvListWidget::Fit );
}

AdvListWidget::~AdvListWidget()
{
}


void AdvListWidget::ShowScrollbar()
{
    if(_pi_scrollbar->Atom().IsVisible()) return;
    _pi_scrollbar->Atom().Show();
    Resize(_p_frame->GetWidth(), _p_frame->GetHeight() );
}

void AdvListWidget::HideScrollbar()
{
    if(!_pi_scrollbar->Atom().IsVisible()) return;
    _pi_scrollbar->Atom().Hide();
    Resize( _sb_width+_p_frame->GetWidth(), _p_frame->GetHeight() );
}


void AdvListWidget::Resize( int w, int h)
{
    HubWidget::Resize( w,h);
    Fit();
}

void AdvListWidget::Fit()
{
    int w = GetWidth();
    int h = GetHeight();

    _pi_scrollbar->Atom().Resize( _sb_width, h);
    _pi_scrollbar->Atom().SetPosition( Pos(w-_sb_width,0) );
    _p_frame->Resize( w-_sb_width, h);
    _p_scroller->Fill();

    if(!_pi_scrollbar->Atom().IsVisible())
    {
        _p_frame->Resize( w,h);
        _p_scroller->Fill();
    }
    _p_list->SetSelectorSize( _p_scroller->GetWidth() );
}

void AdvListWidget::Repaint( const WidgetPainter &painter)
{
    HubWidget::Repaint( painter);
    Rect client_list_area = _p_frame->GetClientRect(_p_scroller);
    Rect list_area = _p_list->GetBoundingRect();
    int delta_h = client_list_area.GetH() - list_area.GetH();
    if ( delta_h > 0)
    {
        Rect area = client_list_area;
        area.SetH( delta_h);
        area.SetY( area.GetY() + client_list_area.GetH()-delta_h);
        
        _p_list->PaintArea( painter, area, ListLogic::NORMAL);
    }
}

};//namespace Useless    
