#ifndef __INCLUDED_USELESS_TRIGGER_WIDGET_H__
#define __INCLUDED_USELESS_TRIGGER_WIDGET_H__

/*
 *    $Id: TriggerWidget.h,v 1.5 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        TriggerWidget
 *
 *    PURPOSE
 *        Button that won't un-press 
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/TriggerLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Just a simple Trigger widget.
 */
namespace Useless {

typedef TemplateWidget< FacedWidget, TriggerLogic > NoKeyTrigger;
typedef TemplateWidget< NoKeyTrigger, KeyButtonLogic > TriggerWidget;

};//namespace Useless

#endif//__INCLUDED_USELESS_TRIGGER_WIDGET_H__
