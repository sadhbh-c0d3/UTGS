#ifndef __INCLUDED_USELESS_CAPTION_TAB_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_TAB_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/TabLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Tab-button with text caption.
 */
namespace Useless {

typedef TemplateWidget< CaptionWidget, TabLogic > NoKeyCaptionTab;
typedef TemplateWidget< NoKeyCaptionTab, KeyButtonLogic > CaptionTab;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_TAB_WIDGET_H__
