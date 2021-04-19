#ifndef __INCLUDED_USELESS_RADIO_WIDGET_H__
#define __INCLUDED_USELESS_RADIO_WIDGET_H__

/*
 *    $Id: RadioWidget.h,v 1.8 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        RadioWidget
 *
 *    PURPOSE
 *        Generic graphic radio/tab button (NOT resizeable)
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/RadioLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"


/*! \ingroup Widgets
 *  Just a simple Radio-button
 */
namespace Useless {

typedef TemplateWidget< FacedWidget, RadioLogic > NoKeyRadio;
typedef TemplateWidget< NoKeyRadio, KeyButtonLogic > RadioWidget;

};//namespace Useless


#endif//__INCLUDED_USELESS_RADIO_WIDGET_H__
