#ifndef __INCLUDED_USELESS_GL_SURFACE_PAINTER_H__
#define __INCLUDED_USELESS_GL_SURFACE_PAINTER_H__

#include "Useless/Graphic/Device/GL/GLContextSurface.h"
#include "Useless/Graphic/Planes/SurfacePainter.h"

namespace Useless {

    class USELESS_API GLSurfacePainter : public SurfacePainter
    {
    public:
        GLSurfacePainter( GLContextSurface *surf )
            : _surf( surf )
            , _w( surf->GetWidth() )
            , _h( surf->GetHeight() )
            , _clipRegion( surf->GetClipper( Rect() ) )
        {
            glPushMatrix();
            _surf->Prepare2D();
            float viewRect[4];
            glGetFloatv( GL_VIEWPORT, viewRect );
            glScalef( 1.0, -1.0, 1.0 );
            glScalef( 2.0/viewRect[2], 2.0/viewRect[3], 1.0 );
            glTranslatef( -0.5*_w, -0.5*_h, 0.0 );
            glEnable( GL_SCISSOR_TEST );
            glEnable( GL_BLEND );
            //glBlendFunc( GL_SRC_ALPHA, GL_ONE );
            glEnable( GL_SMOOTH );
            glEnable( GL_LINE_SMOOTH );
        }

        ~GLSurfacePainter()
        {
            glPopMatrix();
        }

        //-- Put single pixel
        void
        PutPixel        ( const Pos& p, NormalRGB color ) const
        {
            for ( RectList::const_iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                const Rect &r = (*itRect);
                glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                glColor4i( color.r, color.g, color.b, color.a );
                glBegin( GL_POINTS );
                glVertex2i( p.x, p.y );
                glEnd();
            }
        }

        //-- Paint one line segment from p1 to p2
        void
        PaintLine       ( const Pos &p1, const Pos &p2, NormalRGB color,
                                       int thickness, int algorithm = 0 ) const
        {
            for ( RectList::const_iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                const Rect &r = (*itRect);
                glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                glColor4f( color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0 );
                glLineWidth( thickness );
                glBegin( GL_LINES );
                    glVertex2i( p1.x, p1.y );
                    glVertex2i( p2.x, p2.y );
                glEnd();
            }
        }
        
        //-- Paint polyline
        void
        PaintLine       ( const std::vector< Pos > &vertices, NormalRGB color,
                                       int thickness, int algorithm = 0 ) const
        {
            glColor4f( color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0 );
            glVertexPointer( 2, GL_INT, sizeof(Pos), &vertices[0] );
            glEnableClientState( GL_VERTEX_ARRAY );
            glLineWidth( thickness );
            for ( RectList::const_iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                const Rect &r = (*itRect);
                glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                glDrawArrays( GL_LINE_STRIP, 0, vertices.size() );
            }
        }
        
        //-- Paint polygon
        void
        PaintPolygon    ( const std::vector<Pos> &vertices, NormalRGB color ) const
        {
            glColor4f( color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0 );
            glVertexPointer( 2, GL_INT, sizeof(Pos), &vertices[0] );
            glEnableClientState( GL_VERTEX_ARRAY );
            for ( RectList::const_iterator itRect = _clipRegion.begin();
                    itRect != _clipRegion.end(); ++itRect )
            {
                const Rect &r = (*itRect);
                glScissor( r.GetX(), _h - r.GetY() - r.GetH(), r.GetW(), r.GetH() );
                glDrawArrays( GL_POLYGON, 0, vertices.size() );
            }
        }

        
    private:
        GLContextSurface *_surf;
        const RectList _clipRegion;
        int _w;
        int _h;
    };
    
}; //Useless

#endif//__INCLUDED_USELESS_GL_SURFACE_PAINTER_H__
