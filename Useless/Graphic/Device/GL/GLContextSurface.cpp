#include "UselessPch.h"

#include "GLContextSurface.h"
#include "GLSurfacePainter.h"
#include "Useless/Graphic/Device/SurfaceExtFun.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"
#include <string>
#include <map>
#include <list>

namespace Useless {

USELESS_API bool G_nVidiaViewport = true;


//
//
// --== GLContextSurface ==--
//
//

GLContextSurface::GLContextSurface()
    : _w(0)
    , _h(0)
    , _constantAlpha(1.0)
    , _enableFlags(0)
{
    BlendFun(0,0,0);
    ConstColor(255,255,255);
}

GLContextSurface::GLContextSurface( Surf::Properties &properties )
{
    _w = properties.width;
    _h = properties.height;
    _primary = properties.primary;
    _numBackbuffers = properties.num_surfaces;
    _transparency = properties.alpha;
    _constantAlpha = 1.0;
    _enableFlags = 0;
    BlendFun(0,0,0);
    ConstColor(255,255,255);

    if ( G_nVidiaViewport )
    {   // nVidia
        if (_w > _h )
        {
            _viewport[0] = 0;
            _viewport[1] = 0.5*(_h - _w);
            _viewport[2] = _w;
            _viewport[3] = _w;
        }
        else
        {
            _viewport[0] = 0.5*(_w - _h);
            _viewport[1] = 0;
            _viewport[2] = _h;
            _viewport[3] = _h;
        }
    }
    else
    {   // Matrox
        _viewport[0] = 0;
        _viewport[1] = 0;
        _viewport[2] = _w;
        _viewport[3] = _h;
    }
    GetProperties( &properties );
}

GLContextSurface::~GLContextSurface()
{
}
    
Surface *
GLContextSurface::CreateSurface( Surf::Properties &properties ) const
{
    if ( properties.renderable )
    {
        return CreateContext( properties );
    }
    else
    {
        return new GLTextureSurface( properties );
    }
}

SPointer< SurfacePainter >
GLContextSurface::CreatePainter()
{
    return new GLSurfacePainter( this );
}

void GLContextSurface::GetViewport ( int *rect )
{
    GLContext::GetViewport( rect );
    rect[1] = _h - rect[1] - rect[3];
}

void GLContextSurface::SetViewport ( int x, int y, int w, int h )
{
    GLContext::SetViewport( x, _h - (h + y), w, h );
}

void GLContextSurface::InitViewport( int x, int y, int w, int h )
{
    GLContext::InitViewport( x, _h - (h + y), w, h );
}

void GLContextSurface::Invoke( const GVM::Message &msg )
{
    /*
    for ( RectList::iterator itRect = _clipRegion.begin();
            itRect != _clipRegion.end(); ++itRect )
    {
        Rect r = (*itRect);
        if ( !!r )
        {
            glScissor( r.GetX(), _h - (r.GetH() + r.GetY()), r.GetW(), r.GetH() );
            GLContext::Invoke( msg );
        }
    }
    */
    GLContext::Invoke( msg );
}


bool GLContextSurface::IsValidBlitSource( const Surface &surface ) const
{
    if ( const GLTextureSurface *p = dynamic_cast< const GLTextureSurface *>( &surface ))
    {
        return (0 != p->_pixelBuffer.GetData());
    }
    return false;
}

void GLContextSurface::Prepare2D()
{
    MakeCurrent();
    glViewport( _viewport[0], _viewport[1], _viewport[2], _viewport[3] );

    glMatrixMode( GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();

    glDisable( GL_SCISSOR_TEST);
    glDisable( GL_ALPHA_TEST );
    glDisable( GL_DEPTH_TEST);
    glDisable( GL_TEXTURE_2D);
    glDisable( GL_BLEND );

    if ( GetFlag( CONST_MATRIX ))
    {
        glLoadMatrixf( _constMatrix );
    }
}

void
GLContextSurface::Blit( int x, int y,
        const Surface &src,
        const Useless::Rect &src_rect,
        const Surf::BlitFX & fx
        )
{
    if ( !src_rect )
    {
        // ignore: blitting empty region.
        return;
    }

    const GLTextureSurface &glSrc = static_cast< const GLTextureSurface & >( src );

    Prepare2D();

    GLImageTesselator &tesselator = GetTesselator( &glSrc );

    glEnable( GL_TEXTURE_2D );

    tesselator.Clip(
            src_rect.GetX1(),
            src_rect.GetY1(),
            src_rect.GetX2(),
            src_rect.GetY2()
            );

    glEnable( GL_SCISSOR_TEST );
    
    float colR = _constColorRed;
    float colG = _constColorGreen;
    float colB = _constColorBlue;

    if ( fx.flood_fill )
    {
        NormalPixel pixCol( fx.flood_color );
        colR *= ((float)pixCol.c.r)/255.0f;
        colG *= ((float)pixCol.c.g)/255.0f;
        colB *= ((float)pixCol.c.b)/255.0f;
    }

    if ( _constantAlpha < 1.0 || glSrc._useAlpha )
    {
        glBlendFunc( _srcBlend, _dstBlend );
        glEnable( GL_BLEND );
        glColor4f( colR, colG, colB, _constantAlpha );
    }
    else if ( glSrc._useColorKey )
    {
        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_GREATER, 0.5 );
        glColor4f( colR, colG, colB, 1.0 );
    }
    else
    {
        glDisable( GL_BLEND );
        glColor3f( colR, colG, colB );
    }

    Pos  dstP;
    Rect dstR;
    
    if ( fx.stretch && ( fx.width != src_rect.GetW() || fx.height != src_rect.GetH() ))
    {
        dstR = Rect(x, y, fx.width, fx.height );
        //int stretchW = (glSrc.GetWidth() * dstR.GetW()) / src_rect.GetW();
        //int stretchH = (glSrc.GetHeight() * dstR.GetH()) / src_rect.GetH();
        double stretch_w = ((double)dstR.GetW()) / (double)src_rect.GetW();
        double stretch_h = ((double)dstR.GetH()) / (double)src_rect.GetH();
        
        GLImageTesselator::RenderTransform renderTransform;
        tesselator.SetRenderTransform( &renderTransform, stretch_w, stretch_h );

        dstP.x = x - ( src_rect.GetX() * dstR.GetW()) / src_rect.GetW();
        dstP.y = y - ( src_rect.GetY() * dstR.GetH()) / src_rect.GetH();
    
        for ( RectList::iterator itRect = _clipRegion.begin();
                itRect != _clipRegion.end(); ++itRect )
        {
            Rect r = (*itRect) & dstR;
            if ( !!r )
            {
                glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                //tesselator->Render( dstP.x, dstP.y, stretchW, stretchH, false );
                glPushMatrix();
                tesselator.UseRenderTransform( &renderTransform, dstP.x, dstP.y );
                tesselator.Render();
                glPopMatrix();
            }
        }
    }
    else
    {
        GLImageTesselator::RenderTransform renderTransform;
        tesselator.SetRenderTransform( &renderTransform, 1.0, 1.0 );

        dstP = Pos(x, y) - src_rect.GetPos();
        dstR = Rect(x, y, src_rect.GetW(), src_rect.GetH() );
        
        for ( RectList::iterator itRect = _clipRegion.begin();
                itRect != _clipRegion.end(); ++itRect )
        {
            Rect r = (*itRect) & dstR;
            if ( !!r )
            {
                glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                //tesselator.Render( dstP.x, dstP.y, false );
                glPushMatrix();
                tesselator.UseRenderTransform( &renderTransform, dstP.x, dstP.y );
                tesselator.Render();
                glPopMatrix();
            }
        }
    }

    ReleaseTesselator( &glSrc, tesselator );
}

void GLContextSurface::MultiBlit( const Surface &src,
        const PointList &points,
        const RectList& rects,
        const Surf::BlitFX &fx )
{
    const GLTextureSurface &glSrc = static_cast< const GLTextureSurface & >( src );

    Prepare2D();

    GLImageTesselator &tesselator = GetTesselator( &glSrc );

    glEnable( GL_TEXTURE_2D );

    float colR = _constColorRed;
    float colG = _constColorGreen;
    float colB = _constColorBlue;

    if ( fx.flood_fill )
    {
        NormalPixel pixCol( fx.flood_color );
        colR *= ((float)pixCol.c.r)/255.0f;
        colG *= ((float)pixCol.c.g)/255.0f;
        colB *= ((float)pixCol.c.b)/255.0f;
    }

    if ( _constantAlpha < 1.0 || glSrc._useAlpha )
    {
        glBlendFunc( _srcBlend, _dstBlend );
        glEnable( GL_BLEND );
        glColor4f( colR, colG, colB, _constantAlpha );
    }
    else if ( glSrc._useColorKey )
    {
        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_GREATER, 0.5 );
        glColor4f( colR, colG, colB, 1.0 );
    }
    else
    {
        glDisable( GL_BLEND );
        glColor3f( colR, colG, colB );
    }
    
