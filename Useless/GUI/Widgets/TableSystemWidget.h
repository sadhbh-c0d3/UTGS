#ifndef __INCLUDED_USELESS_TALBLE_SYSTEM_WIDGET_H__
#define __INCLUDED_USELESS_TALBLE_SYSTEM_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/WidgetInterfaces.h"
#include "Useless/GUI/Relations/Relation.h"

namespace Useless {

// USES
    class TableWidget;
    class SizerWidget;
    class ScrolledWidget;
    class ScrollbarWidget;
    class FrameWidget;

class USELESS_API TableSystemWidget : private HubWidget, virtual public Widget, 
                          public IWViewport, public IWSizer, public IWTableList
{
public:
    TableSystemWidget( int w, int h);
    virtual ~TableSystemWidget();

    virtual void  Insert( Widget *pwidget );

    virtual void  Resize( int w, int h );

    HubWidget::Accept;
    HubWidget::Repaint;

    /* Interface wrappers
    -----------------------------------------------------------------------*/
    virtual    SizerWidget&  Sizer()     { return _p_caption_sizer->Sizer();}
    virtual    TableWidget&  Table()     { return _p_main_table->Table();}
    virtual    IWScrollbar&  HScroll()   { return *_p_horiz_scrollbar; }
    virtual    IWScrollbar&  VScroll()   { return *_p_verti_scrollbar; }
    virtual ScrolledWidget&  Scrollee()  { return *_p_table_scroller;  }
    virtual     EnumWidget&  Enumerator();
    virtual         Widget&  Atom()      { return *this; }

    void AutoCover( bool t ) { _auto_cover=t; Fit(); }

private:
    IWTable             *_p_main_table;
    IWSizer             *_p_caption_sizer;
    IWScrollbar         *_p_verti_scrollbar;
    IWScrollbar         *_p_horiz_scrollbar;

    ScrolledWidget      *_p_caption_scroller;
    ScrolledWidget      *_p_table_scroller;
    FrameWidget         *_p_table_frame;
    Widget              *_p_top_gadget;
    Widget              *_p_bottom_gadget;

    int                  _sb_width;
    int                  _sb_height;
    int                  _cs_height;
    int                  _pri_tbl_width;
    int                  _pri_tbl_height;
    bool                 _auto_cover;

    SPointer<Relation>   _vertical_scroll_rel;

    void UpdateTableSizes();
    void UpdateScrollbarSizes();
    void Fit();
};

};//namespace Useless

#endif//__INCLUDED_USELESS_TALBLE_SYSTEM_WIDGET_H__
