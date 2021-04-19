#ifndef __INCLUDED_USELESS_GFX_POINT__
#define __INCLUDED_USELESS_GFX_POINT__

/*
**  $Id: Point.h,v 1.7 2003/01/20 20:40:58 koolas Exp $
**
**  NAME
**      Point
**
**  PURPOSE
**      Dummy 2d coordinates structure.
**    
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

namespace Useless
{
class Rect;

/* moving this inside Point struct causes INTERNAL COMPILER ERROR on msvc */
template< class TNUM = int > class Point;
struct point_traits_fcn
{
    template<class T> static long check( const Point<T> & ) { return 0; }
    static short check(...) { return 0; }
};

/*! \ingroup Graphics
 *  Point acts like a vector R^2.
 *  \li All vector operatins may be performed ( V+U, V-U, c*V, V/c ).
 *  \li Point has also defined lexycographical methods: U*V, U/V, and comparison
 *  \li Point::operator bool() returns false if Point(0,0)
 *  \li Defined cast from Point<T1> to Point<T2>, f.e. Point<float> to Point<int>
 *  \li There are also methods returnning: |V|, V'*V, V'*U, (x2,x1)
 *  \li Enclose(Rect) - keeps Point inside Rect
 */
template< class TNUM >
class USELESS_API Point
{
public:
    TNUM x,y;
    
    Point() : x(0), y(0) {}
    Point( TNUM ix, TNUM iy) : x(ix), y(iy) {}
    
    bool   operator !()                const { return !x && !y; }
    bool   operator==( const Point &p) const { return p.x==x && p.y==y; }
    bool   operator!=( const Point &p) const { return !(*this==p); }
    bool   operator< ( const Point &p) const { return x<p.x || (x==p.x && y<p.y); }
    bool   operator<=( const Point &p) const { return x<p.x || (x==p.x && y<=p.y);}
    bool   operator> ( const Point &p) const { return !( (*this)<=p);}
    bool   operator>=( const Point &p) const { return !( (*this)<p); }

    template< class T > Point& operator+=( const Point<T> &p) { x+=p.x; y+=p.y; return *this;}
    template< class T > Point& operator-=( const Point<T> &p) { x-=p.x; y-=p.y; return *this;}
    template< class T > Point  operator+ ( const Point<T> &p) const { return Point(x+p.x, y+p.y); }
    template< class T > Point  operator- ( const Point<T> &p) const { return Point(x-p.x, y-p.y); }

    Point  operator- () const { return Point(-x,-y); }

    template< class T > static Point& mul( Point &p, const T &c, short ) { p.x*=c; p.y*=c; return p; }
    template< class T > static Point& mul( Point &p, const T &c, long )  { p.x*=c.x; p.y*=c.y; return p; }

    template< class T > static Point& div( Point &p, const T &c, short ) { p.x/=c; p.y/=c; return p; }
    template< class T > static Point& div( Point &p, const T &c, long )  { p.x/=c.x; p.y/=c.y; return p; }

    template< class T > static Point& mod( Point &p, const T &c, short ) { p.x%=c; p.y%=c; return p; }
    template< class T > static Point& mod( Point &p, const T &c, long )  { p.x%=c.x; p.y%=c.y; return p; }

    template< class T > Point& operator*=( T c ) { return mul(*this, c, point_traits_fcn::check(c) ); }
    template< class T > Point& operator/=( T c ) { return div(*this, c, point_traits_fcn::check(c) ); }
    template< class T > Point& operator%=( T c ) { return mod(*this, c, point_traits_fcn::check(c) ); }

    template< class TCAS > Point& operator= ( const Point<TCAS> &p) { x=p.x; y=p.y; return *this;}
    template< class TCAS > operator Point<TCAS>() { return Point<TCAS>( (TCAS)x, (TCAS)y); }

    template< class TCAS >
    Point( const Point<TCAS> &p ): x((TNUM)(p.x)), y((TNUM)(p.y)) {}

    Point  Swap   ()               const { return Point(y,x); }
    Point  Abs    ()               const { return Point( (x<0)? -x:x , (y<0)? -y:y ); }
    TNUM   Sqr    ()               const { return x*x+y*y; }
    TNUM   Sqr    (const Point& p) const { return (x+p.x)*(x+p.x)+(y+p.y)*(y+p.y); }
    TNUM   Sum    ()               const { return x+y; }
    TNUM   XSum   ()               const { assert(!x || !y); return x+y; } //<! Exclusive sum - chooses x or y
    Point& Enclose(const Rect& r);
};

template< class P, class T > Point<P> operator*( Point<P> p, const T &c ) { return p*=c; }
template< class P, class T > Point<P> operator/( Point<P> p, const T &c ) { return p/=c; }
template< class P, class T > Point<P> operator%( Point<P> p, const T &c ) { return p%=c; }


typedef Point<int> Pos;
typedef Point<int> Dim2i;


} // namespace Useless


#endif //__INCLUDED_USELESS_GFX_POINT__ 
