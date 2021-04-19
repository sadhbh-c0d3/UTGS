#include "UselessPch.h"

#include "Useless/GUI/Widgets/TableMenu.h"
#include "Useless/GUI/Widgets/TableListWidget.h"
#include "Useless/ErrorConfig.h"

#include "Useless/GUI/Misc/DataTypes.h" // DT_Text in CTor used (compatibility W/A)

namespace Useless {

TableMenu::TableMenu( const CaptionStyle &style, IWTableList *p_items , DataConnector *p_data)
    : _p_items(p_items), DropDown( style ),_p_data(p_data)
{
    Tie2Signals(_p_items);
}


TableMenu::~TableMenu()
{}

DataPointer TableMenu::GetSelectedData() const
{
    int selected = _p_list->Enumerator().GetSelected();

    DT_SingleRow *p_row_data = new DT_SingleRow();
    p_row_data->_stream = _p_data->GetData( selected );
    int n = _p_items->Table().GetNumColumns();

    StupidVector<Belt> columns = _p_items->Table().GetColumns();

    for ( int i=0; i!=n; ++i )
    {
        if ( columns[i]._fixed )
            p_row_data->_cells.push_back( -columns[i]._size );
        else
            p_row_data->_cells.push_back( columns[i]._size );
    }

    DataPointer ptr( p_row_data );
    return ptr;
}

void TableMenu::SetMenuHeight  ( int h)
{
    if ( _p_items->AtomPtr() != _p_items->Table().AtomPtr() ) //w/a for TableSystem check
    {
        int  ih = _p_items->Table().GetHeight();
        Rect ir = _p_items->Table().GetParent()->GetClientRect( &_p_items->Table() );
        Rect lr = _p_items->Atom().GetBoundingRect();
        int  dy = lr.GetH() - ir.GetH();
        int  max_h = ih + ((dy>0)? dy:0);
        h = (h>max_h)? max_h : h;

        _p_items->Atom().Resize( _p_items->Atom().GetWidth(), h );
    }
}


};//namespace Useless
