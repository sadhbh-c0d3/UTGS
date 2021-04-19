#include "UselessPch.h"

#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/Graphic/Device/Surface.h"
#include "Useless/System/Assert.h"
#include "Useless/Graphic/Planes/ImageBase.h"
#include "Useless/Graphic/Device/GVM_Context.h"


//#cannot include "Threads/Lock.h"
#include <map>

//#define USELESS_ENABLE_BLIT_CACHE

//typedef RW_Mutex Mutex;
//typedef WriteLock Lock;
struct Mutex{};
struct Lock { Lock(...){}};

namespace Useless {

struct BlitCache
{
    const Painter  *_painter;
    const Surface *_source;
    Surface  *_target;
    RectList  _rects;
    PointList _points;
    Surf::BlitFX _fx;

    enum { BUFFER_SIZE = 1024 };

    BlitCache()
        : _rects( BUFFER_SIZE )
        , _points( BUFFER_SIZE )
    {
        Reset();
    }

    void Reset()
    {
        _painter = 0;
        _source = 0;
        _target = 0;
        _rects.resize(0);
        _points.resize(0);
    }

    void Begin( const Painter *painter, const Surface *source, Surface *target )
    {
        if ( painter != _painter || _source != source || _target != target )
        {
            Flush();
        }
        _painter = painter;
        _source = source;
        _target = target;
    }

    void Push( const Rect &rect, const Pos &pos )
    {
        /*if ( _rects.size() == BUFFER_SIZE )
        {
            Flush();
        }*/
        _rects.push_back( rect );
        _points.push_back( pos );
    }

