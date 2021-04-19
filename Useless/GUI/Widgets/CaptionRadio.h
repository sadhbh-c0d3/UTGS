#ifndef __INCLUDED_USELESS_CAPTION_RADIO_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_RADIO_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/RadioLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Radio-button with text caption.
 */
namespace Useless {

typedef TemplateWidget< CaptionWidget, RadioLogic > NoKeyCaptionRadio;
typedef TemplateWidget< NoKeyCaptionRadio, KeyButtonLogic > CaptionRadio;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_RADIO_WIDGET_H__
