#ifndef __INCLUDED_USELESS_GFX_RECT__
#define __INCLUDED_USELESS_GFX_RECT__

/*
**  $Id: Rect.h,v 1.16 2002/09/13 16:22:10 koolas Exp $
**
**  NAME
**      Rect
**
**  PURPOSE
**      Dummy rectangle structure. 
**        
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/System/Assert.h"
#include "Useless/Error/Error.h"
#include "Useless/Graphic/Point.h"


namespace Useless
{
#define RECT_ASSERT { assert(h>=0 && w>=0 && "Rect::Rect(): Width and Height can only be positive values"); }

/*! \ingroup Graphics
 *  Rect is a dummy rectangle structure.
 *
 *   It is defined as follows: Rect = { (x,y): x<[x1,x2) and y<[y1,y2) };
 *  
 *  Defined methods:
 *  \li operators +,- (Pos) - move rectangle
 *  \li operator & (Rect) - return intersection
 *  \li operator &&(Rect) - tell if rectangles intersects
 *  \li operator | (Rect) - return boundry of two rectangles sum
 *  \li operator +,- (Rect) - treats Rect as vector R^4, returns V+U, V-U
 *  \li operator *,/ (Rect) - multiply or divide lexycographically
 *  \li Align (Rect) - align self to Rect using one out of 9 alignments
 */
class USELESS_API Rect
{
public:
    int x, y, w, h;

    Rect() : x(0), y(0), w(0), h(0) {}
    Rect( const Rect &r ) : x(r.x), y(r.y), w(r.w), h(r.h) { RECT_ASSERT; }
    Rect( int ix, int iy, int iw, int ih ) : x(ix), y(iy), w(iw), h(ih) { RECT_ASSERT; }
    Rect( int iw, int ih ) : x(0), y(0), w(iw), h(ih) { RECT_ASSERT; }
    Rect( const Pos &p1, const Pos &p2);

    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetW() const { return w; }
    int GetH() const { return h; }

    int GetX1() const { return x; }
    int GetY1() const { return y; }
    int GetX2() const { return x+w; }
    int GetY2() const { return y+h; }
    Pos GetPos()const { return Pos(x,y); }
    Pos GetDim()const { return Pos(w,h); }
    Pos GetP1() const { return Pos(x,y); }
    Pos GetP2() const { return Pos(x+w,y+h); }

    void Set( int ix, int iy, int iw, int ih ) { x=ix; y=iy; w=iw; h=ih; RECT_ASSERT; }
    void SetXY( int ix, int iy ) { x=ix; y=iy; }
    void SetWH( int iw, int ih ) { w=iw; h=ih; RECT_ASSERT; }
    void SetPos( const Pos & p  ){ SetX(p.x);  SetY(p.y); }
    void SetP1( int xp, int yp ) { SetX1(xp);  SetY1(yp); }
    void SetP2( int xk, int yk ) { SetX2(xk);  SetY2(yk); RECT_ASSERT; }
    void SetP1( const Pos & p  ) { SetX1(p.x); SetY1(p.y);}
    void SetP2( const Pos & p  ) { SetX2(p.x); SetY2(p.y);RECT_ASSERT; }
    void SetSize( const Dim2i &d) { w=d.x; h=d.y; RECT_ASSERT; }

    void SetX( int ix ) { x = ix; }
    void SetY( int iy ) { y = iy; }
    void SetW( int iw ) { w = iw; RECT_ASSERT; }
    void SetH( int ih ) { h = ih; RECT_ASSERT; }

    void SetX1( int x1 ) { w -= x1-x; x = x1; }
    void SetY1( int y1 ) { h -= y1-y; y = y1; }
    void SetX2( int x2 ) { w =  x2-x; RECT_ASSERT; }
    void SetY2( int y2 ) { h =  y2-y; RECT_ASSERT; }

    bool IsInside( int x, int y ) const { return x>=GetX1() && x<GetX2() && y>=GetY1() && y<GetY2(); }
    bool Intersects( const Rect &rc ) const { return (!!rc) && (!!*this) && rc.GetX1()<GetX2() && rc.GetX2()>GetX1() && rc.GetY1()<GetY2() && rc.GetY2()>GetY1(); }
    bool OnEdge(int x, int y, int r = 0) const;

    bool operator! () const { return !w || !h; } //!< check if empty

    friend bool operator== ( const Rect &r1, const Rect &r2 ) { return r1.x==r2.x && r1.y==r2.y && r1.w==r2.w && r1.h==r2.h; }
    friend bool operator!= ( const Rect &r1, const Rect &r2 ) { return !(r1==r2); }
    friend bool operator&& ( const Rect &r1, const Rect &r2 ) { return r1.Intersects(r2); }

    Rect& operator+= ( const Pos &p ) { x+=p.x; y+=p.y; return *this;}
    Rect& operator-= ( const Pos &p ) { x-=p.x; y-=p.y; return *this;}

    friend Rect operator+ ( const Rect &r, const Pos &p ) { return Rect( r.x+p.x, r.y+p.y, r.w, r.h ); }
    friend Rect operator- ( const Rect &r, const Pos &p ) { return Rect( r.x-p.x, r.y-p.y, r.w, r.h ); }
    friend Rect operator+ ( const Pos &p, const Rect &r ) { return Rect( r.x+p.x, r.y+p.y, r.w, r.h ); }
    friend Rect operator- ( const Pos &p, const Rect &r ) { return Rect( r.x-p.x, r.y-p.y, r.w, r.h ); }