    void Flush()
    {
        if ( _target && _source && !_points.empty() )
        {
            _target->MultiBlit( *_source, _points, _rects, _fx );
        }
        Reset();
    }
};

BlitCache & GetCache()
{
    static BlitCache s_BlitCache;
    return s_BlitCache;
}

static bool s_FlushCache = true;


//////////////////////////////////////////////////////////////////////////////// Painter::Painter //
Painter::Painter(): _plane(0), _constantAlpha(255), _gvmInitialized(0)
{
}

//////////////////////////////////////////////////////////////////////////////// Painter::Painter //
Painter::Painter ( OGraphics &plane, const Pos &offset )
    :_plane(&plane), _offset(offset), _gvmInitialized(0)
{
    assert(_plane);
    _minimal_area = _plane->GetClipper();
    _constantAlpha = _plane->GetConstantAlpha();
    _paintArea = Rect( _plane->GetWidth(), _plane->GetHeight() );
}

//////////////////////////////////////////////////////////////////////////////// Painter::Painter //
Painter::Painter ( const Painter &painter, const Pos &offset )
    :_plane(painter._plane), _offset(painter._offset+offset), _gvmInitialized(0), _paintArea( painter._paintArea )
{
    assert(_plane); 
    _minimal_area = _plane->GetClipper();
    _constantAlpha = _plane->GetConstantAlpha();
}

/////////////////////////////////////////////////////////////////////////////// Painter::~Painter //
Painter::~Painter()
{
    RestoreClipper();
}

////////////////////////////////////////////////////////////////////////////////// Painter::Clear //
void  Painter::Clear ( NormalPixel color ) const
{
    assert(_plane); Flush();//-- check & unlock surface
    _plane->Clear(color.u);
}

///////////////////////////////////////////////////////////////////////// Painter::PaintRectangle //
void  Painter::PaintRectangle ( NormalPixel color, const Rect &r ) const
{
    assert(_plane); Flush();//-- check & unlock surface
    if (!!r)
        _plane->Clear( color.u, r+_offset );
}

/////////////////////////////////////////////////////////////////////////// Painter::ProjectImage //
void  Painter::ProjectImage ( int x, int y, IGraphics &img ) const
{
#ifdef USELESS_ENABLE_BLIT_CACHE
    s_FlushCache = false;
#endif
    assert(_plane); Flush();//-- check & unlock surface

#ifdef USELESS_ENABLE_BLIT_CACHE
    img.Cooperate( *_plane->GetSurface() );
    GetCache().Begin( this, img.GetSurface(), _plane->GetSurface() );
    GetCache().Push( img.GetRect(), Pos(x ,y) + _offset );
    s_FlushCache = true;
#else
    if (! _plane )
    {
        if ( ImageBase *pImg = dynamic_cast< ImageBase *>( &img ))
        {
            pImg->GetSharedSurface().SetTarget(0);
        }
    }
    else
    {
        img.BlitTo( _offset.x+x, _offset.y+y, *_plane );
    }
#endif
}

////////////////////////////////////////////////////////////////////////// Painter::ProjectImages //
void  Painter::ProjectImages ( IGraphics &img, PointList pnts, RectList rects) const
{
    assert(_plane); Flush();//-- check & unlock surface
    assert( pnts.size() == rects.size() );
    
    for ( PointList::iterator it=pnts.begin(); it!=pnts.end(); ++it )
    {
        *it += _offset;
    }

    img.MultiBlitTo( *_plane, pnts, rects );
}

////////////////////////////////////////////////////////////////////////// Painter::PaintBoundry //
SPointer<IGraphics> Painter::GetImage( const Rect &cut ) const
{
    assert(_plane); Flush();//-- check & unlock surface
    return _plane->QueryIGraphics( cut + _offset );
}

////////////////////////////////////////////////////////////////////////// Painter::PaintBoundry //
void  Painter::PaintBoundary( const Rect &r, NormalPixel left, NormalPixel top,
                       NormalPixel right, NormalPixel bottom ) const
{
    assert(_plane); Flush();//-- check & unlock surface
    
    if ( !!r )
    {
        _plane->Clear( top.u,    Rect( r.GetX1(),r.GetY1()  , r.GetW(), 1)+_offset );
        _plane->Clear( bottom.u, Rect( r.GetX1(),r.GetY2()-1, r.GetW(), 1)+_offset );
        _plane->Clear( left.u,   Rect( r.GetX1(),r.GetY1()  , 1, r.GetH())+_offset );
        _plane->Clear( right.u,  Rect( r.GetX2()-1,r.GetY1(), 1, r.GetH())+_offset );
    }
}

////////////////////////////////////////////////////////////////////////////// Painter::Intersect //
bool  Painter::Intersect ( Rect crop_rect )
{
    assert(_plane); Flush();//-- check & unlock surface

    if ( !!crop_rect )
    {
        RectList rects = _plane->GetClipper( crop_rect+_offset );
        _plane->SetClipper(rects);
        return !rects.empty();
    }
    else
    {
        _plane->SetClipper( RectList() );
        return false;
    }
}

///////////////////////////////////////////////////////////////////////// Painter::RestoreClipper //
void  Painter::RestoreClipper()
{
    if (_plane) 
    {
        Flush();//-- unlock surface
        _plane->SetClipper(_minimal_area); 
        _plane->SetConstantAlpha( _constantAlpha );
    }
}



////////////////////////////////////////////////////////////////////// Painter::SetConstantAlpha //
void  Painter::MultiplyAlpha( int alpha )
{
    Flush();
    alpha = ( _plane->GetConstantAlpha() * alpha ) / 255;
    _plane->SetConstantAlpha( alpha );
}



////////////////////////////////////////////////////////////////////// Painter::GetSurfacePainter //

//-- query for SurfacePainter
SurfacePainter & Painter::GetSurfacePainter() const
{
    if ( !_surfacePainter )
    {
        _surfacePainter = _plane->GetSurface()->CreatePainter();
        assert( !!_surfacePainter );
    }
    return *_surfacePainter;
}

//-- Flushes software operations
void  Painter::Flush() const
{
    _surfacePainter.Reset();
    
#ifdef USELESS_ENABLE_BLIT_CACHE
    if ( s_FlushCache )
    {
        GetCache().Flush();
    }
#endif
}

///////////////////////////////////////////////////////////////////////// Common drawing methods //

//-- Painting single Pixel method
void Painter::PutPixel( const Pos &p, NormalPixel color ) const
{
    GetSurfacePainter().PutPixel( p+_offset, color.c );
}

//-- Painting line method
void Painter::PaintLine( const Pos& p1, const Pos& p2, NormalPixel color, int thickness, int algorithm ) const
{
    GetSurfacePainter().PaintLine( p1+_offset, p2+_offset, color.c, thickness, algorithm );
}

//-- Painting poly-line method
void  Painter::PaintLine( const std::vector<Pos> &vertices, NormalPixel color, int thickness, int algorithm ) const
{
    std::vector<Pos> t_vertices;
    for ( int i=0, n= vertices.size(); i<n; ++i )
        t_vertices.push_back( vertices[i] + _offset );
    
    GetSurfacePainter().PaintLine( t_vertices, color.c, thickness, algorithm );
}

void  Painter::PaintPolygon( const std::vector<Pos> &vertices, NormalPixel color ) const
{
    std::vector<Pos> t_vertices;
    for ( int i=0, n= vertices.size(); i<n; ++i )
        t_vertices.push_back( vertices[i] + _offset );
    
    GetSurfacePainter().PaintPolygon( t_vertices, color.c );
}

GVM::Context * Painter::GetGVMContext()
{
    if ( 0 == _gvmInitialized )
    {
        _gvmInitialized = dynamic_cast< GVM::Context *>( _plane->GetSurface());
        _gvmInitialized->InitViewport( _paintArea.GetX(), _paintArea.GetY(), _paintArea.GetW(), _paintArea.GetH() );
    }
    return _gvmInitialized;
}

void Painter::SetFrustum    ( double left, double right, double bottom, double top, double znear, double zfar )
{
    GetGVMContext()->SetFrustum( left, right, bottom, top, znear, zfar );
}

void Painter::SetOrtho      ( double left, double right, double bottom, double top, double znear, double zfar )
{
    GetGVMContext()->SetOrtho( left, right, bottom, top, znear, zfar );
}

void Painter::SetOrtho2D    ( double left, double right, double bottom, double top )
{
    GetGVMContext()->SetOrtho2D( left, right, bottom, top );
}

void Painter::SetPerspective( double fov, double aspect, double znear, double zfar )
{
    GetGVMContext()->SetPerspective( fov, aspect, znear, zfar );
}

void Painter::Invoke        ( const GVM::Message &msg )
{
    // TODO: apply clip region (also to messages)
    GetGVMContext()->Invoke( msg );
}


void * Painter::GetExtFun( const std::string &fun )
{
    return 0;
}

};//namespace Useless
