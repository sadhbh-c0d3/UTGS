#ifndef __INCLUDED_USELESS_TILER_H__
#define __INCLUDED_USELESS_TILER_H__

#include "Useless/GUI/Skins/Skin.h"

namespace Useless {

/*! \ingroup Skins
 *  Includes image tiling mechanism.
 */
class USELESS_API Tiler
{
public:
    Tiler( PointList &positions, RectList &rect_list, const Pos &p ): _rect_list(rect_list), _positions(positions), _pos(p) {}
    void Orient(Orientation orient) { _orient=orient; }

    void SetClipRect( const Rect &clip_rect) { _clip_rect = clip_rect; }
    
    enum Align{ HALIGNLEFT=0, HALIGNCENTER=1, HALIGNRIGHT=2, 
                VALIGNTOP=0<<8, VALIGNCENTER=1<<8, VALIGNBOTTOM=2<<8 };

    Rect    RectCut( const Rect &src, const Rect &area, int align, int orthogonal_shift=0, int parallel_shift=0 );
    Rect RectFill1D( const Rect &src, const Rect &area, int align, int shift=0);
    Rect RectFill2D( const Rect &src, const Rect &area, int shift=0);

private:
    RectList    &_rect_list;
    PointList   &_positions;
    Pos          _pos;
    Orientation  _orient;
    Rect         _clip_rect;

     Pos  P( int x );
     int  X( int x, int y );
     int  X( const Pos &p ) { return X(p.x,p.y); }
     Pos  A( const Rect& r, int align);
     Pos  D( const Pos &p );
     Pos  T( const Pos &p);
};

////////////////////////////////////////////////////////////////

inline Pos  Tiler::P( int x )
{ 
    return (_orient==HORIZONTAL)? Pos(x,0) : Pos(0,x) ; 
}

inline int  Tiler::X( int x, int y ) 
{ 
    return (_orient==HORIZONTAL)? x : y ; 
}

inline Pos  Tiler::D( const Pos &p )
{
    return  (_orient==HORIZONTAL)? Pos(p.x,0) : Pos(0,p.y) ;
}

inline Pos Tiler::T( const Pos &p)
{
    return  Pos(p.y, p.x);
}

inline Pos Tiler::A( const Rect& r, int align )
{
    register int halign = align&0xFF;
    register int valign = align&0xFF00;
    register int xA = (halign==HALIGNLEFT)? r.GetX1() : (halign==HALIGNRIGHT)? r.GetX2()-1 : (r.GetX1()+r.GetX2())/2;
    register int yA = (valign==VALIGNTOP)? r.GetY1() : (valign==VALIGNBOTTOM)? r.GetY2()-1 : (r.GetY1()+r.GetY2())/2;
    return Pos(xA,yA);
}

};//namespace Useless

#endif//__INCLUDED_USELESS_TILER_H__
