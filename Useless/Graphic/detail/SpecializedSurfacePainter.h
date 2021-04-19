#ifndef __INCLUDED__USELESS_SPECIALIZED_SURFACE_PAINTER_H__
#define __INCLUDED__USELESS_SPECIALIZED_SURFACE_PAINTER_H__

#include "Useless/Graphic/Planes/SurfacePainter.h"
#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/Pixel/DrawLine.h"
#include "Useless/Graphic/Pixel/ThinLine.h"
#include "Useless/Graphic/Pixel/ThickLine.h"
#include "Useless/Graphic/Pixel/PixelPtr.h"
#include "Useless/Util/SafePtr.h"

#include "Useless/ErrorConfig.h"

namespace Useless {

//-- Template specialized for surface with given PixelType
template< class TPixel >
class SpecializedSurfacePainter : public SurfacePainter
{
public:
    typedef TPixel PixelType;

    SpecializedSurfacePainter( LockableSurface *surface, void *ptr, int pitch, const RectList &region )
        : _surface(surface), _ptr(ptr), _pitch(pitch), _region(region) { assert(_surface); }

    virtual ~SpecializedSurfacePainter()
    {
        assert(_surface); _surface->Unlock( Surface::COLOR);
    }

    //-- Put single pixel
	virtual void  PutPixel       ( const Pos& p, NormalRGB color ) const;

    //-- Paint one line segment from p1 to p2
	virtual void  PaintLine      ( const Pos &p1, const Pos &p2, NormalRGB color,
                                   int thickness, int algorithm = 0 ) const;

    //-- Paint polyline
    virtual void  PaintLine      ( const std::vector<Pos> &vertices, NormalRGB color,
                                   int thickness, int algorithm ) const;

    //-- Paint polygon
    virtual void  PaintPolygon   ( const std::vector<Pos> &vertices, NormalRGB color ) const;

private:
    LockableSurface  *_surface;
    void        *_ptr;
    int          _pitch;
    RectList     _region;

//------------------------------------------------------------------------------------
    template< class FObj >
    void PaintInRegion( const FObj &fobj ) const
    {
        for ( int i=0, n=_region.size(); i<n; ++i )
        {
            fobj.apply( PixelType(), _ptr, _pitch, _region[i] );
        }
    }

    //-- Helper functor-struct: Paint a Pixel
    struct __PaintPixel
    {
        __PaintPixel( const Pos &p, NormalRGB color ): _p(p), _color(color){}

        template< class PixelType >
        void apply( const PixelType &pixel, void *ptr, int pitch, const Rect &clip ) const
        {
            if ( clip.IsInside( _p.x, _p.y ))
            {
                *(GenericPixelPtr<PixelType>( ptr, pitch ) + _p) = _color;
            }
        }
    
        Pos       _p;
        NormalRGB _color;
    };

    //-- Helper functor-struct: Paint a Line
    struct __PaintLine
    {
        __PaintLine( const Pos &p1, const Pos &p2, int thickness, NormalRGB color, int algo)
        : _p1(p1), _p2(p2), _color(color), _thickness(thickness), _algorithm(algo) {}

        template< class PixelType >
        void apply( const PixelType &pixel, void *ptr, int pitch, const Rect &clip ) const
        {
            Types::UBYTE *u_ptr = (Types::UBYTE*)ptr;
            ClippedPixelPtr<PixelType> pix_ptr(ptr,pitch,clip);
            switch (_algorithm)
            {
            case 0: Useless::DrawLine( u_ptr, pitch, PixelType(_color), _thickness, _p1, _p2, clip ); break;
            case 1: Useless::DrawLine( pix_ptr, _color, _p1, _p2 ); break;
            case 2: Useless::DrawLine( pix_ptr, _thickness, _color, _p1, _p2 );  break;
            }
        }
    
        Pos       _p1;
        Pos       _p2;
        NormalRGB _color;
        int       _thickness;
        int       _algorithm;
    };

