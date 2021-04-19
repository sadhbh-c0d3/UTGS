#ifndef __INCLUDED_USELESS_SCROLL_RELATION_H__
#define __INCLUDED_USELESS_SCROLL_RELATION_H__

#include "Useless/GUI/Relations/Relation.h"
#include "Useless/GUI/WidgetInterfaces.h"
#include "Useless/Functional/Signal.h"

namespace Useless {

/*! \ingroup Relations
 *  ScrollRelation creates relation between ScrolledWidget and ScrollbarWidget.
 *  Direction of scroll (horizontal|vertical) is obtained from scrollbar orientation.
 */
class USELESS_API ScrollRelation : public Relation
{
public:
    ScrollRelation( IWScrollee &scrollee, IWScrollbar &scroller, bool use_mouse_wheel=false);
    virtual ~ScrollRelation();

private:
    Signal::FuncID _on_scrollbar_advance;
    Signal::FuncID _on_size_change;
    Signal::FuncID _on_range_change;
    Signal::FuncID _on_advance;
    Signal::FuncID _on_scrollbar_wheel;
    Signal::FuncID _on_wheel;
};

};//namespace Useless

#endif  __INCLUDED_USELESS_RELATIONS_H__
