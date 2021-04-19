#include "UselessPch.h"

#include "WGLSurface.h"
#include "WGLScreen.h"

#include <cmath>

#include "Useless/ErrorConfig.h"
#define WGLERR(e) APP_ERROR(e,"WGLScreen");

namespace Useless {
    
    HDC G_wglRootHDC = 0;

    WGLSurface::WGLSurface(): _glPbuffer(0), _glContext(0), _devContext(0)
    {
    }

    WGLSurface::WGLSurface( HDC parentDC, Surf::Properties &prop )
        : _parentDC( parentDC ), _devContext(0), _glPbuffer(0), _glContext(0), GLContextSurface( prop )
    {
        if ( prop.texture )
        {
            // Error: GLTextureSurface shall be used instead.
            WGLERR( Error("WGLSurface::ctor(): prop.texture was 1 (hint: GLTextureSurface shall be used instead)."));
        }
        if ( !prop.renderable )
        {
            // Error: GLContextSurface is renderable.
            WGLERR( Error("WGLSurface::ctor(): prop.renderable was 0 (hint: GLContextSurface is renderable, so set prop.renderable to 1)."));
        }

        // Here we initialize all WGLExtensions at ONCE.
        if ( 0 == G_wglRootHDC )
        {
            InitWGLExtensionsHDC( G_wglRootHDC = parentDC );
        }

        // Standard OpenGL window initializaton w/o using WGL_ARB_pixel_format extension.
        if ( !WGLExtensionSupported("WGL_ARB_pixel_format") || prop.primary )
        {
            if ( prop.primary )
            {
                _devContext = parentDC;
                PIXELFORMATDESCRIPTOR pfd;
                ZeroMemory( &pfd, sizeof(pfd));
                pfd.nSize = sizeof(pfd);
                pfd.nVersion = 1;
                pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
                pfd.iPixelType = PFD_TYPE_RGBA;
                pfd.cColorBits = 32;
                pfd.cDepthBits = 32;
                pfd.iLayerType = PFD_MAIN_PLANE;
                int iFormat = ChoosePixelFormat( _devContext, &pfd);
                if ( 0 == iFormat )
                {
                    WGLERR( Error("WGLSurface::ctor(): ChoosePixelFormatARB - Failed."));
                }
                if ( FALSE == SetPixelFormat( _devContext, iFormat, &pfd))
                {
                    WGLERR("WGLSurface::ctor(): SetPixelFormat - Failed.");
                }
            }
            else
            {
                // WGL_ARB_pixel_format must be available in order to create non-primary WGLSurface.
                WGLERR("WGLSurface::ctor(): WGL_ARB_pixel_format is not supported.");
            }
        }
        else // Here we will use WGL_ARB_pixel_format extension
        {
            int attrListInt[256];
            int nAttr = 0;

            // Must support OpenGL.
            attrListInt[ nAttr++ ] = WGL_SUPPORT_OPENGL_ARB;
            attrListInt[ nAttr++ ] = TRUE;
            // Operations should be fully accelerated.
            attrListInt[ nAttr++ ] = WGL_ACCELERATION_ARB;
            attrListInt[ nAttr++ ] = WGL_FULL_ACCELERATION_ARB;
            if ( prop.color )
            {
                // Select color mode.
                attrListInt[ nAttr++ ] = WGL_PIXEL_TYPE_ARB;
                attrListInt[ nAttr++ ] = WGL_TYPE_RGBA_ARB;
                if ( !prop.primary )
                {
                    // Number of bits per pixel.
                    attrListInt[ nAttr++ ] = WGL_COLOR_BITS_ARB;
                    attrListInt[ nAttr++ ] = (prop.alpha ? 32 : 24);
                }
            }
            if ( prop.zbuffer )
            {
                attrListInt[ nAttr++ ] = WGL_DEPTH_BITS_ARB;
                attrListInt[ nAttr++ ] = 16;
            }
            if ( prop.primary )
            {
                // Primary surface means window DC.
                attrListInt[ nAttr++ ] = WGL_DRAW_TO_WINDOW_ARB;
                attrListInt[ nAttr++ ] = TRUE;
                // Window shall have both front and back buffers.
                attrListInt[ nAttr++ ] = WGL_DOUBLE_BUFFER_ARB;
                attrListInt[ nAttr++ ] = TRUE;
            }
            else
            {
                // Surface will use Pbuffer DC.
                attrListInt[ nAttr++ ] = WGL_DRAW_TO_PBUFFER_ARB;
                attrListInt[ nAttr++ ] = TRUE;
                // Pbuffer shall have only one front buffer.
                attrListInt[ nAttr++ ] = WGL_DOUBLE_BUFFER_ARB;
                attrListInt[ nAttr++ ] = FALSE;
            }
            // Guardian Nil.
            attrListInt[ nAttr++ ] = 0;

            int formats[32];
            UINT numFormats = 0;
            if ( FALSE == wglChoosePixelFormatARB ( parentDC, attrListInt, NULL, 32, formats, &numFormats ))
            {
                WGLERR( Error("WGLSurface::ctor(): ChoosePixelFormatARB - Failed."));
            }
            if ( 0 == numFormats )
            {
                WGLERR( Error("WGLSurface::ctor(): ChoosePixelFormatARB - No valid format."));
            }
            if ( prop.primary )
            {
                _devContext = parentDC;
                PIXELFORMATDESCRIPTOR pfd;
                ZeroMemory( &pfd, sizeof(pfd));
                pfd.nSize = sizeof(pfd);
                pfd.nVersion = 1;
                HRESULT formatOK = FALSE;
                for ( int i = 0; (i < numFormats) && (FALSE == formatOK); ++i )
                {
                    formatOK = SetPixelFormat( _devContext, formats[ i ], &pfd);
                }
                if ( FALSE == formatOK )
                {
                    WGLERR("WGLSurface::ctor(): SetPixelFormat - Failed.");
                }
            }
            else
            {
                if ( !WGLExtensionSupported("WGL_ARB_pbuffer") )
                {
                    WGLERR("WGLSurface::ctor(): WGL_ARB_pbuffer is not supported.");
                }

                for ( int i = 0; (i < numFormats) && (NULL == _glPbuffer); ++i )
                {
                    _glPbuffer = wglCreatePbufferARB( parentDC, formats[ i ], prop.width, prop.height, NULL );
                }
                if ( NULL == _glPbuffer )
                {
                    WGLERR( Error("WGLSurface::ctor(): wglCreatePbufferARB - Failed."));
                }

                _devContext = wglGetPbufferDCARB( _glPbuffer );
            }
        }// here ends use of WGL_ARB_pixel_format extension
        
        
        // Create OpenGL Context (WARNING: wglCreateContext may return 0 which is valid opengl context)
        _glContext = wglCreateContext( _devContext );

        // Select Current OpenGL Context
        if ( FALSE == wglMakeCurrent( _devContext, _glContext ))
        {
            WGLERR( Error("WGLSurface::ctor(): wglMakeCurrent - Failed."));
        }

        // Setup clipping rectangle covering whole surface
        SetClipper( Rect( prop.width, prop.height ));
    }

