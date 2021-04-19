#ifndef __INCLUDED_USELESS_DATA_CONNECTOR_H__
#define __INCLUDED_USELESS_DATA_CONNECTOR_H__

#include "Useless/GUI/Misc/DataStream.h"
#include "Useless/GUI/Misc/DataFilter.h"
#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/GUI/Misc/DisplayState.h"
#include "Useless/Functional/Signal.h"

namespace Useless {

/*! DataConnector - something like cable between database and DataTableWidget
 *
 *  DataTableWidget uses it to query data from database
 *
 *  Database uses it to notify about changes
 */
class USELESS_API DataConnector
{
public:
    // make sure we are polymorphic
    virtual ~DataConnector() {}
    
    //! Those methods won't consider padding. It must be added after calling them.
    //! Get minimum column width to make all entries fit cell-space
    virtual int GetMinWidth ( long column ) const { return 0; }
    //! Get minimum row height to make all entry attributes fit cell-space
    virtual int GetMinHeight( long row ) const { return 0; }
    //! Paint entry-attribute inside given cell-space at given DisplayState
    virtual void Paint( long  row, 
                        long  column,
                       const  Rect &cell_space,
                       const  Painter &painter,
                DisplayState  state ) = 0;

    //! Get row data (uses current DataFilter)
    virtual DataStream GetData( int sel_no ) const
    {
        return DataStream();
    }
    
    //! Get headers (uses current DataFilter)
    virtual DataStream GetHeaders() const
    {
        return DataStream();
    }

    // DataConnector has direct connetion to database so it knows
    // how to sort data or how tell database to sort these data
    virtual void SortData( int by_which_column, bool ascending=true ) {}

    // To achieve complete functionality we need data to be filtered
    virtual void SetFilter( SPointer< DataFilter > filter )
    {
        _filter = filter;
    }
    
    SPointer< DataFilter > GetFilter() const
    {
        return _filter;
    }

    // Find next row where data compares to data
    virtual int FindNext( const DataStream &data, int from=-1 ) const { return -1; }

    // Data connector might want to inform table about changed data in database
    // I think this should be natural to give it signal
    // Regular data-table would tie Sync(int,int,int,int) method to this signal
    Signal_4<int,int,int,int> OnDataChange; // col, row, num_columns, num_rows (x,y,w,h)

private:
    SPointer< DataFilter > _filter;
};


}; // namespace Useless


#endif //__INCLUDED_USELESS_DATA_CONNECTOR_H__
