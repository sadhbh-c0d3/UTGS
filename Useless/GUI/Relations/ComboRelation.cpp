#include "UselessPch.h"

#include "Useless/GUI/Relations/ComboRelation.h"
#include "Useless/GUI/Widgets/CompoundWidget.h"
#include "Useless/GUI/Widgets/ListWidget.h"
#include "Useless/GUI/Widgets/TextButtonWidget.h"

namespace Useless {

ComboRelation::ComboRelation( CaptionButton &activator, IWActive &menu_context, IWList &menu ):
    AppearRelation( menu_context.Atom(), true )
{

    AppearRelation::AddSignal( menu_context.Control().OffPress[0], false);
    AppearRelation::AddSignal( menu_context.Control().OffPress[1], false);
    AppearRelation::AddSignal( menu_context.Control().OffPress[2], false);
    AppearRelation::AddSignal( menu.Items().OnConfirm, false);
    AppearRelation::AddSignal( activator.OnClick[0], true, true);
    AppearRelation::Appear(false);
    AppearRelation::OnShow.TieVoid( menu_context.AtomPtr(), &Widget::FocusIntercept );
    AppearRelation::OnHide.TieVoid( menu_context.AtomPtr(), &Widget::FocusRelease );

    // Make combo button text dependant on selected item on the menu list
    _on_change_set_text = menu.Items().OnConfirm.TieUnary
        ( 
            &activator,
            &CaptionWidget::SetCaption, 
            PackTranslator( &menu.Items(), &ListWidget::GetText, -1, Text() )
        );
}

ComboRelation::~ComboRelation()
{
    _on_change_set_text.Untie();
}

};//namespace Useless
