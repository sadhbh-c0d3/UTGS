#ifndef __INCLUDED__USELESS_TABLE_MENU_H__
#define __INCLUDED__USELESS_TABLE_MENU_H__

#include "Useless/GUI/Widgets/DropDown.h"
#include "Useless/GUI/Widgets/DataConnector.h"
#include "Useless/GUI/Widgets/TableSystemWidget.h"

namespace Useless {

//USES
    class TableListStyle;
    class TableListWidget;

class USELESS_API TableMenu : public DropDown, public IWTableList
{
public:
    TableMenu( const CaptionStyle &style, IWTableList *p_items, DataConnector *p_data );
    virtual ~TableMenu();

//#   include "TableMenu.inl" - ParmList Ctor - unsupported

    virtual DataPointer GetSelectedData() const;
    virtual void SetMenuHeight  ( int h);

    virtual TableWidget& Table() { return _p_items->Table(); }

private:
    IWTableList      *_p_items;
    DataConnector    *_p_data;
};

};
#endif//__INCLUDED__USELESS_TABLE_MENU_H__
