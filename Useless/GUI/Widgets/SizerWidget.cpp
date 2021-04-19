#include "UselessPch.h"

#include "Useless/GUI/Widgets/SizerWidget.h"
#include "Useless/GUI/Widgets/TableWidget.h"
#include "Useless/GUI/WidgetEnvironment.h"

namespace Useless {

SizerWidget::SizerWidget( const ArraySkin &skin ):
    HubWidget(0,0), _sp_skin(&(CopyOf<ArraySkin>(skin))), _w(0), _h(0), _need_update(1)
{
    _frame_left    = _sp_skin->GetDimension( LEFT_MARGIN );
    _frame_top     = _sp_skin->GetDimension( TOP_MARGIN );
    _frame_right   = _sp_skin->GetDimension( RIGHT_MARGIN );
    _frame_bottom  = _sp_skin->GetDimension( BOTTOM_MARGIN );
    _horiz_divider = _sp_skin->GetDimension( HORIZ_DIV );
    _verti_divider = _sp_skin->GetDimension( VERTI_DIV );

    _w = _frame_left + _frame_right; assert(_w>=0);
    _h = _frame_top + _frame_bottom; assert(_h>=0);
}

SizerWidget::SizerWidget( const SizerStyle &style):
    HubWidget(0,0), _sp_skin(&CopyOf<ArraySkin>(style.skin)), _w(0), _h(0), _need_update(1),
    _verti_divider(0), _horiz_divider(0)
{
    if (style.frame_left>-1)   _frame_left   = style.frame_left;
    else _frame_left = _sp_skin->GetDimension( LEFT_MARGIN );

    if (style.frame_top>-1)    _frame_top    = style.frame_top;
    else _frame_top = _sp_skin->GetDimension( TOP_MARGIN );

    if (style.frame_right>-1)  _frame_right  = style.frame_right;
    else _frame_right = _sp_skin->GetDimension( RIGHT_MARGIN );

    if (style.frame_bottom>-1) _frame_bottom = style.frame_bottom;
    else _frame_bottom  = _sp_skin->GetDimension( BOTTOM_MARGIN );

    if (style.divider>-1)
    {
        if ( IsHorizontal() )      _verti_divider = style.divider;
        else                       _horiz_divider = style.divider;
    }
    else
    {
        _horiz_divider = _sp_skin->GetDimension( HORIZ_DIV );
        _verti_divider = _sp_skin->GetDimension( VERTI_DIV );
    }

    _w = _frame_left + _frame_right; assert(_w>=0);
    _h = _frame_top + _frame_bottom; assert(_h>=0);
}

SizerWidget::~SizerWidget()
{
}

void SizerWidget::Setup()
{
    SetDimensions( _w, _h );
    HubWidget::Setup();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//  WidgetSizer Methods
//
////////////////////////////////////////////////////////////////////////////////////////////
bool SizerWidget::Insert( int cellid, Widget *client, int align )
{
    if ( cellid < GetNumCells() && !Settled(cellid) )
    {
        Rect area = client->GetBoundingRect().Align( ClientRect( cellid), Rect::Alignment(align) ) ;
        Cell cell;
        cell._alignment = Rect::Alignment(align);
        cell._settler = client;
        _settlers.Insert( cellid, cell);
        _placement.Insert( client, cellid);
        assert( _placement.Find(client) );
        HubWidget::Insert( client, area.GetX(), area.GetY() );
        return true;
    }
    else return false;
}

int SizerWidget::Insert( Widget *client, int align )
{
    int n = GetNumCells();
    int l = Parallel( client->GetWidth(), client->GetHeight() );
    AddCell( l, false );
    Insert( n, client, align );
    return n;
}


Widget* SizerWidget::Retrieve( int cell)
{
    AssocVector< int, Cell>::ConstIterator iter;
    Cell *pcell = _settlers.Find( cell, &iter);
    if (pcell)
    {
        Widget *client = pcell->_settler;
        HubWidget::Retrieve( client);
        _settlers.RemoveAt( iter);
        _placement.Remove( client);
        return client;
    }
    return NULL;
}

bool SizerWidget::Remove ( int cell)
{
    Widget *client = SizerWidget::Retrieve( cell);
    if (client)
    {
        if ( const WidgetEnvironment *penv = GetEnvironment() ) { penv->GetTrashCan().Trash(client); }
        else { delete client; }
        return true;
    }
    return false;
}


bool SizerWidget::Retrieve( Widget *client)
{
    int *ppos = _placement.Find( client);
    if (ppos)
    {
        return SizerWidget::Retrieve( *ppos);
    }
    return false;
}

bool SizerWidget::Remove ( Widget *client)
{
    int *ppos = _placement.Find( client);
    if (ppos)
    {
        return SizerWidget::Remove( *ppos);
    }
    return false;
}

bool SizerWidget::Settled( int cell) const
{
    return !!_settlers.Find( cell);
}

Widget* SizerWidget::Settler( int cell) const
{
    Cell *pcell = _settlers.Find( cell);
    return pcell ? pcell->_settler : NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//  Common Methods
//
////////////////////////////////////////////////////////////////////////////////////////////
void SizerWidget::Resize( int w, int h)
{
    if (_cells.empty()) { Parallel(w,h)=Parallel(_frame_left+_frame_right,_frame_top+_frame_bottom); }

    if ( IsHorizontal() ) 
    {
        if ( _w!=w ) { _cells.calc_positions( w-(_frame_left+_frame_right) ); }
        _w = _frame_left + _frame_right + _cells.get_fit_size();
    }
    else
    {
        if (_h!=h) { _cells.calc_positions( h-(_frame_top+_frame_bottom) ); }
        _h = _frame_top + _frame_bottom + _cells.get_fit_size();
    }

    Ortho() = Ortho(w,h);

    _need_update = true;
    HubWidget::Resize( _w, _h);
}

Rect SizerWidget::GetClientRect( const Widget *sender ) const
{
    if ( HubWidget::IsChild(sender) )
    {
        const int *ppos = _placement.Find( (Widget*)sender);
        assert(ppos && "SizerWidget::GetClientRect(): Integrity error");
        return ClientRect( *ppos);
    }
    else return Rect(0,0,_w,_h);
}

void SizerWidget::UpdateSizer()
{
    SortedVector<int> dividers;
    
    const cell_t *p = &*_cells.begin();
    const cell_t *q = &*_cells.end();
    if (p!=q) while( ++p!=q )
    {
        dividers.Insert( p->position + Parallel(_frame_left,_frame_right) );
    }

    _sp_skin->SetSize( Dim2i(_w,_h) );
    if (IsHorizontal()) _sp_skin->SetVDividers(dividers);
    else                _sp_skin->SetHDividers(dividers);
    _need_update = false;

    
    AssocVector< int, Cell >::Iterator it = _settlers.Begin();
    for (; !!it; ++it )
    {
        Rect clientrect = ClientRect( it.Key() );
        Rect area = it->_settler->GetBoundingRect().Align( clientrect, it->_alignment ) ;
        Widget *p_child = it->_settler;
        DirtyLock guard_dirty(p_child);
        p_child->SetPosition( Pos( area.GetX(), area.GetY() ) );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//  Sizer Methods
//
////////////////////////////////////////////////////////////////////////////////////////////

void SizerWidget::RepaintAction( const WidgetPainter &painter)
{
    if (_need_update) UpdateSizer();
    _sp_skin->SetClipRect( GetParent()->GetClientRect(this)-GetPosition() );
    _sp_skin->Paint( painter);
}


void SizerWidget::AddCell( int size , bool fixed)
{
    int p = Parallel( _w-(_frame_left + _frame_right), _h-(_frame_top + _frame_bottom) );
    _cells.push_back( cell_t((fixed)? -size:size, p ));
    _need_update = true;
    Parallel() += size;
    Resize( _w, _h);
}

Rect SizerWidget::CellRect( int cell) const
{
    if ( _cells.exists(cell) )
    {
        int p = _cells.at(cell).position;
        int s = _cells.get_size(cell);
        if ( IsHorizontal() )
            return Rect( p, 0, s, _h-(_frame_top+_frame_bottom) ) + Pos(_frame_left,_frame_top);
        else
            return Rect( 0, p, _w-(_frame_left+_frame_right), s ) + Pos(_frame_left,_frame_top);
    }
    else return Rect();
}

Rect SizerWidget::ClientRect( int cell) const
{
    Rect cellrect = CellRect( cell );
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

void SizerWidget::ResizeCell( int cell, int size, bool fixed )
{
    if ( _cells.exists(cell) )
    {
        _cells.at(cell).size = (fixed)? -size:size;
    }
    int s = Parallel( _w-(_frame_left+_frame_right), _h-(_frame_top+_frame_bottom) );
 
    _cells.calc_positions( s );
    Resize(_w,_h);
}

void SizerWidget::RemoveCells( int cell, int num_cells )
{
    _cells.erase( _cells.begin()+cell, _cells.begin()+cell+num_cells );
    int s = Parallel( _w-(_frame_left+_frame_right), _h-(_frame_top+_frame_bottom) );
   
    _cells.calc_positions( s );
    Resize(_w,_h);
}

int SizerWidget::GetCell( int an_x ) const
{
    int x = Parallel( an_x - _frame_left, an_x - _frame_top );
    cell_division_vector::range_t rg_r;
    rg_r = _cells.cross_range( x, x );
    return rg_r.first - _cells.begin();
}

int SizerWidget::GetDivider( int an_x ) const
{
    int x = Parallel( an_x - _frame_left, an_x - _frame_top );
    cell_division_vector::range_t rg_c;
    rg_c = _cells.cross_range( x, x );
    
    if ( rg_c.first != rg_c.second )
        return rg_c.first - _cells.begin();
    else
        return -1;
}

bool SizerWidget::OverDivider( const Pos &pos   ) const
{
    return ( GetDivider( Parallel(pos.x,pos.y)!=-1 ));
}

Rect SizerWidget::GetInside( const Rect &area ) const
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

Rect SizerWidget::GetCrossing( const Rect &an_area ) const
{
    Rect area = an_area - Pos( _frame_left, _frame_top );
    int a = Parallel(area.GetX1(),area.GetY1());
    int b = Parallel(area.GetX2(),area.GetY2());

    cell_division_vector::range_t rg;
    rg = _cells.cross_range( a, b );

    int x = rg.first - _cells.begin();
    int s = rg.second - rg.first;

    //////////////////////////////////////
    // OUT:
    // X - first column crossing area
    // Y - first row crossing area
    // W - number of columns crossing area
    // H - number of rows crossing area
    return IsHorizontal() ? Rect( x, 0, s, 1) : Rect( 0, x, 1, s );
}

void SizerWidget::SetupCells( const StupidVector<Belt> &cells, bool preserve_size )
{
    int s=0; _cells.clear();
    for ( int i=0, n=cells.Size(); i<n; ++i )
    {
        Belt b = cells[i];
        _cells.push_back( (b._fixed)? -b._size:b._size );
        s += b._size;
    }
    if (!preserve_size) Parallel() = s;
    int z = Parallel( _w-(_frame_left+_frame_right), _h-(_frame_top+_frame_bottom) );

    _cells.calc_positions( z );
    Resize(_w,_h);
    OnResize.Send_2(_w,_h);
}

StupidVector<Belt> SizerWidget::GetCells() const
{
    StupidVector<Belt> out;
    const cell_t *p = &*_cells.begin();
    const cell_t *q = &*_cells.end();
    for ( int i=0; p!=q; ++p, ++i )
    {
        Belt b;
        if ( p->size<0 ) { b._fixed=true; b._size=-p->size; }
        else { b._size = _cells.get_size(i); }
        b._position = p->position + Parallel(_frame_left,_frame_top);
        out.Insert(b);
    }
    return out;
}

void SizerWidget::Sync( const TableWidget *p_table )
{
    assert( !p_table->_need_update );

    StupidVector<Belt> cells = ( IsHorizontal() )? 
        p_table->GetColumns() : p_table->GetRows();
    
    if ( !cells.empty() )
    {
        int begin = (*cells.begin())._position;
        int end = (*cells.rbegin())._position;

        (*cells.rbegin())._size = Parallel(GetWidth(),GetHeight())-end;
        (*cells.begin())._size += begin-Parallel(_frame_left,_frame_top);
    }
    SetupCells( cells );
}

};//namespace Useless
