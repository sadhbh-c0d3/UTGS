#ifndef __INCLUDED_USELESS_COMBO_RELATION_H__
#define __INCLUDED_USELESS_COMBO_RELATION_H__

#include "Useless/GUI/Relations/AppearRelation.h"
#include "Useless/GUI/Widgets/CaptionButton.h"
#include "Useless/GUI/WidgetInterfaces.h"

namespace Useless {

//USES  
    class Widget;
    class ListWidget;
    class ActiveWidget;

    class TextButtonWidget;

/*! \ingroup Relations
 *  ComboRelation is used to manage insertions and retrival (show/hide effect)
 *  of any widget compound menulist. This menulist widget must have: GetText(int) 
 *  method.
 */
class USELESS_API ComboRelation : public AppearRelation
{
public:
    /*! \a activator - a widget that activates menu
     *  \a menu_context - a widget which contains menu;
     *  this widget is to be used as a target for show/hide effect
     *  \a menu - a menu widget (f.e. ListWidget)
     */
    ComboRelation( CaptionButton &activator, IWActive &menu_context, IWList &menu );
    virtual ~ComboRelation();

private:
    Signal::FuncID _on_change_set_text;

};
};//namespace Useless
/////////////////////////////////////////////////////////////////////////////////////

#endif//__INCLUDED_USELESS_COMBO_RELATION_H__