    glEnable( GL_SCISSOR_TEST );

    glLoadIdentity();
    GLImageTesselator::RenderTransform renderTransform;
    tesselator.SetRenderTransform( &renderTransform, 1.0, 1.0 );
    
    
    std::multimap< int, int > rectsPerTile;
    for ( int i=0, I=rects.size(); i<I; ++i )
    {
        Rect src_rect = rects[i];

        tesselator.Clip(
                src_rect.GetX1(),
                src_rect.GetY1(),
                src_rect.GetX2(),
                src_rect.GetY2()
                );

        for ( int j=0, J=tesselator._tesselRender.size(); j<J; ++j )
        {
            rectsPerTile.insert( std::pair< int, int >( tesselator._tesselRender[j], i ));
        }
    }

    for ( std::multimap< int, int >::iterator itRectPerTile = rectsPerTile.begin();
            itRectPerTile != rectsPerTile.end(); ++itRectPerTile )
    {
        int tileNo = (*itRectPerTile).first;
        int rectNo = (*itRectPerTile).second;

        int x = points[ rectNo ].x;
        int y = points[ rectNo ].y;
        Rect src_rect = rects[ rectNo ];

        tesselator._tesselRender.resize(1);
        tesselator._tesselRender[0] = tileNo;

        Pos  dstP;
        Rect dstR;
        
        /*if ( fx.stretch && ( fx.width != src_rect.GetW() || fx.height != src_rect.GetH() ))
        {
            dstR = Rect(x, y, fx.width, fx.height );
            int stretchW = (glSrc.GetWidth() * dstR.GetW()) / src_rect.GetW();
            int stretchH = (glSrc.GetHeight() * dstR.GetH()) / src_rect.GetH();
            
            dstP.x = x - ( src_rect.GetX() * dstR.GetW()) / src_rect.GetW();
            dstP.y = y - ( src_rect.GetY() * dstR.GetH()) / src_rect.GetH();
        
            for ( RectList::iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                Rect r = (*itRect) & dstR;
                if ( !!r )
                {
                    glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                    tesselator.Render( dstP.x, dstP.y, stretchW, stretchH, false );
                }
            }
        }
        else*/
        {
            dstP = Pos(x, y) - src_rect.GetPos();
            dstR = Rect(x, y, src_rect.GetW(), src_rect.GetH() );
            
            for ( RectList::iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                Rect r = (*itRect) & dstR;
                if ( !!r )
                {
                    glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                    //tesselator.Render( dstP.x, dstP.y, false );
                    glPushMatrix();
                    tesselator.UseRenderTransform( &renderTransform, dstP.x, dstP.y );
                    tesselator.Render();
                    glPopMatrix();
                }
            }
        }
    }