    WGLSurface::~WGLSurface()
    {
        GLFinishGarbage();
        if ( _glContext )
        {
            wglMakeCurrent( NULL, NULL );
            wglDeleteContext( _glContext );
        }
        if ( _glPbuffer )
        {
            wglDestroyPbufferARB( _glPbuffer );
        }
    }

    bool WGLSurface::MakeCurrent()
    {
        if ( _glContext != wglGetCurrentContext() ||
                _devContext != wglGetCurrentDC() )
        {
            if ( FALSE != wglMakeCurrent( _devContext, _glContext ))
            {
                return true;
            }
        }
        return false;
    }

    GLContextSurface * WGLSurface::CreateContext( Surf::Properties &properties ) const
    {
        properties.primary = 0;
        properties.num_surfaces = 1;
        WGLSurface *pSurface = new WGLSurface( _parentDC, properties );
        return pSurface;
    }

    bool WGLSurface::IsValidBlitSource( const Surface &surface ) const
    {
        if ( const WGLSurface *p = dynamic_cast< const WGLSurface *>( &surface ))
        {
            return true;
        }
        else
        {
            return GLContextSurface::IsValidBlitSource( surface );
        }
    }

    void WGLSurface::Blit( int x, int y,
            const Surface &src, const Useless::Rect &src_rect,
            const Surf::BlitFX &fx )
    {
        if ( WGLSurface *srf = dynamic_cast< WGLSurface *>( const_cast< Surface *>( &src )) )
        {
            if ( !WGLExtensionSupported("WGL_ARB_make_current_read") )
            {
                WGLERR("WGLSurface::Blit(): WGL_ARB_make_current_read is not supported (Required for <WGLSurface to WGLSurface> blits).");
            }
            if ( !src_rect )
            {
                return;
            }
            wglMakeContextCurrentARB( _devContext, srf->_devContext, _glContext );
            glReadBuffer( GL_FRONT );
            glDrawBuffer( GL_BACK );
            Rect dstR;
            double stretch_w = 1.0;
            double stretch_h = 1.0;
            double recip_stretch_x = 1.0;
            double recip_stretch_y = 1.0;
            double recip_stretch_w = 1.0;
            double recip_stretch_h = 1.0;
            if ( fx.stretch && ( fx.width != src_rect.GetW() || fx.height != src_rect.GetH() ))
            {
                dstR = Rect(x, y, fx.width, fx.height );
                stretch_w = dstR.GetW() / (double)src_rect.GetW();
                stretch_h = dstR.GetH() / (double)src_rect.GetH();
                recip_stretch_x = 1.0 / stretch_w;
                recip_stretch_w = 1.0 / stretch_w;
                recip_stretch_y = 1.0 / stretch_h;
                recip_stretch_h = 1.0 / stretch_h;
            }
            else
            {
                dstR = Rect( x, y, src_rect.GetW(), src_rect.GetH() );
            }

            if ( !dstR )
            {
                return;
            }
            glPixelZoom( stretch_w, stretch_h );

            /*
            // on NV RasterPos(0,0) is [upper,left]
            glRasterPos2i( x, y + dstR.GetH() );
            // but CopyPixels(0,0,.) is [lower,left]
            glCopyPixels( src_rect.GetX(), src.GetHeight() - (src_rect.GetY() + src_rect.GetH()), src_rect.GetW(), src_rect.GetH(), GL_COLOR );
            // Clip regions are not implemented for <WGLSurface to WGLSurface> blits.
            */

            glEnable( GL_SCISSOR_TEST );

           
            // apply clip region 
            for ( RectList::iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                // clip destination rectangle
                Rect dstRectClipped = (*itRect) & dstR;
                
                if ( !!dstRectClipped )
                {
                    glScissor(
                            dstRectClipped.GetX(),
                            GetHeight() - dstRectClipped.GetY() - dstRectClipped.GetH(),
                            dstRectClipped.GetW(),
                            dstRectClipped.GetH());
            
                    glRasterPos2i( x, y + dstR.GetH() );
                    glCopyPixels( src_rect.GetX(),
                            src.GetHeight() - src_rect.GetY() - src_rect.GetH(),
                            src_rect.GetW(),
                            src_rect.GetH(),
                            GL_COLOR );

                    /*
                    // calculate clipped coords in source coord space
                    Rect srcRectClipped = Rect(
                            std::floor( recip_stretch_x * (dstRectClipped.GetX() - x) ),
                            std::floor( recip_stretch_y * (dstRectClipped.GetY() - y) ),
                            std::floor( recip_stretch_w * dstRectClipped.GetW() ),
                            std::floor( recip_stretch_h * dstRectClipped.GetH() ));

                    // on NV RasterPos(0,0) is [upper,left]
                    glRasterPos2d( dstRectClipped.GetX(), dstRectClipped.GetY() + dstRectClipped.GetH() );

                    // but CopyPixels(0,0,.) is [lower,left]
                    glCopyPixels(
                            srcRectClipped.GetX(),
                            src.GetHeight() - (srcRectClipped.GetY() + srcRectClipped.GetH()),
                            srcRectClipped.GetW(),
                            srcRectClipped.GetH(),
                            GL_COLOR );
                    */
                }
            }
        }
        else
        {
            GLContextSurface::Blit( x, y, src, src_rect, fx );
        }
    }

    void WGLSurface::MultiBlit( const Surface &sur, const PointList &p, const RectList& r,
            const Surf::BlitFX &fx )
    {
        if ( WGLSurface *srf = dynamic_cast< WGLSurface *>( const_cast< Surface *>( &sur )) )
        {
            for ( int i=0, I=p.size(); i<I; ++i )
            {
                Blit( p[i].x, p[i].y, sur, r[i], fx );
            }
            // Clip regions are not implemented for <WGLSurface to WGLSurface> blits.
        }
        else
        {
            GLContextSurface::MultiBlit( sur, p, r, fx );
        }
    }


}; //Useless
