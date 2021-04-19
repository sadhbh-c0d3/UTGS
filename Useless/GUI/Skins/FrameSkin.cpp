#include "UselessPch.h"

#include "Useless/GUI/Skins/FrameSkin.h"
#include "Useless/GUI/Skins/Tiler.h"

namespace Useless {

FrameSkin::FrameSkin( const RefImage& ref_image, const Rect &inner, bool no_interior )
    : ImageDicer(ref_image)
{
    FrameDice( ref_image.get().GetRect(), inner, no_interior );
}

FrameSkin::FrameSkin(const RefImage &ref_image, const Rect &outer, const Rect &inner, bool no_interior )
    : ImageDicer(ref_image)
{
    FrameDice( outer, inner, no_interior );
}



void FrameSkin::FrameDice( Rect outer, Rect inner, bool no_interior )
{
    if(! outer) { outer = Rect( 0, 0, GetImage(SOURCE).GetWidth(), GetImage(SOURCE).GetHeight() ); }
    if(! inner) { inner = outer; }

    assert( ((inner & (outer-outer.GetPos())) == inner) && "Inner boundary must be inside outer cut rectangle." );
    inner &= outer-outer.GetPos();
    
    // here was a bug,
    // ImageDicer::Dice(assoc_rects) expects rects relative to source image
    Pos p0; // = outer.GetPos();

    int x[] = { 0, inner.GetX1(), inner.GetX2(), outer.GetW() };
    int y[] = { 0, inner.GetY1(), inner.GetY2(), outer.GetH() };
    AssocVector<int ,Rect> assoc_rects;

    assoc_rects.Insert( TOP+HEAD, Rect(x[0],y[0],x[1]-x[0],y[1]-y[0]) +p0); 
    assoc_rects.Insert( TOP+BODY, Rect(x[1],y[0],x[2]-x[1],y[1]-y[0]) +p0);
    assoc_rects.Insert( TOP+TAIL, Rect(x[2],y[0],x[3]-x[2],y[1]-y[0]) +p0);

    assoc_rects.Insert( MIDDLE+HEAD, Rect(x[0],y[1],x[1]-x[0],y[2]-y[1]) +p0);
    assoc_rects.Insert( MIDDLE+TAIL,Rect(x[2],y[1],x[3]-x[2],y[2]-y[1]) +p0);

    assoc_rects.Insert( BOTTOM+HEAD, Rect(x[0],y[2],x[1]-x[0],y[3]-y[2]) +p0);
    assoc_rects.Insert( BOTTOM+BODY, Rect(x[1],y[2],x[2]-x[1],y[3]-y[2]) +p0);
    assoc_rects.Insert( BOTTOM+TAIL, Rect(x[2],y[2],x[3]-x[2],y[3]-y[2]) +p0);

    if(! no_interior) { assoc_rects.Insert( MIDDLE+BODY, Rect(x[1],y[1],x[2]-x[1],y[2]-y[1]) +p0) ; }

    ImageDicer::Dice(assoc_rects);
    _width  = GetImage(SOURCE).GetWidth();
    _height = GetImage(SOURCE).GetHeight();
}

/* Paint components of table

=================================================================================*/


/* Paint default
---------------------------------------------------------------------------------*/
void FrameSkin::Paint( const Painter& painter, const Pos& p )
{
    PaintBackdrop( painter, p);
    PaintFrame( painter, p);
}

/* Paint backdrop
---------------------------------------------------------------------------------*/
void FrameSkin::PaintBackdrop( const Painter& painter, const Pos &p )
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

/* Paint frame
---------------------------------------------------------------------------------*/
void FrameSkin::PaintFrame( const Painter& painter, const Pos &p )
{
    if ( Exists(SOURCE) )
    {
        if ( _point_cache.empty() || _rect_cache.empty() )
        {
            _point_cache.clear();
            _rect_cache.clear();
            ThinkFrame( _point_cache, _rect_cache, p);
        }
        painter.ProjectImages( GetImage(SOURCE), _point_cache, _rect_cache );
        _point_cache.clear();
        _rect_cache.clear();
    }
}



/* Generate list of rectangles to cut from source and blit destination positions

=================================================================================*/


/* Generate frame
---------------------------------------------------------------------------------*/
void  FrameSkin::ThinkFrame( PointList& points, RectList& rects, const Pos &p )
{
    Tiler tiler( points, rects, p);
    _rLU = Rect(0,0, _width/2, _height/2);
    _rRU = Rect( _rLU.GetW(), 0, _width-_rLU.GetW(), _height/2 );
    _rRD = Rect( _rLU.GetW(), _rLU.GetH(), _width-_rLU.GetW(), _height-_rLU.GetH() );
    _rLD = Rect( 0, _rLU.GetH(), _width/2, _height-_rLU.GetH() );

    tiler.SetClipRect( GetClipRect() );

    //Render Corrners
    // LU  
    if ( Exists(TOP+HEAD) )
    {
        IGraphics &g = ImageDicer::GetImage(TOP+HEAD);
        _rLU = tiler.RectCut( g.GetRect(), _rLU, Tiler::HALIGNLEFT|Tiler::VALIGNTOP);
    }
    else
        _rLU = Rect( _rLU.GetX1(), _rLU.GetY1(), 0, 0 );

    // RU
    if ( Exists(TOP+TAIL) )
    {
        IGraphics &g = ImageDicer::GetImage(TOP+TAIL);
        _rRU = tiler.RectCut( g.GetRect(), _rRU, Tiler::HALIGNRIGHT|Tiler::VALIGNTOP);
    }
    else
        _rRU = Rect( _rRU.GetX2(), _rRU.GetY1(), 0, 0 );

    // LD
    if ( Exists(BOTTOM+HEAD) )
    {
        IGraphics &g = ImageDicer::GetImage(BOTTOM+HEAD);
        _rLD = tiler.RectCut( g.GetRect(), _rLD, Tiler::HALIGNLEFT|Tiler::VALIGNBOTTOM);
    }
    else
        _rLD = Rect( _rLD.GetX1(), _rLD.GetY2(), 0, 0 );

    // RD
    if ( Exists(BOTTOM+TAIL) )
    {
        IGraphics &g = ImageDicer::GetImage(BOTTOM+TAIL);
        _rRD = tiler.RectCut( g.GetRect(), _rRD, Tiler::HALIGNRIGHT|Tiler::VALIGNBOTTOM);
    }
    else
        _rRD = Rect( _rRD.GetX2(), _rRD.GetY2(), 0, 0 );

    tiler.Orient(HORIZONTAL);
    // TOP  FRM
    if ( Exists(TOP+BODY) )
    {
        IGraphics &g = ImageDicer::GetImage(TOP+BODY);
        tiler.RectFill1D( g.GetRect(), Rect( _rLU.GetX2(), 0, _rRU.GetX1()-_rLU.GetX2(), _rLU.GetH() ), Tiler::VALIGNTOP);
    }
    // BOTTOM FRM
    if ( Exists(BOTTOM+BODY) )
    {
        IGraphics &g = ImageDicer::GetImage(BOTTOM+BODY);
        tiler.RectFill1D( g.GetRect(), Rect( _rLD.GetX2(), _rLD.GetY1(), _rRD.GetX1()-_rLD.GetX2(), _rLD.GetH() ), Tiler::VALIGNBOTTOM);
    }

    tiler.Orient(VERTICAL);
    // LEFT FRM
    if ( Exists(MIDDLE+HEAD) )
    {
        IGraphics &g = ImageDicer::GetImage(MIDDLE+HEAD);
        tiler.RectFill1D( g.GetRect(), Rect( 0, _rLU.GetY2(), _rLU.GetW(), _rLD.GetY1()-_rLU.GetY2() ), Tiler::HALIGNLEFT);
    }
    // RIGHT  FRM
    if ( Exists(MIDDLE+TAIL) )
    {
        IGraphics &g = ImageDicer::GetImage(MIDDLE+TAIL);
        tiler.RectFill1D( g.GetRect(), Rect( _rRU.GetX1(), _rRU.GetY2(), _rRU.GetW(), _rRD.GetY1()-_rRU.GetY2() ), Tiler::HALIGNRIGHT);
    }

}

/* Generate backdrop
---------------------------------------------------------------------------------*/
void FrameSkin::ThinkBackdrop( PointList& points, RectList& rects, const Pos &p )
{
    Tiler tiler( points, rects, p);
    tiler.SetClipRect( GetClipRect() );

    // INTERRIOR
    if ( Exists(MIDDLE+BODY) )
    {
        IGraphics &g = ImageDicer::GetImage(MIDDLE+BODY);
        int w = ( _rRU.GetX1()-_rLU.GetX2() < _rRD.GetX1()-_rLD.GetX2() )? _rRU.GetX1()-_rLU.GetX2() : _rRD.GetX1()-_rLD.GetX2();
        int h = ( _rLD.GetY1()-_rLU.GetY2() < _rRD.GetY1()-_rRU.GetY2() )? _rLD.GetY1()-_rLU.GetY2() : _rRD.GetY1()-_rRU.GetY2();
        tiler.RectFill2D( g.GetRect(), Rect( _rLU.GetX2(), _rLU.GetY2(), w, h ) );
    }
}

int FrameSkin::GetDimension( int dimID ) const
{
    switch( dimID )
    {
    case LEFT_MARGIN:   return (Exists(HEAD+MIDDLE) ? GetImage(HEAD+MIDDLE).GetRect().GetW() : 0);
    case TOP_MARGIN:    return (Exists(TOP+BODY)    ? GetImage(TOP+BODY).GetRect().GetH()    : 0);
    case RIGHT_MARGIN:  return (Exists(TAIL+MIDDLE) ? GetImage(TAIL+MIDDLE).GetRect().GetW() : 0);
    case BOTTOM_MARGIN: return (Exists(BOTTOM+BODY) ? GetImage(BOTTOM+BODY).GetRect().GetH() : 0);
    default: return 0;
    }
}


};//namespace Useless
