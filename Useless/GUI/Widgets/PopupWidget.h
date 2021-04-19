#ifndef __INCLUDED_USELESS_POPUP_WIDGET_H__
#define __INCLUDED_USELESS_POPUP_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/Popup.h"
#include "Useless/GUI/Widgets/FrameWidget.h"
#include "Useless/Util/Storage/StupidVector.h"

namespace Useless {

//USES
class PopupStyle;
class VLayout;

/*! PopupWidget is a Popup with frame and button tray
 */
class USELESS_API PopupWidget : public Popup
{
public:
    PopupWidget( const PopupStyle &style, const Text &title = "..." );
    virtual ~PopupWidget();

    void  AddButton( ActiveWidget *p_button, int weight=100, bool closes_popup=false );
    void  AddBlock ( Widget *p_widget, int weight );
    void  AddGap   ( int weight=-10 );

           FrameWidget &GetFrame()       { return *_p_frame; }
    const  FrameWidget &GetFrame() const { return *_p_frame; }

    DEFINE_CYCLIC_VISITABLE(Popup);

protected:
    FrameWidget    *_p_frame;
    FrameWidget    *_p_button_tray;
    VLayout        *_p_body_box;

    // obsolete part - will change in future
    typedef struct __ButtonCell { Widget* p_button; int weight; } ButtonCell;
    StupidVector< ButtonCell > _button_cells;
    int _weight_sum;
    int _fixed_sum;
    int _button_tray_size;
    int _tray_alignment;
    void  UpdateButtonTray();

};
/*-----------------------------------------------------------------------------------------*/

class PopupStyle
{
public:
    PopupStyle( int w, int h,
                const Skin         &frame_skin,
                const Skin         &tray_skin,
                const CaptionStyle &titlebar_style,
                      bool          modal=false,
                      int           title_height=0
              ):
                w(w),
                h(h),
                frame_skin(frame_skin),
                button_tray_skin(tray_skin),
                titlebar_style(titlebar_style),
                modal(modal),
                title_height(title_height) {}

    int            w;
    int            h;
    const Skin    &frame_skin;
    const Skin    &button_tray_skin;
    CaptionStyle   titlebar_style;
    bool           modal;
    int            title_height;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_POPUP_WIDGET_H__
