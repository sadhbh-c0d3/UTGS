#ifndef __INCLUDED__USELESS_ADV_LIST_WIDGET_H__
#define __INCLUDED__USELESS_ADV_LIST_WIDGET_H__

#include "Useless/GUI/Widgets/ListWidget.h"
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Widgets/AdvScrollbarWidget.h"
#include "Useless/GUI/Widgets/FrameWidget.h"
#include "Useless/GUI/Widgets/ScrolledWidget.h"


namespace Useless {

//USES
    class AdvListStyle;
    class AdvListStyle2;

/*! \ingroup Widgets
 *  A AdvScrollbar Widget. \sa ScrollbarWidget
 */
class USELESS_API AdvListWidget :private HubWidget, virtual public Widget,
                     public IWList, public IWScrollee, public IWActive
{
public:
    AdvListWidget( const ListStyle      &list_style,
                   const FrameStyle     &frm_style,
                   const ScrollbarStyle &vsb_style );

    AdvListWidget( const     ListStyle2 &list_style,
                   const     FrameStyle &frm_style,
                   const ScrollbarStyle &vsb_style );

    AdvListWidget( const         ListStyle &list_style,
                   const        FrameStyle &frm_style,
                   const AdvScrollbarStyle &vsb_style );

    AdvListWidget( const        ListStyle2 &list_style,
                   const        FrameStyle &frm_style,
                   const AdvScrollbarStyle &vsb_style );

    virtual ~AdvListWidget();
    
    virtual void Repaint( const WidgetPainter &painter);
    virtual void Resize( int w, int h);

    virtual ListWidget&      Items()      { return *_p_list; }
    virtual ScrolledWidget&  Scrollee()   { return *_p_scroller; }
    virtual EnumWidget&      Enumerator() { return Items(); }
    virtual ActiveWidget&    Control()    { return Scrollee(); }

    void ShowScrollbar();
    void HideScrollbar();

    HubWidget::GetHeight;
    HubWidget::GetWidth;

private:
    ListWidget      *_p_list;
    FrameWidget     *_p_frame;
    ScrolledWidget  *_p_scroller;

    IWScrollbar *_pi_scrollbar;
    int          _sb_width;

    void Init();
    void Fit();
};

/*! AdvListStyle definition
-----------------------------------------------------------------*/
class AdvListStyle : public ListStyle, public FrameStyle, public ScrollbarStyle
{
public:
    AdvListStyle( const ListStyle &list_style,
                  const FrameStyle &frm_style,
                  const ScrollbarStyle &vsb_style 

                ): ListStyle(list_style), FrameStyle(frm_style), ScrollbarStyle(vsb_style)
                {}
};

class AdvListStyle2 : public ListStyle, public FrameStyle, public AdvScrollbarStyle
{
public:
    AdvListStyle2( const ListStyle &list_style,
                  const FrameStyle &frm_style,
                  const AdvScrollbarStyle &vsb_style 

                ): ListStyle(list_style), FrameStyle(frm_style), AdvScrollbarStyle(vsb_style)
                {}
};

};//namespace Useless

#endif//__INCLUDED__USELESS_ADV_LIST_WIDGET_H__
