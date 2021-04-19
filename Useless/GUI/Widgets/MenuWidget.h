#ifndef __INCLUDED_USELESS_MENU_WIDGET_H__
#define __INCLUDED_USELESS_MENU_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/ListWidget.h"
#include "Useless/GUI/Widgets/FrameWidget.h"

namespace Useless {

//USES
    class MenuStyle;

/*! \ingroup Widgets
 *  Menu like Widget. It's frame resizes to size of its content.
 */
class USELESS_API MenuWidget : virtual private FrameWidget, virtual public Widget, public IWList, public IWActive
{
public:
    template< class ListStyle >
    MenuWidget( const FrameStyle &frame_style, const ListStyle &list_items_style )
        :FrameWidget(frame_style)
    {
        _option_list = FrameWidget::WInsert( new ListWidget( list_items_style ) );
        Init();
    }

    virtual void Resize(int w, int h);

            void SetMenuWidth(int w);

    virtual ListWidget&   Items() { return *_option_list; }
    virtual EnumWidget&   Enumerator() { return Items(); }
    virtual ActiveWidget& Control() { return Items(); }

private:
    ListWidget *_option_list;
    bool        _close_widget;
    int         _fixed_width;

    void UpdateSize(int w, int h);
    void Init();
};

};//namespace Useless
#endif// __INCLUDED_USELESS_MENU_WIDGET_H__
