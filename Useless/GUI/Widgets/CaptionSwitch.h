#ifndef __INCLUDED_USELESS_CAPTION_SWITCH_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_SWITCH_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/SwitchLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Switch-trigger with text caption.
 */
namespace Useless {

typedef TemplateWidget< CaptionWidget, SwitchLogic > NoKeyCaptionSwitch;
typedef TemplateWidget< NoKeyCaptionSwitch, KeyButtonLogic > CaptionSwitch;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_SWITCH_WIDGET_H__
