#include "UselessPch.h"

#include "Useless/GUI/Widgets/TableSystemWidget.h"
#include "Useless/GUI/Relations/AdvScrollRelation.h"
#include "Useless/GUI/Widgets/ScrolledWidget.h"
#include "Useless/GUI/Widgets/ScrollbarWidget.h"
#include "Useless/GUI/Widgets/SizerWidget.h"
#include "Useless/GUI/Widgets/TableWidget.h"
#include "Useless/GUI/Widgets/FrameWidget.h"
#include "Useless/GUI/Relations/ResizeRelation.h"
#include "Useless/Util/Math.h"

#include "Useless/ErrorConfig.h"

namespace Useless {

TableSystemWidget::~TableSystemWidget()
{}

TableSystemWidget::TableSystemWidget( int w, int h)
    :HubWidget(w,h),
     _sb_width(0), _sb_height(0), _cs_height(0), _pri_tbl_width(0), _pri_tbl_height(0), _auto_cover(0)
{
    int k = h/8;
    _p_caption_scroller = new ScrolledWidget( w, k );
    _p_table_scroller   = new ScrolledWidget( w, h-k );
    _p_main_table       = NULL;
    _p_caption_sizer    = NULL;
    _p_verti_scrollbar  = NULL;
    _p_horiz_scrollbar  = NULL;
    _p_table_frame      = NULL;
    _p_top_gadget       = NULL;
    _p_bottom_gadget    = NULL;
 
    _p_caption_scroller->SetNoDragSliding(true);

    Signal::FuncID fid1 = Tie2Signal( _p_table_scroller->OnHAdvance, _p_caption_scroller, &ScrolledWidget::HScrollTo );
    _p_caption_scroller->Posses( new Signal::UntieOnDestruction(fid1) );

    Signal::FuncID fid2 = Tie2Signal( _p_caption_scroller->OnHAdvance, _p_table_scroller, &ScrolledWidget::HScrollTo );
    _p_table_scroller->Posses( new Signal::UntieOnDestruction(fid2) );

    HubWidget::Insert( _p_caption_scroller, 0, 0 );
    HubWidget::Insert( _p_table_scroller, 0, k );
}



/* Automatic type deduction, and placement
----------------------------------------------------------------------------------------------------*/
void TableSystemWidget::Insert( Widget *pwidget )
{
    /* Try for IWTable interface
    ----------------------------------------------------------------------------------------------------*/
    if ( IWTable *pi_tb = dynamic_cast<IWTable*>(pwidget) )
    {
        TableWidget *ptw = &pi_tb->Table();

        if ( _p_main_table ) { APP_ERROR( Error("Insert: Table allready exists"), "TableSystemWidget"); }

        _p_table_scroller->AttachWidget( (_p_main_table=pi_tb)->AtomPtr() );

        _pri_tbl_width = pi_tb->Atom().GetWidth();
        _pri_tbl_height= pi_tb->Atom().GetHeight();
        pi_tb->Atom().OnResize.TieVoid( this, &TableSystemWidget::UpdateTableSizes );

        if ( _p_caption_sizer ) 
        { 
            _p_caption_sizer->Atom().Posses( new
                ResizeRelation( _p_caption_sizer->Atom(), _p_main_table->Atom(), true, false )
                );
        }
    }

    /* Try for IWSizer interface
    ----------------------------------------------------------------------------------------------------*/
    else if ( IWSizer *pi_siz = dynamic_cast<IWSizer *>(pwidget) )
    {
        SizerWidget *psw = &pi_siz->Sizer();
        if ( _p_caption_sizer ) { APP_ERROR( Error("Insert: Caption sizer allready exists"), "TableSystemWidget"); }

        _p_caption_scroller->AttachWidget( (_p_caption_sizer=pi_siz)->AtomPtr() );
        _cs_height = _p_caption_sizer->Atom().GetHeight();

        if ( _p_main_table ) 
        { 
            _p_caption_sizer->Atom().Posses( new
                ResizeRelation( _p_caption_sizer->Atom(), _p_main_table->Atom(), true, false )
                );
        }
    }

    /* Try for IWScrollbar interface
    ----------------------------------------------------------------------------------------------------*/
    else if ( IWScrollbar *pi_sb = dynamic_cast<IWScrollbar*>(pwidget) )
    {
        ScrollbarWidget *psbw = &pi_sb->Scroller();
        if ( psbw->IsHorizontal() )
        {
            if (_p_horiz_scrollbar ) { APP_ERROR( Error("Insert: H Scrollbar allready exists"), "TableSystemWidget"); }
            HubWidget::Insert( (_p_horiz_scrollbar = pi_sb)->AtomPtr() );
            _sb_height = _p_horiz_scrollbar->Atom().GetHeight();
            assert(_p_main_table);
            _p_table_scroller->Posses( 
                    new AdvScrollRelation( *_p_table_scroller, *_p_horiz_scrollbar, false ) 
                );
            _p_horiz_scrollbar->Atom().OnVisible.TieVoid( this, &TableSystemWidget::UpdateScrollbarSizes );
        }
        else 
        {
            if ( _p_verti_scrollbar ) { APP_ERROR( Error("Insert: V Scrollbar allready exists"), "TableSystemWidget"); }
            HubWidget::Insert( (_p_verti_scrollbar = pi_sb)->AtomPtr() ); 
            _sb_width = _p_verti_scrollbar->Atom().GetWidth();
            assert(_p_main_table);

            // if top gadget was inserted then relation type is disable-scrollbar relation
            if ( _p_top_gadget )
            {
                _vertical_scroll_rel = new AdvScrollRelation2( *_p_table_scroller, *_p_verti_scrollbar, true );
                _p_verti_scrollbar->Atom().Show();
            }
            // else it is hide scrollbar-relation
            else
            {
                _vertical_scroll_rel = new AdvScrollRelation( *_p_table_scroller, *_p_verti_scrollbar, true );
            }

            _p_verti_scrollbar->Atom().OnVisible.TieVoid( this, &TableSystemWidget::UpdateScrollbarSizes );
        }
    }

    /* Check if it's supposed to be a surrounding frame
    ----------------------------------------------------------------------------------------------------*/
    else if ( FrameWidget *pfw = dynamic_cast<FrameWidget*>(pwidget) )
    {
        HubWidget::Retrieve( _p_table_scroller );
        HubWidget::Retrieve( _p_caption_scroller );
        HubWidget::Insert( _p_table_frame = pfw );
        _p_table_frame->Insert( _p_table_scroller );
        _p_table_frame->Insert( _p_caption_scroller );
    }

    /* We can place two small gadgets
    ----------------------------------------------------------------------------------------------------*/
    else if ( !_p_bottom_gadget )
    {
        // if top gadget is beig inserted then change relation type to disable-scrollbar relation
        if (!_p_top_gadget && _p_verti_scrollbar)
        {
            assert( _vertical_scroll_rel.Unique() );
            _vertical_scroll_rel = new AdvScrollRelation2( *_p_table_scroller, *_p_verti_scrollbar, true );
            _p_verti_scrollbar->Atom().Show();
        }

        HubWidget::Insert( (_p_top_gadget ? _p_bottom_gadget : _p_top_gadget) = pwidget, 0,0,0 );
    }


    /* We don't know where to place this weird widget
    ----------------------------------------------------------------------------------------------------*/
    else
    {
        APP_ERROR( Error("Insert: Wrong widget type"), "TableSystemWidget");
    }
    Resize( GetWidth(), GetHeight() );
}



/* Resize and keep layout
----------------------------------------------------------------------------------------------------*/
void TableSystemWidget::Resize( int w, int h )
{
    HubWidget::Resize( w, h);
    Fit();
}

void _ResizePositive( Widget *who, int w, int h )
{
    who->Resize( SimpleMax( 0, w ), SimpleMax( 0, h ));
}

void TableSystemWidget::Fit()
{
    int w = GetWidth(), h=GetHeight();
    int sb_width = _sb_width;
    int sb_height = _sb_height;

    if (_p_verti_scrollbar && _p_verti_scrollbar->Atom().IsVisible() )
    {
        _ResizePositive( _p_verti_scrollbar->AtomPtr(), sb_width, h-(_cs_height+sb_height) );
        _p_verti_scrollbar->Atom().SetPosition ( Pos( w-sb_width, _cs_height) );
    }
    if (_p_horiz_scrollbar && _p_horiz_scrollbar->Atom().IsVisible() )
    {
        _ResizePositive( _p_horiz_scrollbar->AtomPtr(), w-sb_width, sb_height );
        _p_horiz_scrollbar->Atom().SetPosition ( Pos( 0, h-sb_height ) );
    }

    if (_p_table_frame)
    {
        int frame_w = w-sb_width;
        int frame_h = h-sb_height;

        _p_table_frame->Fill( Rect( 0, 0, frame_w, frame_h ));
        Rect c = _p_table_frame->GetClientRect( _p_caption_scroller );
        _p_caption_scroller->Resize( c.GetW(), _cs_height );
        _p_caption_scroller->SetPosition( c.GetPos() );
        _ResizePositive( _p_table_scroller, c.GetW(), c.GetH()-_cs_height );
        _p_table_scroller->SetPosition( c.GetPos() + Pos( 0, _cs_height) );
    }
    else
    {
        if (_p_caption_scroller)
        {
            _p_caption_scroller->Resize( w-sb_width, _cs_height );
            _p_caption_scroller->SetPosition( Pos(0,0) );
        }
        if (_p_table_scroller)
        {
            _ResizePositive( _p_table_scroller, w-sb_width, h-(_cs_height+sb_height) );
            _p_table_scroller->SetPosition  ( Pos(0,_cs_height) );
        }
    }

    if ( _p_top_gadget )
    {
        _p_top_gadget->SetPosition( Pos( w-_p_top_gadget->GetWidth(), 0) );
        HubWidget::BringOnTop( _p_top_gadget );
    }
    if ( _p_bottom_gadget )
    {
        if ( sb_width && sb_height )
        { 
            _p_bottom_gadget->SetPosition( Pos( w-sb_width, h-sb_height) );

            if ( !_p_bottom_gadget->Atom().IsVisible() ) { _p_bottom_gadget->Atom().Show(); }
        }
        else if ( _p_bottom_gadget->Atom().IsVisible() ) { _p_bottom_gadget->Atom().Hide(); }
    }

    if ( _auto_cover )
    {
        int pri_tbl_width = _pri_tbl_width;
        int pri_tbl_height= _pri_tbl_height;
        int table_w = pri_tbl_width;
        int table_h = pri_tbl_height;
        int client_w = _p_table_scroller->GetWidth();
        int client_h = _p_table_scroller->GetHeight();

        // try to cover all client area by table (if table is smaller than area)
        if ( client_w > table_w )
        {
            _p_main_table->Atom().Resize( client_w, table_h );
        }
        else if ( client_w < pri_tbl_width )
        {
            _p_main_table->Atom().Resize( pri_tbl_width, table_h );
        }
    
        table_w = _p_main_table->Atom().GetWidth();

        if ( client_h > table_h )
        {
            _p_main_table->Atom().Resize( table_w, client_h );
        }
        else if ( client_h < pri_tbl_height )
        {
            _p_main_table->Atom().Resize( table_w, pri_tbl_height );
        }

        table_h = _p_main_table->Atom().GetHeight();

        // right scrollbar dissappears only if top-gadget does not exist
        if ( !_p_top_gadget )
        {
            // Check if full table would be visible if we hide both scrollbars
            bool would_fit_width = client_w + sb_width >= table_w;
            bool would_fit_height= client_h + sb_height >= table_h;
    
            if ( would_fit_width && would_fit_height )
            {
                _p_main_table->Atom().Resize( client_w + sb_width, client_h + sb_height);
                _sb_width = _sb_height = 0;
            }
        }
        _pri_tbl_width = pri_tbl_width;
        _pri_tbl_height= pri_tbl_height;
    }

    // Make sure that scrollbar sizes have not been changed, and if so try again
    if ( _sb_width!=sb_width || _sb_height!=sb_height ) { Fit(); }
}

void TableSystemWidget::UpdateScrollbarSizes()
{
    if ( _p_verti_scrollbar )
    {
        _sb_width = (_p_verti_scrollbar->Atom().IsVisible())? _p_verti_scrollbar->Atom().GetWidth() : 0;
    }

    if ( _p_horiz_scrollbar )
    {
        _sb_height = (_p_horiz_scrollbar->Atom().IsVisible())? _p_horiz_scrollbar->Atom().GetHeight() : 0;
    }
}

void TableSystemWidget::UpdateTableSizes()
{
    _pri_tbl_width = _p_main_table->Atom().GetWidth();
    _pri_tbl_height= _p_main_table->Atom().GetHeight();
}

EnumWidget&  TableSystemWidget::Enumerator()
{ 
    return (dynamic_cast<IWEnum&>(Table())).Enumerator(); 
}

};//namespace Useless
