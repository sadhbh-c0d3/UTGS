#include "UselessPch.h"

#include "Useless/GUI/Widgets/DataTableWidget.h"

namespace Useless {

DataTableWidget::DataTableWidget( const ArraySkin &skin ): TableWidget( skin)
{
}

DataTableWidget::DataTableWidget( const TableStyle &style): TableWidget(style)
{
}

DataTableWidget::~DataTableWidget()
{
}

void  DataTableWidget::Repaint( const WidgetPainter &painter)
{
    TableWidget::Repaint(painter);
    RepaintImpl(painter);
}

void  DataTableWidget::RepaintImpl( const WidgetPainter &painter)
{
    if (!_sh_data_connector) return;

    Rect client_area = GetParent()->GetClientRect(this)-GetPosition();
    Rect visibile = TableWidget::GetCrossing( client_area );
    for ( int row=visibile.GetY1(); row<visibile.GetY2(); ++row )
    for ( int col=visibile.GetX1(); col<visibile.GetX2(); ++col )
    {
        Rect cellrect = TableWidget::ClientRect( col, row);
        WidgetPainter cell_painter(painter, Pos() );
        if ( cell_painter.Intersect(cellrect) )
        {
            _sh_data_connector->Paint( row, col, cellrect, cell_painter, GetPaintState(col,row) );
            cell_painter.RestoreClipper();
        }
    }
}

void DataTableWidget::Resize( int w, int h)
{
    TableWidget::Resize( w, h);
}

void  DataTableWidget::SetDataConnector( const Share< DataConnector > &sh_data_connector )
{
    _sh_data_connector = sh_data_connector;
    OnDataConnect.Send();
    _on_data_change = Tie2Signal( _sh_data_connector->OnDataChange, this, &DataTableWidget::SyncData );
}

void  DataTableWidget::SyncData( int col, int row, int num_columns, int num_rows )
{
    Rect client_area = GetParent()->GetClientRect(this)-GetPosition();
    Rect sync(col, row, num_columns, num_rows);
    Rect visibile = TableWidget::GetCrossing( client_area );
    Rect to_redraw = sync & visibile;
    Rect lucorner = CellRect( to_redraw.GetX1(), to_redraw.GetY1() );
    Rect rdcorner = CellRect( to_redraw.GetX2()-1, to_redraw.GetY2()-1 );
    Rect dirty = Rect( lucorner.GetX1(), lucorner.GetY1(), rdcorner.GetX2()-lucorner.GetX1(), rdcorner.GetY2()-lucorner.GetY1());
    SetDirty( dirty );
}



};//namespace Useless
