#ifndef __INCLUDED__USELESS_UNIDIR_H__
#define __INCLUDED__USELESS_UNIDIR_H__

#include "Useless/Graphic/Point.h"

namespace Useless {

/*! Simple macros to get from 2D point
*/
template< class TNUM >
class UniDir
{
public:
    typedef Useless::Point<TNUM> Point;
    typedef Useless::Point<TNUM> Mask;

    UniDir( const Point &p, const Mask &m ) : _p(p), _m(m) {}
    UniDir( const Point &p, bool is_horizontal=true ) : _p(p) { _m = (is_horizontal)? Mask(1,0):Mask(0,1); }

    Point Extrude ( TNUM length ) const { return GetAdjust( Pos(length,length),_p,_m ); }
    Point OrthoExt( TNUM length ) const { return GetAdjust( Pos(length,length),_p,_m ); }
    TNUM  GetLength()             const { return GetScalar(_p,_m); }
    TNUM  GetWidth ()             const { return GetScalar(_p,_m.Swap()); }

    static Point GetPoint ( const Point &p, const Mask &m ) { return p*m; }
    static TNUM  GetScalar( const Point &p, const Mask &m ) { return (p*m).XSum(); }
    static Point GetOrtho ( const Point &p, const Mask &m ) { return p*(m.Swap()); }
    static Point GetAdjust( const Point &para, const Point &ortho, const Mask &m ) 
    {
        return GetPoint(para,m)+GetOrtho(ortho,m); 
    }

private:
    Point _p;
    Mask  _m;
};

typedef UniDir<int> UniPos;
typedef UniDir<int> UniDim;

};//namespace Useless
#endif//__INCLUDED__USELESS_UNIDIR_H__
