#ifndef __INCLUDED_USELESS_FACED_WIDGET_H__
#define __INCLUDED_USELESS_FACED_WIDGET_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/Util/CPT.h"

namespace Useless {

/*! Kind of LogicWidget that paints skin
 */
class USELESS_API FacedWidget : virtual public LogicWidget
{
public:
    FacedWidget( const SkinMan &skinman_copy );

#   include "FacedWidget.inl" // ParmList Ctor
    
     virtual void  Repaint( const WidgetPainter &painter );
     virtual void  Resize ( int w, int h );

     virtual  int  GetState () const;
     virtual bool  SetState (int state);
     virtual bool  HasState (int state) const;
   
    DEFINE_CYCLIC_VISITABLE(Widget);

private:
    SkinSelector  _skin_manager;
};
    
};//namespace Useless

#endif//__INCLUDED_USELESS_FACED_WIDGET_H__
