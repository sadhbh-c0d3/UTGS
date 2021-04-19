#ifndef __INCLUDED_USELESS_INTERCEPTOR_WIDGET_H__
#define __INCLUDED_USELESS_INTERCEPTOR_WIDGET_H__

/*
 *    $Id: InterceptorWidget.h,v 1.3 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        InterceptorWidget
 *
 *    PURPOSE
 *        Intercepts any input
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Widgets/CompoundWidget.h"

namespace Useless {

/*! \ingroup Widgets
 *  This widget adds additional functionality if derrived from it.
 *  Any input requests will be resent only inside this widget.
 */
class USELESS_API InterceptorWidget : virtual public CompoundWidget
{
public:
    InterceptorWidget() {}
    virtual ~InterceptorWidget() {}

    virtual void InputIntercept (Widget *sender=NULL);
    virtual void InputRelease   (Widget *sender=NULL);

protected:
    virtual void Attach();
    virtual void Detach();
};


};//namespace Useless
#endif//__INCLUDED_USELESS_INTERCEPTOR_WIDGET_H__
