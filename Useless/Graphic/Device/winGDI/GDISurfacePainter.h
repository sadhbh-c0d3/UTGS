#ifndef __INCLUDED_GDI_SURFACE_PAINTER_H__
#define __INCLUDED_GDI_SURFACE_PAINTER_H__

#include "Useless/Graphic/Planes/SurfacePainter.h"

namespace Useless {

    class USELESS_API GDISurfacePainter : public SurfacePainter
    {
    public:
        //-- Put single pixel
        void
        PutPixel        ( const Pos& p, NormalRGB color ) const
        {}

        //-- Paint one line segment from p1 to p2
        void
        PaintLine       ( const Pos &p1, const Pos &p2, NormalRGB color,
                                       int thickness, int algorithm = 0 ) const
        {}
        
        //-- Paint polyline
        void
        PaintLine       ( const std::vector<Pos> &vertices, NormalRGB color,
                                       int thickness, int algorithm = 0 ) const
        {}
        
        //-- Paint polygon
        void
        PaintPolygon    ( const std::vector<Pos> &vertices, NormalRGB color ) const
        {}
    };
    
}; //Useless

#endif//__INCLUDED_GDI_SURFACE_PAINTER_H__
