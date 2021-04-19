#ifndef __INCLUDED_USELESS_CAPTION_MARK_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_MARK_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/MarkLogic.h"
#include "Useless/GUI/Logic/KeyButtonLogic.h"

/*! \ingroup Widgets
 *  Check-mark with text caption.
 */
namespace Useless {

typedef TemplateWidget< CaptionWidget, MarkLogic > NoKeyCaptionMark;
typedef TemplateWidget< NoKeyCaptionMark, KeyButtonLogic > CaptionMark;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_MARK_WIDGET_H__
