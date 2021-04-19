#include "UselessPch.h"

#include "Useless/GUI/Widgets/TableWidget.h"
#include "Useless/GUI/Widgets/SizerWidget.h"

namespace Useless {

TableWidget::TableWidget(  const ArraySkin &skin ):
    _sp_skin(&(CopyOf<ArraySkin>(skin))), _need_update(1), _w(0), _h(0)
{
    _frame_left    = _sp_skin->GetDimension( LEFT_MARGIN );
    _frame_top     = _sp_skin->GetDimension( TOP_MARGIN );
    _frame_right   = _sp_skin->GetDimension( RIGHT_MARGIN );
    _frame_bottom  = _sp_skin->GetDimension( BOTTOM_MARGIN );
    _horiz_divider = _sp_skin->GetDimension( HORIZ_DIV );
    _verti_divider = _sp_skin->GetDimension( VERTI_DIV );

    _w = _frame_left + _frame_right; assert(_w>=0);
    _h = _frame_top + _frame_bottom; assert(_h>=0);

    SetDimensions(_w,_h);
}

TableWidget::TableWidget( const TableStyle &style):
    _sp_skin(&(CopyOf<ArraySkin>(style.skin))), _need_update(1), _w(0), _h(0)
{
    if (style.frame_left>-1)   _frame_left   = style.frame_left;
    else _frame_left = _sp_skin->GetDimension( LEFT_MARGIN );

    if (style.frame_top>-1)    _frame_top    = style.frame_top;
    else _frame_top = _sp_skin->GetDimension( TOP_MARGIN );

    if (style.frame_right>-1)  _frame_right  = style.frame_right;
    else _frame_right = _sp_skin->GetDimension( RIGHT_MARGIN );

    if (style.frame_bottom>-1) _frame_bottom = style.frame_bottom;
    else _frame_bottom  = _sp_skin->GetDimension( BOTTOM_MARGIN );

    if (style.horiz_divider>-1) _horiz_divider = style.horiz_divider;
    else _horiz_divider = _sp_skin->GetDimension( HORIZ_DIV );

    if (style.verti_divider>-1) _verti_divider = style.verti_divider;
    else _verti_divider = _sp_skin->GetDimension( VERTI_DIV );

    _w = _frame_left + _frame_right; assert(_w>=0);
    _h = _frame_top + _frame_bottom; assert(_h>=0);
}

TableWidget::~TableWidget()
{}

void TableWidget::Setup()
{
    //SetDimensions(_w,_h);
    UpdateTable();
}

void TableWidget::AddColumn( int width , bool fixed)
{
    _columns.push_back( cell_t((fixed)? -width:width, _w-(_frame_left + _frame_right)) );
    _need_update = true;
    _w += width;
    Resize( _w, _h);
}

void TableWidget::AddRow( int height, bool fixed)
{
    _rows.push_back( cell_t((fixed)? -height:height, _h-(_frame_top + _frame_bottom)) );
    _need_update = true;
    _h += height;
    Resize( _w, _h);
}

Rect TableWidget::CellRect( int column, int row) const
{
    if ( _columns.exists(column) && _rows.exists(row) )
    {
        return Rect( _columns.at(column).position, _rows.at(row).position,
                     _columns.get_size(column), _rows.get_size(row) ) + Pos(_frame_left,_frame_top);
    }
    else return Rect();
}
Rect TableWidget::ClientRect( int column, int row) const
{
    Rect cellrect = CellRect( column, row);
    if(!! cellrect)
    {
        int x[2], y[2];
        int vdl = _verti_divider/2;
        int vdr = _verti_divider-vdl;
        int hdl = _horiz_divider/2;
        int hdr = _horiz_divider-hdl;

        x[0] = cellrect.GetX1()+vdr;
        x[1] = cellrect.GetX2()-vdl;
        y[0] = cellrect.GetY1()+hdr;
        y[1] = cellrect.GetY2()-hdl;

        return Rect( x[0], y[0], (x[1]-x[0]>0)? x[1]-x[0]:0, (y[1]-y[0]>0)? y[1]-y[0]:0 );
    }
    else return Rect();
}

int TableWidget::GetRowHeight( int row ) const 
{
    return _rows.get_size( row ); 
}

int TableWidget::GetColumnWidth( int column ) const 
{
    return _columns.get_size( column ); 
}

StupidVector<Belt> TableWidget::GetColumns() const
{
    StupidVector<Belt> out;
    const cell_t *p = _columns.empty()? NULL : &*_columns.begin();
    const cell_t *q = p + _columns.size();
    for ( int i=0; p!=q; ++p, ++i )
    {
        Belt b;
        if ( p->size<0 ) { b._fixed=true; b._size=-p->size; }
        else { b._size = _columns.get_size(i); }
        b._position = p->position + _frame_left;
        out.Insert(b);
    }
    return out;
}

StupidVector<Belt> TableWidget::GetRows() const
{
    StupidVector<Belt> out;
    const cell_t *p = _rows.empty() ? NULL : &*_rows.begin();
    const cell_t *q = p + _rows.size();
    for ( int i=0; p!=q; ++p, ++i )
    {
        Belt b;
        if ( p->size<0 ) { b._fixed=true; b._size=-p->size; }
        else { b._size = _rows.get_size(i); }
        b._position = p->position + _frame_top;
        out.Insert(b);
    }
    return out;
}

void TableWidget::Repaint( const WidgetPainter &painter)
{
    if (_need_update) { UpdateTable(); }
    _sp_skin->SetClipRect( GetParent()->GetClientRect(this)-GetPosition() );
    _sp_skin->PaintBackdrop( painter);
    PaintClients(painter );
    _sp_skin->PaintGrid( painter);
}


void TableWidget::Resize( int w, int h )
{
    if (_columns.empty()) { w=_frame_left+_frame_right; }
    if (_rows.empty()) { h=_frame_top+_frame_bottom; }

    if (_w!=w) { _columns.calc_positions( w-(_frame_left+_frame_right) ); }
    if (_h!=h) { _rows.calc_positions( h-(_frame_top+_frame_bottom) ); }

    _w = _frame_left + _frame_right + _columns.get_fit_size();
    _h = _frame_top + _frame_bottom + _rows.get_fit_size();

    _need_update = true;

    SetDimensions( (_w>w)? _w:w, (_h>h)? _h:h ); // Allow extending size beyond table
}

void TableWidget::UpdateTable()
{
    SortedVector<int> xdividers;
    SortedVector<int> ydividers;
    
    const cell_t *p = _columns.empty() ? NULL : &*_columns.begin();
    const cell_t *q = p + _columns.size();
    if (p!=q) while( ++p!=q )
    {
        xdividers.Insert( p->position + _frame_left );
    }

    p = _rows.empty() ? NULL : &*_rows.begin();
    q = p + _rows.size();
    if (p!=q) while( ++p!=q )
    {
        ydividers.Insert( p->position + _frame_top );
    }

    //_sp_skin->SetSize( Dim2i(_w,_h) );
    _sp_skin->SetSize( Dim2i( GetWidth(), GetHeight()) );
    _sp_skin->SetVDividers(xdividers);
    _sp_skin->SetHDividers(ydividers);
    _need_update = false;
    
    OnTableUpdate.Send();
}

void  TableWidget::ResizeColumn( int column, int width, bool fixed )
{
    if ( _columns.exists(column) )
    {
        _columns.at(column).size = (fixed)? -width:width;
    }
    _columns.calc_positions( _w-(_frame_left+_frame_right) );
    Resize(_w,_h);
}

void  TableWidget::ResizeRow( int row, int height, bool fixed )
{
    if ( _rows.exists(row) )
    {
        _rows.at(row).size = (fixed)? -height:height;
    }
    _rows.calc_positions( _h-(_frame_top+_frame_bottom) );
    Resize(_w,_h);
}

void  TableWidget::RemoveColumns( int column, int num_columns )
{
    _columns.erase( _columns.begin()+column, _columns.begin()+column+num_columns );
    _columns.calc_positions( _w-(_frame_left+_frame_right) );
    Resize(_w,_h);
}

void  TableWidget::RemoveRows( int row, int num_rows )
{
    _rows.erase( _rows.begin()+row, _rows.begin()+row+num_rows );
    _rows.calc_positions( _h-(_frame_top+_frame_bottom) );
    Resize(_w,_h);
}

void TableWidget::SetupColumns( const StupidVector<Belt> &columns, bool preserve_width )
{
    int s=0; _columns.clear();
    for ( int i=0, n=columns.Size(); i<n; ++i )
    {
        Belt b = columns[i];
        _columns.push_back( (b._fixed)? -b._size:b._size );
        s += b._size;
    }
    _w = (preserve_width ? _w : s + (_frame_left+_frame_right));
    _columns.calc_positions( _w - (_frame_left+_frame_right));
    Resize(_w,_h);
}

void TableWidget::SetupRows( const StupidVector<Belt> &rows, bool preserve_height  )
{
    int s=0; _rows.clear();
    for ( int i=0, n=rows.Size(); i<n; ++i )
    {
        Belt b = rows[i];
        _rows.push_back( (b._fixed)? -b._size:b._size );
        s += b._size;
    }
    _h = (preserve_height ? _h : s + (_frame_top+_frame_bottom));
    _rows.calc_positions( _h - (_frame_top+_frame_bottom));
    Resize(_w,_h);
}

void TableWidget::Sync( const SizerWidget *p_sizer )
{
    assert( !p_sizer->_need_update );
    StupidVector<Belt> cells = p_sizer->GetCells();
    bool horiz = p_sizer->IsHorizontal();
    
    if ( !cells.empty() )
    {
        int begin = (*cells.begin())._position;
        int end = (*cells.rbegin())._position;

        (*cells.rbegin())._size = ((horiz) ? GetWidth() : GetHeight())-end;
        (*cells.begin())._size += begin-((horiz)? _frame_left : _frame_top);
    }
    if (horiz) SetupColumns( cells ); else SetupRows( cells );
}

Rect TableWidget::GetInside( const Rect &area ) const //not working
{
    assert(0);

    //////////////////////////////////////
    // OUT:
    // X - first column inside area
    // Y - first row inside area
    // W - number of columns inside area
    // H - number of columns inside area
    return GetCrossing(area);
}

Rect TableWidget::GetCrossing( const Rect &an_area ) const
{
    Rect area = an_area - Pos( _frame_left, _frame_top );
    //return Rect(0,0,_columns.size(),_rows.size());
    cell_division_vector::range_t rg_c;
    rg_c = _columns.cross_range( area.GetX1(), area.GetX2() );

    cell_division_vector::range_t rg_r;
    rg_r = _rows.cross_range( area.GetY1(), area.GetY2() );

    int x = rg_c.first - _columns.begin();
    int y = rg_r.first - _rows.begin();
    int w = rg_c.second - rg_c.first;
    int h = rg_r.second - rg_r.first;
    if ( x<0 ) { x=0; --w; }
    if ( y<0 ) { y=0; --h; }

    //////////////////////////////////////
    // OUT:
    // X - first column crossing area
    // Y - first row crossing area
    // W - number of columns crossing area
    // H - number of rows crossing area
    return Rect(x,y, (w>0)? w:0, (h>0)? h:0);
}

int TableWidget::GetRow( int an_y ) const
{
    if ( an_y > _h ) { return -1; }
    int y = an_y - _frame_top;
    cell_division_vector::range_t rg_r;
    rg_r = _rows.cross_range( y, y );
    return rg_r.first - _rows.begin();
}

int TableWidget::GetColumn( int an_x ) const
{
    if ( an_x > _w ) { return -1; }
    int x = an_x - _frame_left;
    cell_division_vector::range_t rg_c;
    rg_c = _columns.cross_range( x, x );
    return rg_c.first - _columns.begin();
}

int TableWidget::GetHDivider( int an_y ) const
{
    int y = an_y - _frame_top;
    cell_division_vector::range_t rg_r;
    rg_r = _rows.cross_range( y-2, y+2 );
    
    if ( rg_r.first != rg_r.second )
        return rg_r.first - _rows.begin();
    else
        return -1;
}

int TableWidget::GetVDivider( int an_x ) const
{
    int x = an_x - _frame_left;
    cell_division_vector::range_t rg_c;
    rg_c = _columns.cross_range( x, x );
    
    if ( rg_c.first != rg_c.second )
        return rg_c.first - _columns.begin();
    else
        return -1;
}

bool TableWidget::OverDivider( const Pos &pos   ) const
{
    return (GetVDivider(pos.x)!=-1 && GetHDivider(pos.y)!=-1);
}



Rect TableWidget::ColumnRect( int column ) const
{
    int last_row = GetNumRows() -1;
    
    Rect R = CellRect( column, 0) | CellRect( column, last_row);
    Rect r = ClientRect( column, 0) | ClientRect( column, last_row);

    return Rect( r.GetX1(), R.GetY1(), r.GetW(), R.GetH() );
}

Rect TableWidget::RowRect( int row ) const
{
    int last_col = GetNumColumns() -1;
    
    Rect R = CellRect( 0, row) | CellRect( last_col, row);
    Rect r = ClientRect( 0, row) | ClientRect( last_col, row);

    return Rect( R.GetX1(), r.GetY1(), R.GetW(), r.GetH() );
}

};//namespace Useless
