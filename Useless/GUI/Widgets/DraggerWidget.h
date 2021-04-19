#ifndef __INCLUDED_USELESS_DRAGGER_WIDGET_H__
#define __INCLUDED_USELESS_DRAGGER_WIDGET_H__

/*
 *    $Id: DraggerWidget.h,v 1.3 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        DraggerWidget
 *
 *    PURPOSE
 *        Generic graphic button (NOT resizeable)
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/DraggerLogic.h"

/*! \ingroup Widgets
 *  Just a simple Dragger widget.
 */
namespace Useless {

    typedef Useless::TemplateWidget< Useless::FacedWidget, Useless::DraggerLogic > DraggerWidget;

};//namespace Useless

#endif//__INCLUDED_USELESS_DRAGGER_WIDGET_H__