    Rect& operator&= ( const Rect &rc );
    Rect& operator|= ( const Rect &rc );   //!< boundary of two-rect-sum
    Rect& operator+= ( const Rect &rc ) { x+=rc.x; y+=rc.y; w+=rc.w; h+=rc.h; return *this; }
    Rect& operator-= ( const Rect &rc ) { x-=rc.x; y-=rc.y; w-=rc.w; h-=rc.h; return *this; }

    friend Rect operator& ( Rect r1, const Rect &r2 ) { return r1&=r2; }
    friend Rect operator| ( Rect r1, const Rect &r2 ) { return r1|=r2; }    //!< boundary of two-rect-sum
    friend Rect operator+ ( Rect r1, const Rect &r2 ) { return r1+=r2; }
    friend Rect operator- ( Rect r1, const Rect &r2 ) { return r1-=r2; }


    template< class TCOF > Rect  operator* (TCOF c) const { Rect r(*this); return r*=c; }
    template< class TCOF > Rect& operator*=(TCOF c)       { x*=c; y*=c; w*=c; h*=c; return *this; }
    template< class TCOF > Rect  operator/ (TCOF c) const { Rect r(*this); return r/=c; }
    template< class TCOF > Rect& operator/=(TCOF c)       { x/=c; y/=c; w/=c; h/=c; return *this; }

    enum Alignment{ HALIGNLEFT=0, HALIGNCENTER=1, HALIGNRIGHT=2, 
                    VALIGNTOP=0<<8, VALIGNCENTER=1<<8, VALIGNBOTTOM=2<<8 };

    Rect& Align( const Rect &to, Alignment align);

};
#undef RECT_ASSERT

#define MIN(a,b) ((a>b)? b:a )
#define MAX(a,b) ((a<b)? b:a )

inline Rect& Rect::operator&=(const Rect &rc)
{
    if ( Intersects(rc) )
    {
        SetX1( MAX( GetX1(),rc.GetX1() ) );
        SetX2( MIN( GetX2(),rc.GetX2() ) );
        SetY1( MAX( GetY1(),rc.GetY1() ) );
        SetY2( MIN( GetY2(),rc.GetY2() ) );
    }
    else
    {
        (*this)=Rect();
    }
    return *this;
}

inline Rect& Rect::operator|=(const Rect &rc)
{
    SetX1( MIN( GetX1(),rc.GetX1() ) );
    SetX2( MAX( GetX2(),rc.GetX2() ) );
    SetY1( MIN( GetY1(),rc.GetY1() ) );
    SetY2( MAX( GetY2(),rc.GetY2() ) );
    return *this;
}

template<class TNUM>
inline Point<TNUM>& Point<TNUM>::Enclose(const Rect& r)
{
    x = ( x<r.GetX1() )? r.GetX1() : ( x<r.GetX2() )? x : r.GetX2()-1;
    y = ( y<r.GetY1() )? r.GetY1() : ( y<r.GetY2() )? y : r.GetY2()-1;
    return *this;
}

inline Rect& Rect::Align( const Rect& r, Alignment align )
{
    register int halign = align&0xFF;
    register int valign = align&0xFF00;
    register int xA = (halign==HALIGNLEFT)? r.GetX1() : (halign==HALIGNRIGHT)? r.GetX2()-1 : (r.GetX1()+r.GetX2())/2;
    register int yA = (valign==VALIGNTOP)? r.GetY1() : (valign==VALIGNBOTTOM)? r.GetY2()-1 : (r.GetY1()+r.GetY2())/2;
    register int xB = (halign==HALIGNLEFT)? GetX1() : (halign==HALIGNRIGHT)?  GetX2()-1 : (GetX1()+GetX2())/2;
    register int yB = (valign==VALIGNTOP)?  GetY1() : (valign==VALIGNBOTTOM)? GetY2()-1 : (GetY1()+GetY2())/2;
    return (*this)+= Pos( xA-xB, yA-yB);
}

inline bool Rect::OnEdge(int ix, int iy, int r) const
{
    return ( ix-r<=GetX1() && ix+r>=GetX1() || ix-r<=GetX2() && ix+r>=GetX2() ) && ( iy+r>=GetY1() && iy-r<=GetY2() ) || 
           ( iy-r<=GetY1() && iy+r>=GetY1() || iy-r<=GetY2() && iy+r>=GetY2() ) && ( ix+r>=GetX1() && ix-r<=GetX2() ) ; 
}

inline Rect::Rect( const Pos &p1, const Pos &p2)
{
    x = p1.x<p2.x ? p1.x:p2.x;
    y = p1.y<p2.y ? p1.y:p2.y;
    
    Pos d = (p1 - p2).Abs();
    w = d.x;
    h = d.y;
}

inline Rect CheckedRect( int ix, int iy, int iw, int ih )
{
    if (iw < 0)
    {
        throw Error("CheckedRect width cannot be negative");
    }
    else if (ih < 0)
    {
        throw Error("CheckedRect height cannot be negative");
    }
    else
    {
        return Rect(ix, iy, iw, ih);
    }
}

#pragma warning(disable:4067)
#undef MIN;
#undef MAX;

};

#endif //__INCLUDED_USELESS_RECT__
