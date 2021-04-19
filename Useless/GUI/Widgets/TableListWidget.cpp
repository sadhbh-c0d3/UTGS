#include "UselessPch.h"

#include "Useless/GUI/Widgets/TableListWidget.h"
#include "Useless/GUI/Skins/SkinSet.h"

namespace Useless {

TableListWidget::TableListWidget( const ArraySkin &skin, const SkinMan &state_skins )
        :TableListedItems( skin, state_skins ),
         TableWidget(skin),
         DataTableWidget(skin)
{}

TableListWidget::TableListWidget( const TableListStyle &style)
        :TableListedItems( style, style.state_skins ),
         TableWidget( style ),
         DataTableWidget( style )
{}

TableListWidget::~TableListWidget()
{}

TableListWidget::TableListWidget( const ArraySkin &skin )
        :TableListedItems( skin, SkinSet( skin.CreateSubSkin(HIGHLIGHT), ListLogic::SELECTED) ),
         TableWidget( skin ),
         DataTableWidget( skin )
{}

TableListWidget::TableListWidget( const TableStyle &style)
        :TableListedItems( style, SkinSet( style.skin.CreateSubSkin(HIGHLIGHT), ListLogic::SELECTED) ),
         TableWidget( style ),
         DataTableWidget( style )
{}


};//namespace Useless
