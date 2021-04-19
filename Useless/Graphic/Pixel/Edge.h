#ifndef __INCLUDED__USELESS__EDGE_H__
#define __INCLUDED__USELESS__EDGE_H__

#ifdef WIN32
#   define USELESS_SPECIALIZATION template<>
#else
#   define USELESS_SPECIALIZATION
#endif

#include "Useless/Graphic/Pixel/Pixel.h"

#include <vector>
#include <algorithm>

namespace Useless {

/* An edge is determined by an equation:  x = a*y + b, limited from Y0 to Y1
------------------------------------------------------------------------------*/
struct Edge
{
    double Y0;
    double Y1;
    double a;  // (X1-X0) / (Y1-Y0);
    double b;  // X0 - a*Y0;

    typedef Useless::Point<int> PointType;

    Edge() {}
    Edge( const PointType &p0, const PointType &p1)
    {
        Y0 = (p1.y > p0.y)? p0.y : p1.y;
        Y1 = (p1.y > p0.y)? p1.y : p0.y;
        double X0 = (p1.y > p0.y)? p0.x : p1.x;
        double X1 = (p1.y > p0.y)? p1.x : p0.x;
        a = (Y1-Y0) ? (X1-X0)/(Y1-Y0) : 0;
        b = X0 - a*Y0;
    }
    double Intersect( double y ) const
    {
        return a*y + b;
    }
    bool IsHorizontal() const { return (int)(Y1-Y0)==0; }
};

/* Optimized for step-by-step edge calculation and antialiasing
-----------------------------------------------------------------------------*/
struct EdgeIterator
{
    EdgeIterator(): X(0), Y(0) {}
    EdgeIterator( const Edge &e ): edge(e), Y(e.Y0-1), X(e.Intersect(e.Y0-1) ) {}

    void Next() 
    {
        ++Y; X+= edge.a; 
    }

    int  LeftBlend( double Ymax )  const 
    { 
             if ( edge.a < -1 && edge.Y1 > Y && Ymax > Y ) { return  (edge.a); } 
        else if ( edge.a >  1 && edge.Y0 < Y )             { return -(edge.a); }
        else    { return 0; }
    }
    
    int  RightBlend( double Ymax ) const 
    { 
             if ( edge.a >  1 && edge.Y1 > Y && Ymax > Y ) { return  (edge.a); } 
        else if ( edge.a < -1 && edge.Y0 < Y )             { return -(edge.a); }
        else    { return 0; }
    }

    int GetInteger() const
    {
        return (X*256.0);
    }

    bool InRange( int Y ) const { return edge.Y0<=Y && edge.Y1>Y; }
    bool LastPt ( int Y ) const { return Y==(int)(edge.Y1+0.5); }

    EdgeIterator& operator++() { Next(); return *this; }

    Edge edge;
    double X;
    long   Y;
};

/* CrossPoint is a place where edge crosses horizontal line at some y-value
 * Optimized for antialiasing
---------------------------------------------------------------------------*/
struct CrossPoint
{
    CrossPoint(): X(0), BlendL(0), BlendR(0) {}
    CrossPoint( int x, int blendL, int blendR ): X(x), BlendL(blendL), BlendR(blendR) {}

    bool operator < ( const CrossPoint &cp) const { return X < cp.X || X==cp.X && !BlendR; }
    bool operator ==( const CrossPoint &cp) const { return X == cp.X; }

    int X, BlendL, BlendR;
};

/*! Creates vector of Edge's conecting given vertices {v0, v1, v2, ..., v0}
---------------------------------------------------------------------------*/
struct EdgesFromVertices
{
    typedef Edge::PointType PointType;

    template<class T>
    EdgesFromVertices( std::vector<T> vertices )
    {
        std::copy( vertices.begin(), vertices.end(), std::back_inserter(Vertices) );
    }

    USELESS_SPECIALIZATION
    EdgesFromVertices( std::vector< PointType > vertices ): Vertices(vertices) {}

    template < class TS > void GetEdges( TS &edges, int &Ymin, int &Ymax ) const
    {
        assert( !Vertices.empty() );
        Ymin = Vertices.at(0).y;
        Ymax = Vertices.at(0).y;
        for ( int i=0, n=Vertices.size(); i!=n ; ++i)
        {
            Edge e( Vertices.at(i), Vertices.at(i<n-1 ? i+1: 0) );
            if ( !e.IsHorizontal() ) 
            {  
                edges.push_back(e);
            }
            Ymax = Ymax > Vertices.at(i).y ? Ymax : Vertices.at(i).y;
            Ymin = Ymin < Vertices.at(i).y ? Ymin : Vertices.at(i).y;
        }
    }

    std::vector< PointType > Vertices;
};

#undef USELESS_SPECIALIZATION

};//namespace Useless
#endif//__INCLUDED__USELESS__EDGE_H__
