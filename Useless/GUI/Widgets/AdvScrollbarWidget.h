#ifndef __INCLUDED_USELESS_ADV_SCROLLBAR_WIDGET_H__
#define __INCLUDED_USELESS_ADV_SCROLLBAR_WIDGET_H__

#include "Useless/GUI/Widgets/ScrollbarWidget.h"
#include "Useless/GUI/Widgets/ButtonWidget.h"


namespace Useless {

//USES
    class AdvScrollbarStyle;

/*! \ingroup Widgets
 *  A AdvScrollbar Widget. \sa ScrollbarWidget
 */
class USELESS_API AdvScrollbarWidget: private HubWidget, virtual public Widget, public IWScrollbar
{
public:
    AdvScrollbarWidget( const ScrollbarStyle &sb_style,
                        const SkinMan &skins_button_up,
                        const SkinMan &skins_button_down,
                                  int  button_scroll_speed=1
                       );
    AdvScrollbarWidget( const AdvScrollbarStyle &style );

    virtual ~AdvScrollbarWidget();

    virtual void Resize(int w, int h);

    virtual ScrollbarWidget& Scroller() { return *_p_scrollbar; }

            void SetRepeatRate( long int period, long int delay );

private:
    ScrollbarWidget *_p_scrollbar;
    ButtonWidget    *_p_button_up;
    ButtonWidget    *_p_button_down;
             int     _button_scroll_speed;

    void Init();
};



/*! AdvScrollbarStyle definition
-------------------------------------------------------------------------------------*/
class AdvScrollbarStyle : public ScrollbarStyle
{
public:
    AdvScrollbarStyle(           int  size, 
                       const SkinSet &skins,
                       const SkinMan &skins_button_up,
                       const SkinMan &skins_button_down,
                                 int  size_limit=16,
                                 int  button_scroll_speed=5

                      ): ScrollbarStyle( size, skins, size_limit),
                         skins_button_up(skins_button_up),
                         skins_button_down(skins_button_down),
                         button_scroll_speed(button_scroll_speed)
                         {}

    AdvScrollbarStyle( const ScrollbarStyle &sb_style,
                       const SkinMan &skins_button_up,
                       const SkinMan &skins_button_down,
                                 int button_scroll_speed=5

                     ) : ScrollbarStyle(sb_style),
                         skins_button_up(skins_button_up),
                         skins_button_down(skins_button_down),
                         button_scroll_speed(button_scroll_speed)
                         {}

    const SkinMan &skins_button_up;
    const SkinMan &skins_button_down;
             int  button_scroll_speed;
};


};//namespace Useless

#endif//__INCLUDED_USELESS_ADV_SCROLLBAR_WIDGET_H__
