#ifndef __INCLUDED_USELESS_SCROLLED_WIDGET_H__
#define __INCLUDED_USELESS_SCROLLED_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/Util/LiveVariable.h"
#include "Useless/GUI/WidgetInterfaces.h"

namespace Useless {

/*! \ingroup Widgets
 *  Virtual widget which may scroll its content in any direction.
 */
class USELESS_API ScrolledWidget: public BridgeWidget, virtual public ActiveWidget, public IWScrollee
{
public:
    ScrolledWidget(int w=0, int h=0);
    virtual ~ScrolledWidget() {}

    void Scroll(int dx, int dy);
    void ScrollTo(int x, int y);
    void ScrollPrecentages(double ax, double ay);
	void ScrollVisible(double ax, double ay);
    void HScrollTo(int x) { Scroll(x-_x, 0); }
    void VScrollTo(int y) { Scroll(0, y-_y); }

    enum { DEFFAULT_SPEED = 10 };
    void SetSpeed(unsigned int pix_per_second) { _speed = pix_per_second; }

    void SetNoDragSliding( bool t=true ) { _no_drag_sliding=t; }

    virtual void AttachWidget( Widget *widget, int x=0, int y=0);
    virtual Widget* DetachWidget();
    virtual void Resize(int w, int h);

    virtual bool Accept ( const Request& request);
            bool AcceptInputCursor( const InputCursor &mc );

    /////////////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME                      | ARGS
    //-----------------------------------------------------------------------
    Signal_1<int>&                   OnHAdvance;                // x advance
    Signal_1<int>&                   OnVAdvance;                // y advance
    Signal_1<int>&                   OnHSize;                   // pager x size
    Signal_1<int>&                   OnVSize;                   // pager y size
    Signal_1<int>&                   OnHRange;                  // full x size
    Signal_1<int>&                   OnVRange;                  // full y size
    //
    /////////////////////////////////////////////////////////////////////////

    int GetHAdvance() { return _x; }
    int GetVAdvance() { return _y; }
    int GetHSize()    { return _x_size; }
    int GetVSize()    { return _y_size; }
    int GetHRange()   { return _x_full; }
    int GetVRange()   { return _y_full; }

    virtual ScrolledWidget& Scrollee() { return *this; }

public://rather than private
    LiveVariable<int>   _x;
    LiveVariable<int>   _x_size;
    LiveVariable<int>   _x_full;

    LiveVariable<int>   _y;
    LiveVariable<int>   _y_size;
    LiveVariable<int>   _y_full;

    int                 _speed;
    int                 _x_vel;
    int                 _y_vel;

    bool                _no_drag_sliding;

    void IdleScroll();
    void UpdateSize(int w, int h);
    void Update();
    void KeepActorVisible( const Rect &actor_bound);
    Signal::FuncID _on_resize;
    Signal::FuncID _on_actor_move;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_SCROLLED_WIDGET_H__
