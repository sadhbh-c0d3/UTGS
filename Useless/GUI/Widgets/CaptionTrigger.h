#ifndef __INCLUDED_USELESS_CAPTION_TRIGGER_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_TRIGGER_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/TriggerLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Trigger with text caption.
 */
namespace Useless {

typedef TemplateWidget< CaptionWidget, TriggerLogic > NoKeyCaptionTrigger;
typedef TemplateWidget< NoKeyCaptionTrigger, KeyButtonLogic > CaptionTrigger;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_TRIGGER_WIDGET_H__
