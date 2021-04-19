#ifndef __INCLUDED_USELESS_CAPTION_HILITE_WIDGET_H__
#define __INCLUDED_USELESS_CAPTION_HILITE_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Logic/HiliteLogic.h"

/*! \ingroup Widgets
 *  Horizontal bar with text caption.
 */
namespace Useless {

    typedef Useless::TemplateWidget< Useless::CaptionWidget, Useless::HiliteLogic > CaptionHilite;

};//namespace Useless

#endif//__INCLUDED_USELESS_CAPTION_HILITE_WIDGET_H__
