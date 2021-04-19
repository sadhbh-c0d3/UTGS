#ifndef __INCLUDED__USELESS__THICK_LINE_H__
#define __INCLUDED__USELESS__THICK_LINE_H__

#include "Useless/Graphic/Pixel/Poly.h"
#include "Useless/Graphic/Pixel/ThinLine.h"
#include <math.h>

namespace Useless {

/* Draw Thick line
----------------------------------------------------*/
template< class PixelPtr >
void DrawLine( PixelPtr pix_ptr,
                int line_width, NormalRGB cRGB, Pos pos1, Pos pos2 
              )
//----------------------------------------------------
{
    if (line_width>0)
    {
        Point<float> p = (pos2 - pos1).Swap(); p.x=-p.x;
        float l = p.Sqr();
        p.x*=abs(p.x); p.y*=abs(p.y);
        p*=(float)(((float)line_width)/2.0);
        p/=l;

        Point<float> p1 = pos1;
        Point<float> p2 = pos2;

        std::vector< Edge::PointType > vcs;
        vcs.push_back(Edge::PointType(p1 - p));
        vcs.push_back(Edge::PointType(p2 - p));

        p.x += (p.x>0)? 0.5 : -0.5;
        p.y += (p.y>0)? 0.5 : -0.5;
        vcs.push_back(Edge::PointType(p2 + p));
        vcs.push_back(Edge::PointType(p1 + p));
    
        PaintPoly( pix_ptr, cRGB, vcs );
    }
    else
    {
        DrawLine( pix_ptr, cRGB, pos1, pos2 );
    }
}


/* Draw Thick poly-line
----------------------------------------------------*/
template< class PixelPtr >
void DrawLine( PixelPtr pix_ptr,
                int line_width, NormalRGB cRGB, std::vector<Pos> vertices
              )
//----------------------------------------------------
{
    if ( line_width>0 )
    {
        std::vector< Edge::PointType > vcs;
        std::vector< Edge::PointType > vcs_e;

        for (int i=1, n=vertices.size(); i<n; ++i )
        {
            Point<float> p1 = vertices[i-1];
            Point<float> p2 = vertices[i];
            if (p1==p2) { continue; }

            Point<float> p = (p2 - p1).Swap(); p.x=-p.x;
            float l = p.Sqr();
            p.x*=abs(p.x); p.y*=abs(p.y);
            p*=(float)(((float)line_width)/2.0);
            p/=l;

            if ( vcs.empty() ) { vcs.push_back(Edge::PointType(p1 - p)); }
            *vcs.rbegin() = (*vcs.rbegin() + (p1 - p))/2.0f;
            vcs.push_back(Edge::PointType(p2 - p));

            p.x += (p.x>0)? 0.5 : -0.5;
            p.y += (p.y>0)? 0.5 : -0.5;

            if ( vcs_e.empty() ) { vcs_e.push_back(Edge::PointType(p1 + p)); }
            *vcs_e.rbegin() = (*vcs_e.rbegin() + (p1 + p))/2.0f;
            vcs_e.push_back(Edge::PointType(p2 + p));
        }
        std::copy( vcs_e.rbegin(), vcs_e.rend(), std::back_inserter(vcs) );
        assert( vcs.size()%2 == 0 );

        if ( !vcs.empty() ) 
        {
            PaintPoly( pix_ptr, cRGB, vcs ); 
        }
    }
    else
    {
        for (int i=1, n=vertices.size(); i<n; ++i )
        {
            DrawLine( pix_ptr, cRGB, vertices[i-1], vertices[i] );
        }
    }
}


};//namespace Useless
#endif//__INCLUDED__USELESS__THICK_LINE_H__
