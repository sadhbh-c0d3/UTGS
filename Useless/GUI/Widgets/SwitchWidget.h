#ifndef __INCLUDED_USELESS_SWITCH_WIDGET_H__
#define __INCLUDED_USELESS_SWITCH_WIDGET_H__

/*
 *    $Id: SwitchWidget.h,v 1.9 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        SwitchWidget
 *
 *    PURPOSE
 *        Generic graphic push-button (NOT resizeable)
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/SwitchLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"


/*! \ingroup Widgets
 *  A Fixed size Switch Widget.
 *  Immidiatelly after press, it changes it's state and ramains in it.
 *
 *  Widget is a kind of switch, and may be used to turn something on or off.
 *  
 */
namespace Useless {

typedef TemplateWidget< FacedWidget, SwitchLogic > NoKeySwitch;
typedef TemplateWidget< NoKeySwitch, KeyButtonLogic > SwitchWidget;

};//namespace Useless

#endif//__INCLUDED_USELESS_SWITCH_WIDGET_H__