    //-- Helper functor-struct: Paint a PolyLine
    struct __PaintPolyLine
    {
        __PaintPolyLine( const std::vector<Pos> &vertices, int thickness, NormalRGB color, int algo )
        : _vertices(vertices), _color(color), _thickness(thickness), _algorithm(algo) {}

        template< class PixelType >
        void apply( const PixelType &pixel, void *ptr, int pitch, const Rect &clip ) const
        {
            Types::UBYTE *u_ptr = (Types::UBYTE*)ptr;
            ClippedPixelPtr<PixelType> pix_ptr(ptr,pitch,clip);
            int i=0, n=_vertices.size();
            switch (_algorithm)
            {
            case 0:
                for( ;i<n-1; ++i )
                Useless::DrawLine( u_ptr, pitch, PixelType(_color), _thickness, _vertices[i], _vertices[i+1], clip );
                break;

            case 1:
                for( ;i<n-1; ++i )
                Useless::DrawLine( pix_ptr, _color, _vertices[i], _vertices[i+1] );
                break;

            case 2:
                for( ;i<n-1; ++i )
                Useless::DrawLine( pix_ptr, _thickness, _color, _vertices[i], _vertices[i+1] );
                break;

            case 3:
                Useless::DrawLine( pix_ptr, _thickness, _color, _vertices );
                break;
            }
        }
    
        const std::vector<Pos> &_vertices;//< It's temporary class (internal) so it's ok
        NormalRGB _color;
        int       _thickness;
        int       _algorithm;
    };


    //-- Helper functor-struct: Paint a Polygon
    struct __PaintPolygon
    {
        __PaintPolygon( const std::vector<Edge::PointType> &vertices, NormalRGB color )
        : _vertices(vertices), _color(color) {}

        template< class PixelType >
        void apply( const PixelType &pixel, void *ptr, int pitch, const Rect &clip ) const
        {
            Types::UBYTE *u_ptr = (Types::UBYTE*)ptr;
            ClippedPixelPtr<PixelType> pix_ptr(ptr,pitch,clip);
            PaintPoly( pix_ptr, _color, _vertices );
        }
    
        const std::vector<Edge::PointType> &_vertices;//< It's temporary class (internal) so it's ok
        NormalRGB _color;
    };
};
//------------------------------------------------------------------------------------

//-- Painting single Pixel method
template< class PixelType >
void SpecializedSurfacePainter<PixelType>::PutPixel( const Pos &p, NormalRGB color ) const
{
    __PaintPixel paint_pixel( p, color );
    PaintInRegion( paint_pixel );
}

//-- Painting line method
template< class PixelType >
void SpecializedSurfacePainter<PixelType>::PaintLine( const Pos& p1, const Pos& p2,
                                           NormalRGB color, int thickness, int algorithm ) const
{
    __PaintLine paint_line(p1, p2, thickness, color, algorithm );
    PaintInRegion( paint_line );
}

//-- Painting poly-line method
template< class PixelType >
void SpecializedSurfacePainter<PixelType>::PaintLine( const std::vector<Pos> &vertices,
                                           NormalRGB color, int thickness, int algorithm ) const
{
    __PaintPolyLine paint_line( vertices, thickness, color, algorithm );
    PaintInRegion( paint_line );
}

//-- Painting poly-line method
template< class PixelType >
void SpecializedSurfacePainter<PixelType>::PaintPolygon( const std::vector<Pos> &vertices,
                                            NormalRGB color ) const
{
    if ( vertices.size() >2 )
    {
        std::vector<Edge::PointType> d_vertices( vertices.size() );
        std::copy( vertices.begin(), vertices.end(), d_vertices.begin() );
        __PaintPolygon paint_polygon( d_vertices, color );
        PaintInRegion( paint_polygon );
    }
}

};//namespace Useless

namespace Useless {
//------------------------------------------------------------------------------------
//-- Creates SurfacePainter which will fit to given Surface properties
SafePtr< SurfacePainter > USELESS_API CreateSurfacePainter( LockableSurface *p_surface );

};//namespace Useless
#endif//__INCLUDED__USELESS_SPECIALIZED_SURFACE_PAINTER_H__
