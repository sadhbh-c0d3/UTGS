#include "UselessPch.h"

#include "Useless/Graphic/Device/GL/GLImageTesselator.h"
#include "Useless/Graphic/Device/GL/GLResourcePool.h"
#include "Useless/Graphic/detail/PixelBuffer.h"
#include "Useless/Graphic/Linear.h"
#include <assert.h>

namespace Useless {

    // REMOVED: Reserve border pixels to avoid bad filtering (Radeon W/A). The border is: (1+1) columns and (1+1) rows.
    // ADDED: G_GLTesselator{*}TexelOffset added
    USELESS_API double G_GLTesselatorStartingTexelOffset = 0.05;
    USELESS_API double G_GLTesselatorEndingTexelOffset = 0.05;

    GLImageTesselator::GLImageTesselator( const PixelBuffer &pix_buf )
    {
        _flipX          = 0;
        _flipY          = 0;
        _w              = pix_buf.GetWidth();
        _h              = pix_buf.GetHeight();
        int bytes_pp    = pix_buf.GetBytesPP();
        int pitch       = pix_buf.GetPitch();
        int row_length  = pix_buf.GetRowLength();
        int fmt         = pix_buf.GetFormat();
        const void *data= pix_buf.GetData();

        int rx = _w;
        int ry = _h;

        // Selecting texture size for every image tile
        // 64x64 textures are much faster on nVidia... if rendering without scissors
        // When we use scissors, best sollution is 256x256 (bigger textures won't do ant better)
        GLint max_tex;
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &max_tex);

        while ( rx>0 )
        {
            int w=
                (rx<=16 || max_tex==16)? 16 :
                (rx<=32 || max_tex==32)? 32 :
                (rx<=64 || max_tex==64)? 64 : 128;

            _tesselColWidths.push_back( w );
            rx -= w;
        }
        while ( ry>0 )
        {
            int h=
                (ry<=16 || max_tex==16)? 16 :
                (ry<=32 || max_tex==32)? 32 :
                (ry<=64 || max_tex==64)? 64 : 128;

            _tesselRowHeights.push_back( h );
            ry -= h;
        }

        assert(!(pitch%bytes_pp)); // We assume: pitch = n*bytes_pp

        int full_w=0; // sum of all texture quads
        int full_h=0;

        {   // Create MxN quad textures
            GLint wrap_s = GLTexture::_Wrap_s; // Record old parameter values
            GLint wrap_t = GLTexture::_Wrap_t;

            GLTexture::_Wrap_s = GL_CLAMP;     // and set u,v clamp mode
            GLTexture::_Wrap_t = GL_CLAMP;

            int x,y;
            std::vector< GLuint >::iterator itDx, itDy;
            char *ptr_y= static_cast< char *>( const_cast< void *>( data ));

            // move back one line, and one pixel
            ptr_y -= pitch + bytes_pp;

            for ( y = 0, itDy = _tesselRowHeights.begin(); itDy != _tesselRowHeights.end(); ++itDy )
            {
                full_w = 0;
                char *ptr_x = ptr_y;

                for ( x = 0, itDx = _tesselColWidths.begin(); itDx != _tesselColWidths.end(); ++itDx )
                {
                    // Allocate new OpenGL texture resource w/o any data
                    GLTexture texture( *itDx, *itDy, row_length, fmt, NULL);

                    _tesselAll.push_back( texture );
                    x += *itDx;
                    ptr_x += (*itDx) * bytes_pp;
                    full_w += *itDx;
                }
                y += *itDy;
                ptr_y += (*itDy) * pitch;
                full_h += *itDy;
            }

            GLTexture::_Wrap_s = wrap_s; // Load saved parameters
            GLTexture::_Wrap_t = wrap_t;
        }


        {   // Generate all sub-quad vertices {x,y,u,v}
            int x,y;
            std::vector< GLuint >::iterator itDx, itDy;
            std::vector< Tessel >::iterator itTess = _tesselAll.begin();

            double y1 = 0;
            double s_K1 = G_GLTesselatorStartingTexelOffset;
            double s_K2 = G_GLTesselatorEndingTexelOffset;

            for ( y = 0, itDy = _tesselRowHeights.begin(); itDy != _tesselRowHeights.end(); ++itDy )
            {
                double dv = 1.0 / (*itDy);
                double v1 = 0.0 + s_K1 * dv;
                double v2 = 1.0;
                if ( itDy == _tesselRowHeights.end() )
                {
                    v2 -= (full_h - _h + s_K2) * dv;
                }
                else
                {
                    v2 -= s_K2 * dv;
                }
                
                double x1 = 0;
                double y2 = ( y + *itDy ) / (double)( _h );

                for ( x = 0, itDx = _tesselColWidths.begin(); itDx != _tesselColWidths.end(); ++itDx )
                {
                    double du = 1.0 / (*itDx);
                    double u1 = 0.0 + s_K1 * du;
                    double u2 = 1.0;
                    if ( itDx == _tesselColWidths.end() )
                    {
                        u2 -= (full_w - _w + s_K2) * du;
                    }
                    else
                    {
                        u2 -= s_K2 * du;
                    }

                    double x2 = ( x + *itDx ) / (double)( _w );

                    (*itTess)._texture.Use();
                    (*itTess)._u1 = u1;
                    (*itTess)._u2 = u2;
                    (*itTess)._v1 = v1;
                    (*itTess)._v2 = v2;
                    (*itTess)._x1 = x1;
                    (*itTess)._x2 = x2;
                    (*itTess)._y1 = y1;
                    (*itTess)._y2 = y2;
                    
                    ++itTess;
                    x += (*itDx);
                    x1 = x2;
                }
                y += (*itDy);
                y1 = y2;
            }
        }

