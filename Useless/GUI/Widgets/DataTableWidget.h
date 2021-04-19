#ifndef __INCLUDED_USELESS_DATA_TABLE_WIDGET_H__
#define __INCLUDED_USELESS_DATA_TABLE_WIDGET_H__

#include "Useless/GUI/Widgets/TableWidget.h"
#include "Useless/GUI/Widgets/DataConnector.h"
#include "Useless/Util/Shares.h"

namespace Useless {

/*! \ingroup Widgets
 *  DataTableWidget is kind of TableWidget which uses USER DEFINED DataConnector to paint data
 *  into talbe fields.
 */
class USELESS_API DataTableWidget : virtual public TableWidget
{
public:
    DataTableWidget( const ArraySkin &skin  );
    DataTableWidget( const TableStyle &style);
    virtual ~DataTableWidget();

    virtual  void  Repaint( const WidgetPainter &painter);
    virtual  void  Resize( int w, int h);

             void  SetDataConnector( const Share< DataConnector > &sh_data_connector );
			 Share< DataConnector > GetDataConnector(){ return _sh_data_connector; }
             void  SyncData  ( int col, int row, int num_columns, int num_rows );

    virtual DisplayState GetPaintState( int col, int row) const { return DisplayState::NORMAL; }

	Signal OnDataConnect;
protected:
    void  RepaintImpl( const WidgetPainter &painter);
    void  ResizeImpl( int w, int h) {}
    void  UpdateTableImpl() {}

private:
    Share< DataConnector > _sh_data_connector;
    Signal::FuncID         _on_data_change;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_DATA_TABLE_WIDGET_H__
