#ifndef __INCLDED__USELESS__SURFACE_PAINTER_H__
#define __INCLDED__USELESS__SURFACE_PAINTER_H__

#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {

/*! SurfacePainter is software implementation of painting onto a Surface
 */
class USELESS_API SurfacePainter
{
public:
    virtual ~SurfacePainter() {}

    //-- Put single pixel
    virtual void  PutPixel       ( const Pos& p, NormalRGB color ) const = 0;

    //-- Paint one line segment from p1 to p2
    virtual void  PaintLine      ( const Pos &p1, const Pos &p2, NormalRGB color,
                                   int thickness, int algorithm = 0 ) const = 0;
    //-- Paint polyline
    virtual void  PaintLine      ( const std::vector<Pos> &vertices, NormalRGB color,
                                   int thickness, int algorithm = 0 ) const = 0;
    //-- Paint polygon
    virtual void  PaintPolygon   ( const std::vector<Pos> &vertices, NormalRGB color ) const = 0;
};

};//namespace Useless
#endif//__INCLDED__USELESS__SURFACE_PAINTER_H__
