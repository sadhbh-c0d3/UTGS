#ifndef __INCLUDED_USELESS_TAB_WIDGET_H__
#define __INCLUDED_USELESS_TAB_WIDGET_H__

/*
 *    $Id: TabWidget.h,v 1.5 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        TabWidget
 *
 *    PURPOSE
 *        Generic graphic tab-button (NOT resizeable)
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/TabLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"


/*! \ingroup Widgets
 *  Just a simple Tab widget
 */
namespace Useless {

typedef TemplateWidget< FacedWidget, TabLogic > NoKeyTab;
typedef TemplateWidget< NoKeyTab, KeyButtonLogic > TabWidget;

};//namespace Useless

#endif//__INCLUDED_USELESS_TAB_WIDGET_H__
