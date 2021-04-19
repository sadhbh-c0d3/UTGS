#ifndef __INCLUDED_USELESS_CAPTION_BUTTON_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_BUTTON_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/ButtonLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Button with text caption.
 */
namespace Useless {

typedef TemplateWidget< CaptionWidget, ButtonLogic > NoKeyCaptionButton;
typedef TemplateWidget< NoKeyCaptionButton, KeyButtonLogic > CaptionButton;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_BUTTON_WIDGET_H__