        UpdateAllTextures( pix_buf );
    }

    GLImageTesselator::~GLImageTesselator()
    {
    }

    // Render image as quad 1.0x1.0 at (current) origin
    void GLImageTesselator::Render()
    {
        for ( int i=0, I=_tesselRender.size(); i<I; ++i )
        {
            Tessel &t = _tesselAll[ _tesselRender[i] ];
            if ( t._texture.Use() )
            {
                glBegin( GL_TRIANGLE_STRIP); {
                    glTexCoord2d( t._u1, t._v1); glVertex2d( t._x1, t._y1);
                    glTexCoord2d( t._u1, t._v2); glVertex2d( t._x1, t._y2);
                    glTexCoord2d( t._u2, t._v1); glVertex2d( t._x2, t._y1);
                    glTexCoord2d( t._u2, t._v2); glVertex2d( t._x2, t._y2);
                } glEnd();
            }
			else {
				bool debugPoint = 0;
			}
        }
    }

    // Scale to 1:1 in pixel domain, translate to (x, y) in traditional screen coordinates and render
    void GLImageTesselator::Render( float screen_x, float screen_y, bool absolute)
    {
        glPushMatrix();
        if ( absolute)
        {
            glLoadIdentity();
        }
        RenderTransform T;
        SetRenderTransform( &T, 1.0, 1.0 );
        UseRenderTransform( &T, screen_x, screen_y );
        Render();
        glPopMatrix();
    }

    void GLImageTesselator::Render( float screen_x, float screen_y, float screen_w, float screen_h, bool absolute )
    {
        glPushMatrix();
        if ( absolute)
        {
            glLoadIdentity();
        }
        RenderTransform T;
        SetRenderTransform( &T, (screen_w/_w), (screen_h/_h) );
        UseRenderTransform( &T, screen_x, screen_y );
        Render();
        glPopMatrix();
    }
    
    void GLImageTesselator::SetRenderTransform( GLImageTesselator::RenderTransform *T, double w_scale, double h_scale )
    {
        GAL::P4f params;
        glGetFloatv( GL_VIEWPORT, params);
        T->l_bnd = -params.x[0];
        T->t_bnd = -params.x[1];
        T->h_full = params.x[2]; //horizontal
        T->v_full = params.x[3]; //vertical
        T->h_size = T->h_full-2*T->l_bnd; //pixels per 1 u unit
        T->v_size = T->v_full-2*T->t_bnd; //pixels per 1 v unit
        T->h_full_recip = 1.0/T->h_full;
        T->v_full_recip = 1.0/T->v_full;
        T->w_scale = w_scale;
        T->h_scale = h_scale;
        T->x_scale = 2*_w*T->w_scale/T->h_full;
        T->y_scale = 2*_h*T->h_scale/T->v_full;
        T->x_scale_recip = 1.0/T->x_scale;
        T->y_scale_recip = 1.0/T->y_scale;
        glScaled( T->x_scale, -T->y_scale, 1.0 );
        glTranslated( -T->x_scale_recip, -T->y_scale_recip, 0.0);
    }

    void GLImageTesselator::UseRenderTransform( GLImageTesselator::RenderTransform *T, double screen_x, double screen_y )
    {
        double x = 2.0 * (T->l_bnd + screen_x) * T->h_full_recip * T->x_scale_recip;
        double y = 2.0 * (T->t_bnd + screen_y) * T->v_full_recip * T->y_scale_recip;
        glTranslated( x, y, 0 );
        if ( _flipX )
        {
            glScaled( -1.0, 1.0, 1.0 );
            glTranslated( -1.0, 0.0, 0.0 );
        }
        if ( _flipY )
        {
            glScaled( 1.0, -1.0, 1.0 );
            glTranslated( 0.0, -1.0, 0.0 );
        }
    }


    void GLImageTesselator::Clip( float min_x, float min_y, float max_x, float max_y )
    {
        GLuint x = 0, y = 0;
        int min_c = 0, max_c = _tesselColWidths.size(), min_r = 0, max_r = _tesselRowHeights.size();
        
        int bigW = _tesselColWidths[0];
        int bigH = _tesselRowHeights[0];
        //min_c = min_x / bigW;
        //max_c = max_x / bigW + 1;
        //min_r = min_y / bigH;
        //max_r = max_y / bigH + 1;

        for ( int c=0, C=_tesselColWidths.size(); c < C; ++c )
        {
            GLuint tcw = _tesselColWidths[ c ];
            if ( x <= min_x )
            {
                min_c = c;
            }
            if ( x >= max_x )
            {
                max_c = c;
                break;
            }
            x += tcw;
        }

        for ( int r=0, R=_tesselRowHeights.size(); r < R; ++r )
        {
            GLuint trw = _tesselRowHeights[ r ];
            if ( y <= min_y )
            {
                min_r = r;
            }
            if ( y >= max_y )
            {
                max_r = r;
                break;
            }
            y += trw;
        }


        int nCol = max_c - min_c;
        int nRow = max_r - min_r;

        _tesselRender = std::vector< GLuint >( nCol * nRow );
        _tesselRender.resize(0);

        if ( min_c > _tesselColWidths.size()-1 )
        {
            return;
        }
        if ( min_r > _tesselRowHeights.size()-1 )
        {
            return;
        }
        if ( max_c < 0 )
        {
            return;            
        }
        if ( max_r < 0 )
        {
            return;
        }

        int stride = _tesselColWidths.size();
        int index = min_r * stride;

        for ( int r = min_r; r < max_r; ++r )
        {
            int index1 = index + min_c;
            for ( int c = min_c; c < max_c; ++c )
            {
                _tesselRender.push_back( index1 );
                ++index1;
            }
            index += stride;
        }
    }


    void GLImageTesselator::UpdateAllTextures( const PixelBuffer &pix_buf )
    {
        assert( _w == pix_buf.GetWidth());
        assert( _h == pix_buf.GetHeight());

        int bytes_pp    = pix_buf.GetBytesPP();
        int pitch       = pix_buf.GetPitch();
        int row_length  = pix_buf.GetRowLength();
        int fmt         = pix_buf.GetFormat();
        const void *data= pix_buf.GetData();

        assert(!(pitch % bytes_pp)); // We assume: pitch = n*bytes_pp

        UpdateAllTextures( data, 0, 0, _w, _h, fmt, row_length );
    }
    
    void GLImageTesselator::UpdateAllTextures( const void *data, int px, int py, int width, int height, int fmt, int row_length )
    {
        //assert( _w == width );
        //assert( _h == height );

        int bytes_pp    = (fmt+1)/8;
        int pitch       = row_length * bytes_pp;

        assert(!(pitch % bytes_pp)); // We assume: pitch = n*bytes_pp

        int full_w=0; // sum of all texture quads
        int full_h=0;
        int iTess=0; // tessel index

        {   // Create MxN quad textures
            GLint wrap_s = GLTexture::_Wrap_s; // Record old parameter values
            GLint wrap_t = GLTexture::_Wrap_t;

            GLTexture::_Wrap_s = GL_CLAMP;     // and set u,v clamp mode
            GLTexture::_Wrap_t = GL_CLAMP;

            int x,y;
            std::vector< GLuint >::iterator itDx, itDy;
            char *ptr_y= static_cast< char *>( const_cast< void *>( data ));

            for ( y = 0, itDy = _tesselRowHeights.begin(); itDy != _tesselRowHeights.end(); ++itDy )
            {
                full_w = 0;
                char *ptr_x = ptr_y;

                if ( y >= py && y < py + height )
                {
                    Rect r;
                    for ( x = 0, itDx = _tesselColWidths.begin(); itDx != _tesselColWidths.end(); ++itDx )
                    {
                        // nor (x + tw), nor (y + th) shall exceed pixel_buffer dimensions
                        int tw = std::min( (int)(_w - full_w), (int)(*itDx) );
                        int th = std::min( (int)(_h - full_h), (int)(*itDy) );
                        //_tesselAll[ iTess++ ]._texture.LoadSubPixels( 0, 0, tw, th, row_length, fmt, ptr_x);

                        if ( x >= px && x < px + width )
                        {
                            r = Rect( x, y, tw, th ) & Rect( px, py, width, height );
                            _tesselAll[ iTess++ ]._texture.LoadSubPixels( r.GetX() - x, r.GetY() - y, r.GetW(), r.GetH(), row_length, fmt, ptr_x);
                            ptr_x += r.GetW() * bytes_pp;
                        }
                        x += *itDx;
                        //ptr_x += (*itDx) * bytes_pp;
                        full_w += *itDx;
                    }
                    ptr_y += r.GetH() * pitch;
                }
                y += *itDy;
                //ptr_y += (*itDy) * pitch;
                full_h += *itDy;
            }

            GLTexture::_Wrap_s = wrap_s; // Load saved parameters
            GLTexture::_Wrap_t = wrap_t;
        }
    }


}; //namespace Useless