    ReleaseTesselator( &glSrc, tesselator );
}


void
GLContextSurface::Clear( unsigned long color, const Useless::Rect &drc )
{
    Prepare2D();
    
    NormalPixel pixColor( color );
    float r = pixColor.c.r; r/=255;
    float g = pixColor.c.g; g/=255;
    float b = pixColor.c.b; b/=255;
    float a = pixColor.c.a; a/=255;

    bool useAlpha = ( pixColor.c.a > 0 && pixColor.c.a < 255 ? 1 : 0 );

    if ( !drc && _clipRegion.empty() && !useAlpha )
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor( r, g, b, a);
    }
    else
    { 
        if ( useAlpha )
        {
            glBlendFunc( _srcBlend, _dstBlend );
            glEnable( GL_BLEND );
        }

        Rect xdrc( _w, _h );
        if ( !!drc )
        {
            xdrc = drc;
        }

        // Set standard pixel-based coordinate system, with (0,0) at upper-left screen corrner.
        float viewRect[4];
        glGetFloatv( GL_VIEWPORT, viewRect );
        glScalef( 1.0, -1.0, 1.0 );
        glScalef( 2.0/viewRect[2], 2.0/viewRect[3], 1.0 );
        glTranslatef( -0.5*_w, -0.5*_h, 0.0 );

        for ( RectList::iterator itRect = _clipRegion.begin();
                itRect != _clipRegion.end(); ++itRect )
        {
            Rect rc = xdrc & (*itRect);
            glColor4f( r, g, b, a );
            glRecti( rc.GetX1(), rc.GetY1(), rc.GetX2(), rc.GetY2() );
        }
    }
}


