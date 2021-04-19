#ifndef __INCLUDED_USELESS_MARK_WIDGET_H__
#define __INCLUDED_USELESS_MARK_WIDGET_H__

/*
 *    $Id: MarkWidget.h,v 1.9 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        MarkWidget
 *
 *    PURPOSE
 *        Generic graphic check-marks (NOT resizeable)
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/FacedWidget.h"
#include "Useless/GUI/Logic/MarkLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Just a simple check-mark widget
 */
namespace Useless {

typedef TemplateWidget< FacedWidget, MarkLogic > NoKeyMark;
typedef TemplateWidget< NoKeyMark, KeyButtonLogic > MarkWidget;

};//namespace Useless

#endif//__INCLUDED_USELESS_MARK_WIDGET_H__
