#include "UselessPch.h"

#include "Useless/GUI/Widgets/TableListedItems.h"

namespace Useless {

#pragma warning(disable:4355)

TableListedItems::TableListedItems(const ArraySkin &table_skin, const SkinMan &state_skins)
    : TableWidget(table_skin), DataTableWidget(table_skin), _skins(this, state_skins)
{
}

TableListedItems::TableListedItems(const TableStyle &table_style, const SkinMan &state_skins)
    : TableWidget(table_style), DataTableWidget(table_style), _skins(this, state_skins)
{
}

TableListedItems::~TableListedItems()
{
}

void TableListedItems::UpdateTable()
{
    TableWidget::UpdateTable();
    UpdateTableImpl();
}

void TableListedItems::UpdateTableImpl()
{
    if ( GetNumItems() != _states.Size() )
    {
        _states.Resize( GetNumItems() );
    }
}

int TableListedItems::GetState( int item_no ) const
{
    if ( item_no>=0 && item_no<_states.Size() )
    {
        return _states[item_no];
    }
    return 0;
}

bool TableListedItems::SetState( int item_no, int state_no )
{
    if ( item_no>=0 && item_no<GetNumItems() )
    {
        _states[item_no] = state_no;
        SetDirty( TableWidget::RowRect(item_no) );
        return true;
    }
    return false;
}

bool TableListedItems::HasState( int item_no, int state_no )
{
    return true;
}

int TableListedItems::ItemAtPosition( const Pos &pos) const 
{ 
    if ( GetBoundingRect().IsInside( pos.x, pos.y ) ) 
    { 
        return TableWidget::GetRow(pos.y);
    }
    return -1;
}

Rect TableListedItems::GetItemBound( int item_no  ) const
{
    if ( item_no >=0 && item_no<GetNumItems() )  
    {
        return TableWidget::RowRect(item_no);
    }
    else return Rect(); 
}

void TableListedItems::PaintClients( const WidgetPainter &painter )
{
    Pos p;
    Dim2i d( GetWidth(), 0);

    Rect client_area = GetParent()->GetClientRect(this)-GetPosition();
    Rect visibile = TableWidget::GetCrossing( client_area );
    
    p.y = RowRect( visibile.GetY1() ).GetY();
    for ( int i=visibile.GetY1(); i<visibile.GetY2(); ++i )
    {
        d.y = GetRowHeight(i);

        int last_col = GetNumColumns() -1;
        Rect fa = RowRect( i );

        if ( _skins.Select( GetState(i) ) )
        {
            _skins.SetSize( Dim2i(fa.GetW(),fa.GetH()) );
        }
        _skins.Paint( painter, fa.GetPos(), fa );

        p.y += d.y;
    }
}

void TableListedItems::SetupSkin( int flags )
{
    _skins._gen_focussed = flags & GEN_FOCUSSED;
    _skins._gen_highlight = flags & GEN_HIGHLIGHT;
}


};//namespace Useless