void
GLContextSurface::GetProperties( Surf::Properties *prop ) const
{
    prop->width = _w;
    prop->height = _h;
    prop->pixelformat = ImageFormat::R8G8B8A8;
    prop->color = 1;
    prop->alpha = _transparency;
    prop->zbuffer = 1;
    prop->renderable = 1;
    prop->blit_source = 1;
    prop->prefer_copy = 1;
    prop->video_memory = 1;
    prop->primary = _primary;
    prop->has_color_key = 0;
    prop->num_surfaces = _numBackbuffers;
}

bool
GLContextSurface::SetClipper( const RectList &region )
{
    _clipRegion = region;
    return true;
}


struct IntersectRect
{
    IntersectRect( const Rect &r ): _r(r) {} const Rect &_r;
    Rect operator () ( const Rect &r ) const { return (r & _r); }
};

RectList
GLContextSurface::GetClipper( const Rect &crop ) const
{
    if ( !crop )
    {
        return _clipRegion;
    }
    else
    {
        RectList result;
        std::transform( _clipRegion.begin(), _clipRegion.end(),
                std::back_inserter( result ), IntersectRect( crop ));
        return result;
    }
}

void
GLContextSurface::SetConstantAlpha( int alpha )
{
    _constantAlpha = ((GLfloat)alpha)/255.0;
}

int
GLContextSurface::GetConstantAlpha() const
{
    return (255 * _constantAlpha);
}

bool GLContextSurface::operator == ( const Surface& s )
{
    return (true);
}

bool GLContextSurface::operator != ( const Surface& s )
{
    return !(this->operator==(s));
}

void GLContextSurface::SetSize( int w, int h )
{
    // align size to 2 pixels
    _w = (w / 2 + w % 2) * 2;
    _h = (h / 2 + h % 2) * 2;
    if ( G_nVidiaViewport )
    {   // nVidia
        if (_w > _h )
        {
            _viewport[0] = 0;
            _viewport[1] = 0.5*(_h - _w);
            _viewport[2] = _w;
            _viewport[3] = _w;
        }
        else
        {
            _viewport[0] = 0.5*(_w - _h);
            _viewport[1] = 0;
            _viewport[2] = _h;
            _viewport[3] = _h;
        }
    }
    else
    {   // Matrox
        _viewport[0] = 0;
        _viewport[1] = 0;
        _viewport[2] = _w;
        _viewport[3] = _h;
    }
    SetClipper( Rect( 0, 0, _w, _h ));
}

void GLContextSurface::BlendFun( int sf, int df, bool enabled )
{
    if ( enabled )
    {
        _srcBlend = sf;
        _dstBlend = df;
    }
    else
    {
        _srcBlend = GL_SRC_ALPHA;
        _dstBlend = GL_ONE_MINUS_SRC_ALPHA;
    }
}

void GLContextSurface::ConstColor( int r, int g, int b )
{
    _constColorRed   = ((GLfloat)r)/255.0;
    _constColorGreen = ((GLfloat)g)/255.0;
    _constColorBlue  = ((GLfloat)b)/255.0;
}

void GLContextSurface::ConstMatrix( float *matrix )
{
    if ( matrix )
    {
        std::copy( matrix, matrix + 16, _constMatrix );
        SetFlag( CONST_MATRIX );
    }
    else
    {
        ClrFlag( CONST_MATRIX );
    }
}

void * GLContextSurface::GetExtFun( const std::string &fun )
{
    if ( "BlendFun" == fun )
    {
        return (void *)(& SurfExt::Thunk3< int, int, bool, GLContextSurface, &GLContextSurface::BlendFun >::apply );
    }
    else if ( "ConstColor" == fun )
    {
        return (void *)(& SurfExt::Thunk3< int, int, int, GLContextSurface, &GLContextSurface::ConstColor >::apply );
    }
    else if ( "ConstMatrix" == fun )
    {
        return (void *)(& SurfExt::Thunk1< float *, GLContextSurface, &GLContextSurface::ConstMatrix >::apply );
    }
    else
    {
        assert(!"Unknown SurfaceExtFun");
        return 0;
    }
}

