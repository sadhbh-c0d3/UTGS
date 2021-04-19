#include "UselessPch.h"

#include "Useless/GUI/Skins/SizerSkin.h"
#include "Useless/GUI/Skins/Tiler.h"
#include "Useless/GUI/Skins/NullSkin.h"

namespace Useless {

SizerSkin::SizerSkin( const RefImage& image, const Rect& boundry, Orientation orient, int x1, int x2, int x3, int y1, int y2, bool noinner, const Rect &hl_outer, const Rect &hl_inner): ImageDicer(image)
{
    _hilite_bound = hl_outer;
    _hilite_interior = hl_inner;
    SizerDice( boundry, orient, x1, x2, x3, y1, y2, noinner );
}

SizerSkin::~SizerSkin()
{}

void SizerSkin::SizerDice( const Rect &bound, Orientation orient, int x1, int x2, int x3, int y1, int y2, bool noinner)
{
    AssocVector<int ,Rect> assoc_rects;
    _orient = orient;
    Pos p0;// = boundry.GetPos();

    if (_orient==HORIZONTAL)
    {
        int x[5] = { 0, x1, x2, x3, bound.GetW() };
        int y[4] = { 0, y1, y2, bound.GetH() };
        
        assoc_rects.Insert( TOP+HEAD, CheckedRect( x[0], y[0], x[1]-x[0], y[1]-y[0]) +p0);
        assoc_rects.Insert( TOP+BODY, CheckedRect( x[1], y[0], x[2]-x[1], y[1]-y[0]) +p0);
        assoc_rects.Insert( TOP+TAIL, CheckedRect( x[3], y[0], x[4]-x[3], y[1]-y[0]) +p0);

        assoc_rects.Insert( MIDDLE+HEAD, CheckedRect( x[0], y[1], x[1]-x[0], y[2]-y[1]) +p0);
        assoc_rects.Insert( MIDDLE+TAIL, CheckedRect( x[3], y[1], x[4]-x[3], y[2]-y[1]) +p0);

        assoc_rects.Insert( BOTTOM+HEAD, CheckedRect(x[0], y[2], x[1]-x[0], y[3]-y[2]) +p0);
        assoc_rects.Insert( BOTTOM+BODY, CheckedRect(x[1], y[2], x[2]-x[1], y[3]-y[2]) +p0);
        assoc_rects.Insert( BOTTOM+TAIL, CheckedRect(x[3], y[2], x[4]-x[3], y[3]-y[2]) +p0);

        assoc_rects.Insert( HEAD+DIV , CheckedRect( x[2], y[0], x[3]-x[2], y[1]-y[0]) +p0);
        assoc_rects.Insert( BODY+DIV , CheckedRect( x[2], y[1], x[3]-x[2], y[2]-y[1]) +p0);
        assoc_rects.Insert( TAIL+DIV , CheckedRect( x[2], y[2], x[3]-x[2], y[3]-y[2]) +p0);

        if (!noinner) assoc_rects.Insert( MIDDLE+BODY, Rect( x[1], y[1], x[2]-x[1], y[2]-y[1]) +p0);
        Skin::SetDirMask( Pos(1,0) );
    }
    else
    {
        int x[4] = { 0, y1, y2, bound.GetW() };
        int y[5] = { 0, x1, x2, x3, bound.GetH() };
        
        assoc_rects.Insert( TOP+HEAD, Rect( x[0], y[0], x[1]-x[0], y[1]-y[0]) +p0);
        assoc_rects.Insert( TOP+BODY, Rect( x[1], y[0], x[2]-x[1], y[1]-y[0]) +p0);
        assoc_rects.Insert( TOP+TAIL, Rect( x[2], y[0], x[3]-x[2], y[1]-y[0]) +p0);

        assoc_rects.Insert( MIDDLE+HEAD, Rect( x[0], y[1], x[1]-x[0], y[2]-y[1]) +p0);
        assoc_rects.Insert( MIDDLE+TAIL, Rect( x[2], y[1], x[3]-x[2], y[2]-y[1]) +p0);

        assoc_rects.Insert( BOTTOM+HEAD, Rect(x[0], y[3], x[1]-x[0], y[4]-y[3]) +p0);
        assoc_rects.Insert( BOTTOM+BODY, Rect(x[1], y[3], x[2]-x[1], y[4]-y[3]) +p0);
        assoc_rects.Insert( BOTTOM+TAIL, Rect(x[2], y[3], x[3]-x[2], y[4]-y[3]) +p0);

        assoc_rects.Insert( HEAD+DIV , Rect( x[0], y[2], x[1]-x[0], y[3]-y[2]) +p0);
        assoc_rects.Insert( BODY+DIV , Rect( x[1], y[2], x[2]-x[1], y[3]-y[2]) +p0);
        assoc_rects.Insert( TAIL+DIV , Rect( x[2], y[2], x[3]-x[2], y[3]-y[2]) +p0);

        if (!noinner) assoc_rects.Insert( MIDDLE+BODY, Rect( x[1], y[1], x[2]-x[1], y[2]-y[1]) +p0);
        Skin::SetDirMask( Pos(0,1) );
    }
    ImageDicer::Dice(assoc_rects);
    _orient = orient;
    SetSize( Dim2i(bound.GetW(), bound.GetH()) );
}


/* Paint components of table

=================================================================================*/


/* Paint backdrop
---------------------------------------------------------------------------------*/
void SizerSkin::PaintBackdrop( const Painter& painter, const Pos &p )
{
    if ( Exists(SOURCE) )
    {
        _point_cache.clear();
        _rect_cache.clear();
        ThinkFrame( _point_cache, _rect_cache, p);

        PointList points;
        RectList  rects;
        ThinkBackdrop( points, rects, p );

        painter.ProjectImages( GetImage(SOURCE), points, rects );
    }
}

/* Paint frame & grid
---------------------------------------------------------------------------------*/
void SizerSkin::PaintGrid( const Painter& painter, const Pos &p )
{
    if ( Exists(SOURCE) )
    {
        if ( _point_cache.empty() || _rect_cache.empty() )
        {
            _point_cache.clear();
            _rect_cache.clear();
            ThinkFrame( _point_cache, _rect_cache, p);
        }
        ThinkGrid( _point_cache, _rect_cache, p);
        painter.ProjectImages( GetImage(SOURCE), _point_cache, _rect_cache );

        _point_cache.clear();
        _rect_cache.clear();
    }
}


/* Generate list of rectangles to cut from source and blit destination positions

=================================================================================*/

/* Generate grid
---------------------------------------------------------------------------------*/
void SizerSkin::ThinkGrid( PointList& points, RectList& rects, const Pos &p )
{
    Tiler tiler( points, rects, p);
    tiler.SetClipRect( GetClipRect() );

    // TOP/LEFT     DIVIDERS
    // BOTTOM/RIGHT DIVIDERS
    // DIVIDERS
    int i, n = _dividers.Size();
    for( i=1; i<n-1; ++i )
    {
        int  begin = (_dividers[i-1] + _dividers[i])/2;
        int  end   = (_dividers[i] + _dividers[i+1])/2;
        int shift  = _dividers[i] - (end+begin-1)/2;

        if (_orient==HORIZONTAL)
        {
            Rect top_bound = Rect( begin, _rLU.GetY1(), end-begin, _rLU.GetH() );
            Rect mid_bound = Rect( begin, _rLU.GetY2(), end-begin, _rLD.GetY1()-_rLU.GetY2());
            Rect btm_bound = Rect( begin, _rLD.GetY1(), end-begin, _rLD.GetH() );
            tiler.Orient(VERTICAL);
            if ( Exists(HEAD+DIV) )
            {
                IGraphics &g = ImageDicer::GetImage(HEAD+DIV);
                tiler.RectCut( g.GetRect(), top_bound, Tiler::HALIGNCENTER|Tiler::VALIGNTOP, shift );
            }
            if ( Exists(BODY+DIV) )
            {
                IGraphics &g = ImageDicer::GetImage(BODY+DIV);
                tiler.RectFill1D( g.GetRect(), mid_bound, Tiler::HALIGNCENTER, shift );
            }
            if ( Exists(TAIL+DIV) )
            {
                IGraphics &g = ImageDicer::GetImage(TAIL+DIV);
                tiler.RectCut( g.GetRect(), btm_bound, Tiler::HALIGNCENTER|Tiler::VALIGNBOTTOM, shift );
            }
        }

        else
        {
            Rect top_bound = Rect( _rLU.GetX1(), begin, _rLU.GetW(), end-begin );
            Rect mid_bound = Rect( _rLU.GetX2(), begin, _rRU.GetX1()-_rLU.GetX2(), end-begin);
            Rect btm_bound = Rect( _rRU.GetX1(), begin, _rRU.GetW(), end-begin );
            tiler.Orient(HORIZONTAL);
            if ( Exists(HEAD+DIV) )
            {
                IGraphics &g = ImageDicer::GetImage(HEAD+DIV);
                tiler.RectCut( g.GetRect(), top_bound, Tiler::VALIGNCENTER|Tiler::HALIGNLEFT, shift );
            }
            if ( Exists(BODY+DIV) )
            {
                IGraphics &g = ImageDicer::GetImage(BODY+DIV);
                tiler.RectFill1D( g.GetRect(), mid_bound, Tiler::VALIGNCENTER, shift );
            }
            if ( Exists(TAIL+DIV) )
            {
                IGraphics &g = ImageDicer::GetImage(TAIL+DIV);
                tiler.RectCut( g.GetRect(), btm_bound, Tiler::VALIGNCENTER|Tiler::HALIGNRIGHT, shift );
            }
        }
    }
}

int SizerSkin::GetDimension( int dimID ) const
{
    switch( dimID )
    {
    case VERTI_DIV: return Exists(DIV+BODY) && _orient==HORIZONTAL ? GetImage(DIV+BODY).GetRect().GetH() : 0;
    case HORIZ_DIV: return Exists(DIV+BODY) && _orient==VERTICAL   ? GetImage(DIV+BODY).GetRect().GetW() : 0;
    }
    return FrameSkin::GetDimension( dimID );
}

SPointer<Skin> SizerSkin::CreateSubSkin( int skinID ) const
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

void SizerSkin::SetHDividers( const SortedVector<int> &h_dividers )
{
    if (_orient==VERTICAL)
    {
        _dividers=h_dividers;
        _dividers.Insert(0);
        _dividers.Insert(_width); 
    }
}

void SizerSkin::SetVDividers( const SortedVector<int> &v_dividers )
{
    if (_orient==HORIZONTAL)
    {
        _dividers=v_dividers;
        _dividers.Insert(0);
        _dividers.Insert(_height); 
    }
}

};//namespace Useless
