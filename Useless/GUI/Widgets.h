#ifndef __INCLUDED__USELESS_WIDGETS_H__
#define __INCLUDED__USELESS_WIDGETS_H__

/*
 *    $Id: Widgets.h,v 1.7 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        Widgets
 *
 *    PURPOSE
 *        Just include this file instead of writing class x-Widget,
 *        when trying to avoid long compilation.
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *    TODO
 *        Please add some other often used classes 
 *        at end of this list
 * 
*/

namespace Useless {

/* All widget classes listed by name (do not change order) 
  ---------------------------------------------------------*/
    class ActiveWidget;
    class AnimatedWidget;
    class BarWidget;
    class BorderWidget;
    class BridgeWidget;
    class BufferWidget;
    class CaptionWidget;
    class CompoundWidget;
    class CreditsWidget;
    class DataTableWidget;
    class EnumWidget;
    class FacedWidget;
    class FrameWidget;
    class GraphicWidget;
    class HubWidget;
    class ImageWidget;
    class ListWidget;
    class MenuWidget;
    class PopupWidget;
    class ScrollbarWidget;
    class ScrolledWidget;
    class SizerWidget;
    class TableListWidget;
    class TableSystemWidget;
    class TableWidget;
    class TextAreaWidget;
    class TextField;
    class Widget;
    class WidgetTableWidget;

/* OBSOLETE:
------------
    class GfxGroupWidget;
    class TextBarWidget;
    class TextButtonWidget;
    class TextEditWidget;
*/

};//namespace Useless


/* SOME #include's here (don't worry they won't change too often) */


/* NOT-CLASSES, ONLY TYPEDEFS 
-------------------------------------*/
#include "Useless/GUI/Widgets/ButtonWidget.h"
#include "Useless/GUI/Widgets/HiliteWidget.h"
#include "Useless/GUI/Widgets/MarkWidget.h"
#include "Useless/GUI/Widgets/RadioWidget.h"
#include "Useless/GUI/Widgets/SwitchWidget.h"
#include "Useless/GUI/Widgets/TabWidget.h"
#include "Useless/GUI/Widgets/TriggerWidget.h"

#include "Useless/GUI/Widgets/CaptionButton.h"
#include "Useless/GUI/Widgets/CaptionHilite.h"
#include "Useless/GUI/Widgets/CaptionMark.h"
#include "Useless/GUI/Widgets/CaptionRadio.h"
#include "Useless/GUI/Widgets/CaptionSwitch.h"
#include "Useless/GUI/Widgets/CaptionTab.h"
#include "Useless/GUI/Widgets/CaptionTrigger.h"

/* NullWidget may be handy */
#include "Useless/GUI/Widgets/NullWidget.h"

#endif//__INCLUDED__USELESS_WIDGETS_H__
