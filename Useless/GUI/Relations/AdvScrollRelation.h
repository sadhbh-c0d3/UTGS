#ifndef __INCLUDED_USELESS_ADVSCROLL_RELATION_H__
#define __INCLUDED_USELESS_ADVSCROLL_RELATION_H__

#include "Useless/GUI/Relations/Relation.h"
#include "Useless/GUI/Relations/AppearRelation.h"
#include "Useless/GUI/Relations/ScrollRelation.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Misc/WidgetCycler.h"

namespace Useless {

/*! \ingroup Relations
 *  AdvScrollRelation creates relation between ScrolledWidget and ScrollbarWidget.
 *  Direction of scroll (horizontal|vertical) is obtained from scrollbar orientation.
 *  In addition to ScrollRelation manages scrollbar show/hide effect
 */
template< class WidgetAction > // action functor
class BaseScrollRelation : public ScrollRelation
{
public:
    BaseScrollRelation( ScrolledWidget &scrollee, IWScrollbar &isbw,
                       bool use_mouse_wheel=false );
    virtual ~BaseScrollRelation();

    void UpdateSize(int s);
    void UpdateRange(int r);

    //Signal &OnScrollbarShow;
    //Signal &OnScrollbarHide;
    WidgetAction& GetAction() { return _widget_action; }

private:
    WidgetAction   _widget_action;
    Signal::FuncID _on_update_s;
    Signal::FuncID _on_update_r;
    int  _size;
    int  _range;
};

//This action hides widget ( and so its child )
struct AppearWidgetAction : public AppearRelation
{
    AppearWidgetAction( Widget &p ): AppearRelation(p) {}
    virtual ~AppearWidgetAction() {}

    void operator ()( bool t ) { Appear(t); }
};

//This action De/Activates all child ActiveWidgets
struct ActivateWidgetAction
{
    ActivateWidgetAction( Widget &p ): _p(&p) {}
    virtual ~ActivateWidgetAction() {}

    void operator ()( bool t ) 
    {
        ActiveWidget *p;
        if ( p = dynamic_cast<ActiveWidget *>(_p) )
        {
            p->SetActive( t ); 
        }
        WidgetCycler c(_p);
        do {
        if ( p = dynamic_cast<ActiveWidget *>( c.Who() ) )
        {
            p->SetActive( t );
        }
        } while ( c.CycleForward() );
    }
    Widget *_p;
};

typedef BaseScrollRelation< AppearWidgetAction > AdvScrollRelation;
typedef BaseScrollRelation< ActivateWidgetAction > AdvScrollRelation2;

//---------------------------------------------------------------------------------
template< class WidgetAction >
BaseScrollRelation<WidgetAction>::BaseScrollRelation( ScrolledWidget &scw, IWScrollbar &isbw, bool mwh )
    :ScrollRelation( scw, isbw, mwh), _widget_action( isbw.Atom() )
    //OnScrollbarShow(_appear_relation.OnShow), OnScrollbarHide(_appear_relation.OnHide)
{
    ScrollbarWidget &sbw = isbw.Scroller();

    if ( sbw.IsHorizontal() )
    {
        _on_update_s = Tie2Signal( scw.OnHSize,  this, &BaseScrollRelation::UpdateSize );
        _on_update_r = Tie2Signal( scw.OnHRange, this, &BaseScrollRelation::UpdateRange );
        _size = scw.GetHSize();
        _range = scw.GetHRange();
    }
    else
    {
        _on_update_s = Tie2Signal( scw.OnVSize,  this, &BaseScrollRelation::UpdateSize );
        _on_update_r = Tie2Signal( scw.OnVRange, this, &BaseScrollRelation::UpdateRange );
        _size = scw.GetVSize();
        _range = scw.GetVRange();
    }
}

template< class WidgetAction >
BaseScrollRelation<WidgetAction>::~BaseScrollRelation()
{
    _on_update_s.Untie();    
    _on_update_r.Untie();    
}

template< class WidgetAction >
void BaseScrollRelation<WidgetAction>::UpdateSize(int s)
{
    _size = s;
    _widget_action( _size<_range );
}

template< class WidgetAction >
void BaseScrollRelation<WidgetAction>::UpdateRange(int r)
{
    _range = r;
    _widget_action( _size<_range );
}

};//namespace Useless


#endif  __INCLUDED_USELESS_ADVSCROLL_RELATION_H__
