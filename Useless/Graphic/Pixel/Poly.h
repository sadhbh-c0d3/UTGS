#ifndef __INCLUDED__USELESS__POLY_H__
#define __INCLUDED__USELESS__POLY_H__

#include <vector>
#include <set>
#include "Useless/Graphic/Point.h"
#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/Pixel/PixelPtr.h"
#include "Useless/Graphic/Pixel/Edge.h"

namespace Useless {

struct EdgeContainer
{
    EdgeContainer( const std::vector<Edge> &e )
    {
        const Edge *p = &*e.begin(), *q = &*e.end();

        for ( ; p!=q; ++p )
        {
            _b.push_back(p);
            _e.push_back(p);
        }
        std::sort( _b.begin(), _b.end(), less_Y0() );
        std::sort( _e.begin(), _e.end(), less_Y1() );
    }

    struct less_Y0
    {
        bool operator ()( const Edge *e1, const Edge *e2 ) const
        {
            return e1->Y0 < e2->Y0;
        }
    };

    struct less_Y1
    {
        bool operator ()( const Edge *e1, const Edge *e2 ) const
        {
            return e1->Y1 < e2->Y1;
        }
    };

    std::vector< const Edge* > _b, _e;
};

struct XPt
{
    XPt() {}
    XPt( double x, double dx ): x(x), dx(dx) {}
    XPt( const Edge &e, double y ): x(e.Intersect(y)), dx(e.a)
    {}

    bool operator < ( const XPt &p ) const
    {
        return (int)x < (int)p.x || (int)x == (int)p.x && dx < p.dx;
    }

    XPt& operator ++() { x+=dx; return *this; }
    operator double () const { return x; }

    double Lt() const { return ((dx>0)? x-dx: x); }
    double Rt() const { return ((dx>0)? x: x-dx); }

    double x;
    double dx;
};

struct less_second
{
    template< class T >
    bool operator ()( const T &a, const T &b ) const
    {
        return a.second < b.second;
    }
};


template< class PixelPtr >
inline void RasterizeLine( PixelPtr p, Useless::NormalRGB cRGB, double x0, double x1, double x2, double x3 )
{
    const double BLEND_MAX = 255*1024;
    int b_inc = BLEND_MAX/(x1 - x0 + 1);
    int b_dec = BLEND_MAX/(x3 - x2 + 1);
    
    double d0 = x0 - int(x0);
    double d1 = x2 - int(x2);

    int blend_start = -d0 * b_inc;
    int blend_end = BLEND_MAX + d1 * b_dec;
    
    int blend = blend_start;
    int x = x0;  p += x;

    if ( cRGB.a==255 )
    {
        for ( ; blend<BLEND_MAX && x<x1; ++x, ++p )
        {
             int b = Saturate( (blend+=b_inc)/1024 );
             p.Write( cRGB.Blend( b, p.Read().Get().c ) );
        }

        for ( ; x<=x2-1; ++x, ++p )
        {
            p.Write( cRGB );
        }

        for ( blend=blend_end; blend>0; ++x, ++p )
        {
             int b = Saturate( (blend-=b_dec)/1024 );
             p.Write( cRGB.Blend( b, p.Read().Get().c ) );
        }
    }
    else if ( cRGB.a==0 )
    {}
    else if ( cRGB.a==127 )
    {
        for ( ; blend<BLEND_MAX && x<x1; ++x, ++p )
        {
             int b = Saturate( (blend+=b_inc)/1024 );
             p.Write( cRGB.Blend( b/2, p.Read().Get().c ) );
        }

        for ( ; x<=x2-1; ++x, ++p )
        {
            p.Write( cRGB.Blend( 127, p.Read().Get().c ) );
        }

        for ( blend=blend_end; blend>0; ++x, ++p )
        {
             int b = Saturate( (blend-=b_dec)/1024 );
             p.Write( cRGB.Blend( b/2, p.Read().Get().c ) );
        }
    }
    else
    {
        for ( ; blend<BLEND_MAX && x<x1; ++x, ++p )
        {
             int b = Saturate( (blend+=b_inc)/1024 );
             p.Write( cRGB.Blend( b*cRGB.a, p.Read().Get().c ) );
        }

        for ( ; x<=x2-1; ++x, ++p )
        {
            p.Write( cRGB.Blend( cRGB.a, p.Read().Get().c ) );
        }

        for ( blend=blend_end; blend>0; ++x, ++p )
        {
             int b = Saturate( (blend-=b_dec)/1024 );
             p.Write( cRGB.Blend( b*cRGB.a, p.Read().Get().c ) );
        }
    }
}


template< class PixelPtr >
inline void PaintPoly( PixelPtr ppix, NormalRGB cRGB, const std::vector< Edge::PointType > &v )
{
    std::vector< Edge > edges;
    int Ymin, Ymax;
    EdgesFromVertices edge_c(v);
    edge_c.GetEdges( edges, Ymin, Ymax );

    EdgeContainer edge_d(edges);
    typedef std::pair< XPt, const Edge* > XpEdge; // cross eval. per edge
    typedef std::vector< XpEdge > XpLine; // line cross sections
    XpLine line_x;

    const Edge **bi = &*edge_d._b.begin(), **bi_e = &*edge_d._b.end();
    const Edge **ei = &*edge_d._e.begin(), **ei_e = &*edge_d._e.end();

    ppix += Pos(0,Ymin);

    for ( int Y=Ymin; Y<=Ymax; ++Y, ppix.Next() )
    {
        while ( ei != ei_e  &&  Y >= (int)(*ei)->Y1 )
        {
            XpLine::iterator i = line_x.begin(), j = line_x.end();
            for ( ; i!=j && (*ei)!=(*i).second; ++i ) {}
            line_x.erase(i);
            ++ei;
        }

        if ( bi != bi_e  &&  Y >= (int)(*bi)->Y0 )
        {
            do {
                line_x.push_back( XpEdge( XPt(**bi, Y),*bi) );
                ++bi;
            } while ( bi != bi_e  &&  Y >= (int)(*bi)->Y0 );
            std::sort( line_x.begin(), line_x.end() );
        }

        XpLine::iterator i = line_x.begin(), j = line_x.end(), k;
        for ( ;i!=j; ++i )
        {
            ++((*i).first); k=i;
            ++i; ++((*i).first);
            if ( i==j ) { break; }

            RasterizeLine( ppix, cRGB,
                (*k).first.Lt(),
                (*k).first.Rt(),
                (*i).first.Lt(),
                (*i).first.Rt() );
        }
    }
}

};//namespace Useless
#endif//__INCLUDED__USELESS__POLY_H__