struct GLContextReader : PixelTransfer
{
    GLContextSurface *_surface;
    GLenum            _gl_type;
    int               _destPF;
    void             *_destPtr;
    int               _destPitch;    
    GLContextReader( GLContextSurface *surface ): _surface(surface), _gl_type(GL_RGBA), _destPF(32), _destPtr(0) {}
    virtual ~GLContextReader() {}

    enum Type { SOURCE=1, DESTINATION=2, TRANSFORM=4 };

    //! Obtain transfer type information (combination of flags)
    virtual int  Info() { return SOURCE+TRANSFORM; }

    //! Fetch data from source
    virtual void Fetch( const void *source, const Rect &area ) { assert(0); }

    //! Store data into destination
    virtual void Store( void *destination, const Rect &area )
    {
        _surface->MakeCurrent();
        for ( int i=0; i<area.GetH(); ++i )
        {            
            glReadPixels( area.GetX(), _surface->GetHeight()-area.GetY()-i-1, area.GetW(), 1, _gl_type, GL_UNSIGNED_BYTE, destination );
            (char*&)destination += _destPitch;
        }
        //glReadPixels( area.GetX(), area.GetY(), area.GetW(), area.GetH(), _gl_type, GL_UNSIGNED_BYTE, destination );
    }

    //! Transform data from input buffer to destination buffer
    virtual void Transform()
    {
        if ( _destPtr )
        {
            Store( _destPtr, Rect( _surface->GetWidth(), _surface->GetHeight() ));
        }
    }

    //! Set source pixel-stream type
    virtual void SetSource( int  width,
                            int  height,
                            int  pitch,
                            int  pixel_format, //!< use ImageFormat::Format or BPP
                     const void *palette = 0,
                            int  colors = 0) { assert(0); }
    
    //! Set destination pixel-stream type
    virtual void SetDestination( int  width,
                                 int  height,
                                 int  pitch,
                                 int  pixel_format, //!< use ImageFormat::Format or BPP
                                void *palette = 0,
                                 int  colors = 0)
    {
        switch( pixel_format )
        {
            case ImageFormat::B8G8R8A8:
            case ImageFormat::B8G8R8A0:
                _gl_type = GL_BGRA_EXT;
                break;

            case ImageFormat::R8G8B8A8:
            case ImageFormat::R8G8B8A0:
                _gl_type = GL_RGBA;
                break;

            case ImageFormat::ALPHA8:
            case ImageFormat::ALPHA8PM:
                _gl_type = GL_ALPHA;
                break;

            case ImageFormat::B8G8R8:
                _gl_type = GL_BGR_EXT;
                break;

            case ImageFormat::R8G8B8:
                _gl_type = GL_RGB;
                break;

            case ImageFormat::SINGLE8:
            //case ImageFormat::INDEX8:
                _gl_type = GL_LUMINANCE;
                break;
        };
        _destPF = pixel_format;
        _destPitch = pitch;
    }

    //! Get source pixel-stream type
    virtual void GetSource( int  *width,
                            int  *height,
                            int  *pixel_format,
                     const void **palette,
                            int  *colors )
    {
        *width = _surface->GetWidth();
        *height = _surface->GetHeight();
        *pixel_format = 32;
        *palette = 0;
        *colors = 0;
    }

    //! Get destination pixel-stream type
    virtual void GetDestination( int  *width,
                                 int  *height,
                                 int  *pixel_format,
                                void **palette,
                                 int  *colors )
    {        
        *width = _surface->GetWidth();
        *height = _surface->GetHeight();
        *pixel_format = _destPF;
        *palette = 0;
        *colors = 0;
    }
    
    //! Set source buffer pointer. If needed use only before SetSource().
    virtual void SetSourcePointer( void *source)
    {
        assert(0);
    }

    //! Set destination area pointer. If needed use only before SetDestination().
    virtual void SetDestinaitonPointer( void *destination)
    {
        _destPtr = destination;
    }
};

SPointer< PixelTransfer > GLContextSurface::CreateReader( int channelFormat ) const
{
    return new GLContextReader( const_cast< GLContextSurface *>( this ));
}

SPointer< PixelTransfer > GLContextSurface::CreateWriter( int channelFormat )
{
    assert(0);
    return 0;
}
        
}; //Useless
