#include "UselessPch.h"

#include "TableListedWidgets.h"
//----------------------------------
#include "Useless/GUI/Skins/SkinSet.h"
#include "Useless/GUI/Widgets/WidgetRequest.h"

namespace Useless {

TableListedWidgets::TableListedWidgets( const ArraySkin &table_skin, const SkinMan &state_skins )
     :TableWidget(table_skin),
      DataTableWidget(table_skin),
      TableListedItems(table_skin,state_skins),
      WidgetTableWidget(table_skin)
{
}

TableListedWidgets::TableListedWidgets( const TableStyle &table_style, const SkinMan &state_skins )
     :TableWidget(table_style),
      DataTableWidget(table_style),
      TableListedItems(table_style,state_skins),
      WidgetTableWidget(table_style)
{
}

TableListedWidgets::~TableListedWidgets()
{
}

void TableListedWidgets::Repaint( const WidgetPainter &painter )
{
    TableWidget::Repaint( painter );
    DataTableWidget::RepaintImpl( painter );
    TableListedItems::RepaintImpl( painter );
    WidgetTableWidget::RepaintImpl( painter );
}

void TableListedWidgets::Resize( int w, int h )
{
    TableWidget::Resize( w, h );
    DataTableWidget::ResizeImpl( w, h );
    TableListedItems::ResizeImpl( w, h );
    WidgetTableWidget::ResizeImpl( w, h );
}

void TableListedWidgets::UpdateTable()
{
    TableWidget::UpdateTable();
    DataTableWidget::UpdateTableImpl();
    TableListedItems::UpdateTableImpl();
    WidgetTableWidget::UpdateTableImpl();
}

//----------------------------------------------------------------------------------

TableList::TableList( const ArraySkin &table_skin, const SkinMan &state_skins )
     :TableWidget(table_skin),
      DataTableWidget(table_skin),
      TableListedWidgets(table_skin,state_skins)
{}

TableList::TableList( const TableListStyle &table_style )
     :TableWidget(table_style),
      DataTableWidget(table_style),
      TableListedWidgets(table_style,table_style.state_skins)
{}

TableList::TableList( const ArraySkin &table_skin )
     :TableWidget(table_skin),
      DataTableWidget(table_skin),
      TableListedWidgets(table_skin,
        SkinSet( table_skin.CreateSubSkin(HIGHLIGHT), ListLogic::SELECTED) 
       )
{}


TableList::TableList( const TableStyle &style )
     :TableWidget(style),
      DataTableWidget(style),
      TableListedWidgets(style,
        SkinSet( style.skin.CreateSubSkin(HIGHLIGHT), ListLogic::SELECTED) 
       )
{}

TableList::~TableList()
{}

bool TableList::Accept( const Widget::Request &req )
{
    if ( !TableListedWidgets::Accept(req) )
    {
        return ListLogic::Accept(req);
    }
    /*else // commented while this feature may be annoying
    {
        if ( const InputCursor *p = dynamic_cast<const InputCursor*>(&req) )
        {
            InputCursor alt_req( p->Position(), p->Move(), OFFSIDE );
            //ListLogic::Accept( alt_req );
            ListLogic::Accept( req );
        }
        else if ( const InputButton *p = dynamic_cast<const InputButton*>(&req) )
        {
            InputButton alt_req( p->Status(), p->Changed(), OFFSIDE );
            //ListLogic::Accept( alt_req );
            ListLogic::Accept( req );
        }
    }*/
    return true;
}


};//namespace Useless
