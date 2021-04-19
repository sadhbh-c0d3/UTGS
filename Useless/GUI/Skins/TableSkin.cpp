#include "UselessPch.h"

#include "Useless/GUI/Skins/TableSkin.h"
#include "Useless/GUI/Skins/Tiler.h"
#include "Useless/GUI/Skins/NullSkin.h"

namespace Useless {

TableSkin::TableSkin( const RefImage& image, const Rect& boundry, int x1, int x2, int x3, int y1, int y2, int y3, bool no_interrior, const Rect& hilite_outer, const Rect& hilite_inner): ImageDicer(image)
{
    _hilite_bound = hilite_outer;
    _hilite_interior = hilite_inner;
    TableDice( boundry, x1, x2, x3, y1, y2, y3, no_interrior );
}


TableSkin::~TableSkin()
{}

void TableSkin::TableDice( const Rect &bound, int x1, int x2, int x3, int y1, int y2, int y3, bool no_interrior)
{
    AssocVector<int ,Rect> assoc_rects;

    Pos p0;// = bound.GetPos();

    int x[5] = { 0, x1, x2, x3, bound.GetW() };
    int y[5] = { 0, y1, y2, y3, bound.GetH() };

    assoc_rects.Insert( TOP+HEAD, CheckedRect( x[0], y[0], x[1]-x[0], y[1]-y[0]) +p0);
    assoc_rects.Insert( TOP+BODY, CheckedRect( x[1], y[0], x[2]-x[1], y[1]-y[0]) +p0);
    assoc_rects.Insert( TOP+TAIL, CheckedRect( x[3], y[0], x[4]-x[3], y[1]-y[0]) +p0);

    assoc_rects.Insert( MIDDLE+HEAD, CheckedRect( x[0], y[1], x[1]-x[0], y[2]-y[1]) +p0);

    if ( !no_interrior )
    assoc_rects.Insert( MIDDLE+BODY, CheckedRect( x[1], y[1], x[2]-x[1], y[2]-y[1]) +p0);

    assoc_rects.Insert( MIDDLE+TAIL, CheckedRect( x[3], y[1], x[4]-x[3], y[2]-y[1]) +p0);

    assoc_rects.Insert( BOTTOM+HEAD, CheckedRect(x[0], y[3], x[1]-x[0], y[4]-y[3]) +p0);
    assoc_rects.Insert( BOTTOM+BODY, CheckedRect(x[1], y[3], x[2]-x[1], y[4]-y[3]) +p0);
    assoc_rects.Insert( BOTTOM+TAIL, CheckedRect(x[3], y[3], x[4]-x[3], y[4]-y[3]) +p0);

    assoc_rects.Insert( HEAD+HDIV , CheckedRect( x[0], y[2], x[1]-x[0], y[3]-y[2]) +p0);
    assoc_rects.Insert( BODY+HDIV , CheckedRect( x[1], y[2], x[2]-x[1], y[3]-y[2]) +p0);
    assoc_rects.Insert( TAIL+HDIV , CheckedRect( x[3], y[2], x[4]-x[3], y[3]-y[2]) +p0);

    assoc_rects.Insert( HEAD+VDIV , CheckedRect( x[2], y[0], x[3]-x[2], y[1]-y[0]) +p0);
    assoc_rects.Insert( BODY+VDIV , CheckedRect( x[2], y[1], x[3]-x[2], y[2]-y[1]) +p0);
    assoc_rects.Insert( TAIL+VDIV , CheckedRect( x[2], y[3], x[3]-x[2], y[4]-y[3]) +p0);

    assoc_rects.Insert( CROSSING  , CheckedRect( x[2], y[2], x[3]-x[2], y[3]-y[2]) +p0);

    ImageDicer::Dice(assoc_rects);
    SetSize( Dim2i(bound.GetW(), bound.GetH()) );
}


/* Paint components of table

=================================================================================*/


/* Paint backdrop
---------------------------------------------------------------------------------*/
void TableSkin::PaintBackdrop( const Painter& painter, const Pos &p )
{
    if ( Exists(SOURCE) )
    {
        _point_cache.clear();
        _rect_cache.clear();
        ThinkFrame( _point_cache, _rect_cache, p);

        ThinkBackdrop( _point_cache, _rect_cache, p );

        painter.ProjectImages( GetImage(SOURCE), _point_cache, _rect_cache );
    }
}

/* Paint frame & grid
---------------------------------------------------------------------------------*/
void TableSkin::PaintGrid    ( const Painter& painter, const Pos &p )
{
    if ( Exists(SOURCE) )
    {
        if ( _point_cache.empty() || _rect_cache.empty() )
        {
            _point_cache.clear();
            _rect_cache.clear();
            ThinkFrame( _point_cache, _rect_cache, p);
        }
        PointList points;
        RectList  rects;
        ThinkGrid( points, rects, p);
        painter.ProjectImages( GetImage(SOURCE), points, rects );

        _point_cache.clear();
        _rect_cache.clear();
    }
}


/* Generate list of rectangles to cut from source and blit destination positions

=================================================================================*/

/* Generate grid
---------------------------------------------------------------------------------*/
void TableSkin::ThinkGrid( PointList& points, RectList& rects, const Pos &p )
{
    Tiler tiler( points, rects, p);
    tiler.SetClipRect( GetClipRect() );

    // TOP/LEFT     DIVIDERS
    // BOTTOM/RIGHT DIVIDERS
    // DIVIDERS

    int i, n = _x_dividers.Size();
    int j, m = _y_dividers.Size();

    if ( Exists(BODY+VDIV) ) for( i=1; i<n-1; ++i )
    {
        int  begin = (_x_dividers[i-1] + _x_dividers[i])/2;
        int  end   = (_x_dividers[i] + _x_dividers[i+1])/2;
        int shift  = _x_dividers[i] - (end+begin-1)/2;

        Rect top_bound = Rect( begin, _rLU.GetY1(), end-begin, _rLU.GetH() );
        Rect mid_bound = Rect( begin, _rLU.GetY2(), end-begin, _rLD.GetY1()-_rLU.GetY2());
        Rect btm_bound = Rect( begin, _rLD.GetY1(), end-begin, _rLD.GetH() );
        tiler.Orient(VERTICAL);
        if ( Exists(HEAD+VDIV) )
        {
            IGraphics &g = ImageDicer::GetImage(HEAD+VDIV);
            tiler.RectCut( g.GetRect(), top_bound, Tiler::HALIGNCENTER|Tiler::VALIGNTOP, shift );
        }
 
            IGraphics &g = ImageDicer::GetImage(BODY+VDIV);
            tiler.RectFill1D( g.GetRect(), mid_bound, Tiler::HALIGNCENTER, shift );
 
        if ( Exists(TAIL+VDIV) )
        {
            IGraphics &g = ImageDicer::GetImage(TAIL+VDIV);
            tiler.RectCut( g.GetRect(), btm_bound, Tiler::HALIGNCENTER|Tiler::VALIGNBOTTOM, shift );
        }
    }

    
    if ( Exists(BODY+HDIV) ) for( j=1; j<m-1; ++j )
    {
        int  begin = (_y_dividers[j-1] + _y_dividers[j])/2;
        int  end   = (_y_dividers[j] + _y_dividers[j+1])/2;
        int shift  = _y_dividers[j] - (end+begin-1)/2;

        Rect top_bound = Rect( _rLU.GetX1(), begin, _rLU.GetW(), end-begin );
        Rect mid_bound = Rect( _rLU.GetX2(), begin, _rRU.GetX1()-_rLU.GetX2(), end-begin);
        Rect btm_bound = Rect( _rRU.GetX1(), begin, _rRU.GetW(), end-begin );
        tiler.Orient(HORIZONTAL);
        if ( Exists(HEAD+HDIV) )
        {
            IGraphics &g = ImageDicer::GetImage(HEAD+HDIV);
            tiler.RectCut( g.GetRect(), top_bound, Tiler::VALIGNCENTER|Tiler::HALIGNLEFT, shift );
        }
            
            IGraphics &g = ImageDicer::GetImage(BODY+HDIV);
            tiler.RectFill1D( g.GetRect(), mid_bound, Tiler::VALIGNCENTER, shift );

        if ( Exists(TAIL+HDIV) )
        {
            IGraphics &g = ImageDicer::GetImage(TAIL+HDIV);
            tiler.RectCut( g.GetRect(), btm_bound, Tiler::VALIGNCENTER|Tiler::HALIGNRIGHT, shift );
        }
    }

    if ( Exists(CROSSING) )
    {
        for( i=1; i<n-1; ++i) for( j=1; j<m-1; ++j)
        {
            int xb = (_x_dividers[i-1] + _x_dividers[i])/2;
            int yb = (_y_dividers[j-1] + _y_dividers[j])/2;
            int xe = (_x_dividers[i+1] + _x_dividers[i])/2;
            int ye = (_y_dividers[j+1] + _y_dividers[j])/2;
            int xs = _x_dividers[i] - (xe+xb-1)/2;
            int ys = _y_dividers[j] - (ye+yb-1)/2;
            Rect r = Rect( xb, yb, xe-xb, ye-yb );
            IGraphics &g = ImageDicer::GetImage(CROSSING);
            tiler.Orient(VERTICAL);
            tiler.RectCut( g.GetRect(), r, Tiler::VALIGNCENTER|Tiler::HALIGNCENTER, xs, ys);
        }
    }
}


/* Get dimentions information
---------------------------------------------------------------------------------*/
int TableSkin::GetDimension( int dimID ) const
{
    switch( dimID )
    {
    case VERTI_DIV:     return Exists(HDIV+BODY)   ? GetImage(HDIV+BODY).GetRect().GetH()   : 0;
    case HORIZ_DIV:     return Exists(VDIV+BODY)   ? GetImage(VDIV+BODY).GetRect().GetW()   : 0;
    }
    return FrameSkin::GetDimension( dimID );
}

SPointer<Skin> TableSkin::CreateSubSkin( int skinID ) const
{
    if( !!_hilite_bound && !!_hilite_interior && skinID==HIGHLIGHT )
    {
        return new FrameSkin( GetSource(), _hilite_bound, _hilite_interior);
    }
    else
    {
        return new NullSkin();
    }
}


/* Set row and column dividers
-------------------------------------------------------------------------------*/
void TableSkin::SetHDividers( const SortedVector<int> &y_dividers )
{
    _y_dividers = y_dividers; _y_dividers.Insert(0); _y_dividers.Insert(_height);
}

void TableSkin::SetVDividers( const SortedVector<int> &x_dividers )
{
    _x_dividers = x_dividers; _x_dividers.Insert(0); _x_dividers.Insert(_width);
}

};//namespace Useless
