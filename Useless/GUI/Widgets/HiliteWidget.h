#ifndef __INCLUDED_USELESS_HILITIE_WIDGET_H__
#define __INCLUDED_USELESS_HILITIE_WIDGET_H__

/*
 *    $Id: HiliteWidget.h,v 1.4 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        HiliteWidget
 *
 *    PURPOSE
 *        Widget that hilites when mouse moves over it 
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
 */

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/HiliteLogic.h"

/*! \ingroup Widgets
 *  Widget that hilites when mouse moves over it. 
 */
namespace Useless {

    typedef Useless::TemplateWidget< Useless::FacedWidget, Useless::HiliteLogic > HiliteWidget;

};//namespace Useless

#endif//__INCLUDED_USELESS_HILITING_WIDGET_H__
