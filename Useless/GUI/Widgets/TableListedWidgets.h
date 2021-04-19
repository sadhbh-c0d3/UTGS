#ifndef __INCLUDED__USELESS__TABLE_LISTED_WIDGETS_H__
#define __INCLUDED__USELESS__TABLE_LISTED_WIDGETS_H__

#include "Useless/GUI/Widgets/WidgetTableWidget.h"
#include "Useless/GUI/Widgets/TableListedItems.h"

namespace Useless {

class USELESS_API TableListedWidgets : public WidgetTableWidget,public TableListedItems
{
public:
    TableListedWidgets( const ArraySkin &table_skin, const SkinMan &state_skins );
    TableListedWidgets( const TableStyle &table_style, const SkinMan &state_skins );
    virtual ~TableListedWidgets();

    virtual  void  Repaint( const WidgetPainter &painter );
    virtual  void  Resize( int w, int h );
    virtual  void  UpdateTable();

    DEFINE_CYCLIC_VISITABLE(TableListedItems);
};
};//namespace Useless

//----------------------------------------------------------------------------------
#include "Useless/GUI/Widgets/TableListWidget.h"
#include "Useless/GUI/Logic/ListLogic.h"

namespace Useless {

class USELESS_API TableList : public TableListedWidgets, public ListLogic
{
public:
    TableList( const ArraySkin &table_skin, const SkinMan &state_skins );
    TableList( const TableListStyle &table_style );
    TableList( const ArraySkin &table_skin );
    TableList( const TableStyle &style );
    virtual ~TableList();

	virtual bool Accept( const Widget::Request &req );

    virtual void Setup() { TableListedWidgets::Setup(); ListLogic::Setup(); }

};

};//namespace Useless
#endif//__INCLUDED__USELESS__TABLE_LISTED_WIDGETS_H__
