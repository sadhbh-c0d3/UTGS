#ifndef __INCLUDED_USELESS_TABLE_PAINTER_H__
#define __INCLUDED_USELESS_TABLE_PAINTER_H__

#include "Useless/GUI/Widgets/DataConnector.h"
#include "Useless/GUI/Display/WidgetPainter.h"

namespace Useless {

class USELESS_API TablePainter : public DataConnector
{
public:
    virtual ~TablePainter() {}

/* Paint v. 2.0
-----------------------------------------------------*/
    virtual void Paint( long  row, 
                        long  column,
                       const  Rect &cell_space,
                       const  Painter &painter,
                DisplayState  state ) 
    { Paint(row, column, cell_space, (const  WidgetPainter &)painter, state.Get(DisplayState::HIGHLIGHTEN));  }


/* Paint v. 1.0
-----------------------------------------------------*/
    virtual void Paint( long  row, 
                        long  column,
                       const  Rect &cell_space,
                       const  WidgetPainter &painter,
                        bool  highliten=false ) {}

};

};//namespace Useless

#endif//__INCLUDED_USELESS_TABLE_PAINTER_H__
