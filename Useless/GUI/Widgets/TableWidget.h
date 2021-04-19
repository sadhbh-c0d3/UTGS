#ifndef __INCLUDED_USELESS_TABLEWIDGET_H__
#define __INCLUDED_USELESS_TABLEWIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Skins/ArraySkin.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/GUI/Widgets/detail/Belt.h"
#include "Useless/Util/Storage/cell_division_vector.h"

namespace Useless {

//USES
    class TableStyle;
/*! \ingroup Widgets
 *  TableWidget is pure table. It is completely passive widget.
 *  It's goal is to draw table grid with or without backgroud.
 *  It has also hilite capability, which means it may hilite
 *  any number of rows and columns inside row/col-rectangle.
 */
class USELESS_API TableWidget : virtual public Widget, public IWTable
{
public:
    TableWidget(  const ArraySkin &skin );
    TableWidget( const TableStyle &style);
    virtual ~TableWidget();

    virtual TableWidget& Table() { return *this; }

    virtual void  Repaint     ( const WidgetPainter &painter );
    virtual void  PaintClients( const WidgetPainter &painter ) {}
    virtual void  Resize( int w, int h);

            void  AddColumn( int width , bool fixed=false);
            void  AddRow   ( int height, bool fixed=false);

             int  GetNumColumns() const { return _columns.size();}
             int  GetNumRows()    const { return    _rows.size();}

            void  ResizeColumn( int column, int width, bool fixed=false );
            void  ResizeRow   ( int row,   int height, bool fixed=false );

             int  GetRowHeight  ( int row )    const;
             int  GetColumnWidth( int column ) const;

    virtual void  RemoveColumns( int column, int num_columns );
    virtual void  RemoveRows   ( int row   , int num_rows );

    virtual void  Setup();
    virtual void  UpdateTable();

    Rect CellRect  ( int column, int row) const;
    Rect ClientRect( int column, int row) const;
    Rect ColumnRect( int column )         const;
    Rect RowRect   ( int row )            const;

     int GetRow     ( int y ) const;
     int GetColumn  ( int x ) const;
     int GetHDivider( int y ) const;
     int GetVDivider( int x ) const;
    bool OverDivider( const Pos &pos   ) const;

    Rect GetInside  ( const Rect &area ) const;
    Rect GetCrossing( const Rect &area ) const;

    void  SetupColumns( const StupidVector<Belt> &columns, bool preserve_width = false );
    void  SetupRows   ( const StupidVector<Belt> &rows   , bool preserve_height= false );
    
    StupidVector<Belt> GetColumns() const;
    StupidVector<Belt> GetRows   () const;

    void Sync( const SizerWidget *p_sizer );

    Signal OnTableUpdate;

    DEFINE_CYCLIC_VISITABLE(Widget);

protected:
    bool                _need_update;

private:
    SPointer<ArraySkin> _sp_skin;
    int                 _w;
    int                 _h;

    cell_division_vector _columns;
    cell_division_vector _rows;

    int                 _frame_left;
    int                 _frame_top;
    int                 _frame_right;
    int                 _frame_bottom;
    int                 _horiz_divider;
    int                 _verti_divider;

    friend class SizerWidget;
};
////////////////////////////////////////////////////////////////////////////

class TableStyle
{
public:
    TableStyle( const ArraySkin &skin, int l=0, int t=0, int r=0, int b=0, int hd=0, int vd=0): 
        skin(skin), frame_left(l), frame_top(t), frame_right(r), frame_bottom(b),
        horiz_divider(hd), verti_divider(vd) {}

    const ArraySkin &skin;
    int              frame_left;
    int              frame_top;
    int              frame_right;
    int              frame_bottom;
    int              horiz_divider;
    int              verti_divider;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_TABLEWIDGET_H__
