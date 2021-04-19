#ifndef __INCLUDED_USELESS_POPUP_H__
#define __INCLUDED_USELESS_POPUP_H__

#include "Useless/GUI/Widgets/FormWidget.h"
#include "Useless/GUI/Widgets/CaptionTrigger.h"

namespace Useless {

//USES
    class FillRelation;

/*! Popup adds titlebar to arbitrary widget.
 *  Only one popup may be on the top, and active.
 *  Modal popup intercepts input, so only widgets inside it may recieve it.
 */
class USELESS_API Popup : private HubWidget, public FormWidget, virtual public Widget
{
public:
    Popup( Widget             *content,
           const CaptionStyle &title_style,
           const Text &title = "...",
            int title_height = 0,
            bool modal = false );

    virtual ~Popup();
    virtual void Setup();

    virtual Rect GetClientRect( const Widget *sender = NULL ) const;

    typedef NoKeyCaptionTrigger Titlebar;

    void SetTitle( const Text &title );
    Text GetTitle() const;

    Titlebar& GetTitlebar() { return *_p_titlebar; }
    
private:
    Widget   *_p_content;
    Titlebar *_p_titlebar;
    bool      _modal;
    FillRelation *_p_content_fill;

    void ActivatePopupImp();
    void DeactivatePopupImp();
    void NotifyCycler();
    friend class PopupCycler;

    void InitPopup();

protected:
    Popup( int w, int h,
           const CaptionStyle &title_style,
           const Text &title = "...",
            int title_height = 0,
            bool modal = false );

    void SetContent( Widget *p_content );
    Widget* RetrieveContent();


public:
    HubWidget::Resize;
    FormWidget::Accept;
    DEFINE_CYCLIC_VISITABLE(Widget);
};

};//namespace Useless
#endif//__INCLUDED_USELESS_POPUP_H__
