#ifndef __INCLUDED_USELESS_ICON_BUTTON_WIDGET_H__
#define __INCLUDED_USELESS_ICON_BUTTON_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/IconWidget.h"
#include "Useless/GUI/Logic/ButtonLogic.h"

/*! \ingroup Widgets
 *  Button with Icon.
 */
namespace Useless {

typedef Useless::TemplateWidget< Useless::IconWidget, Useless::ButtonLogic > IconButton;

};//namespace Useless

#endif//__INCLUDED_USELESS_ICON_BUTTON_WIDGET_H__
