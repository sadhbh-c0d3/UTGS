#ifndef __INCLUDED_USELESS_GL_CONTEXT_SURFACE_H__
#define __INCLUDED_USELESS_GL_CONTEXT_SURFACE_H__

#include "Useless/Graphic/Device/GL/GVM_GLContext.h"
#include "Useless/Graphic/Device/GL/GLTextureSurface.h"
#include "Useless/Graphic/Device/GL/GLTextureSurfaceAllocator.h"

namespace Useless {

    struct USELESS_API GLContextSurface : GLTextureSurfaceAllocator, GVM::GLContext, Surface
    {
        GLContextSurface();
        GLContextSurface( Surf::Properties &properties );

        ~GLContextSurface();
    
        Surface *
            CreateSurface( Surf::Properties &properties ) const;
    
        SPointer< SurfacePainter >
            CreatePainter();

        SPointer< PixelTransfer >
            CreateReader( int channelFormat = COLOR ) const;
        
        SPointer< PixelTransfer >
            CreateWriter( int channelFormat = COLOR );
        
        void GetViewport ( int *rect );
        void SetViewport ( int x, int y, int w, int h );
        void InitViewport( int x, int y, int w, int h );
        void Invoke( const GVM::Message &msg );
        
        bool IsValidBlitSource( const Surface &surface ) const;

        void Blit
            ( int x, int y,
              const Surface &src,
              const Useless::Rect &src_rect,
              const Surf::BlitFX & fx
            );

        void Clear
            ( unsigned long color,
              const Useless::Rect &drc
            );

    
        void MultiBlit
            ( const Surface &sur,
              const PointList &p,
              const RectList& r,
              const Surf::BlitFX &fx = Surf::BlitFX() );


        int  GetWidth () const { return _w; }
        int  GetHeight() const { return _h; }
        bool IsColorKeyed () const { return false; }
        bool SetColorKey  ( unsigned long color ) { assert(0); return false; }
        void SetNoColorKey() {}
        void GetProperties( Surf::Properties *prop ) const;
        bool SetProperties( const Surf::Properties &prop ) { assert(0); return 0; }
    
        bool SetClipper( const RectList &region );
        RectList GetClipper( const Rect &crop ) const;
    
        void SetConstantAlpha( int alpha );
        int  GetConstantAlpha() const;

        bool operator == ( const Surface& s );
        bool operator != ( const Surface& s );

        virtual bool MakeCurrent() = 0;
        virtual GLContextSurface *CreateContext( Surf::Properties &properties ) const = 0;

        void Prepare2D();

        void * GetExtFun( const std::string &name );

        void BlendFun( int sf, int df, bool enabled );
        void ConstColor( int red, int green, int blue );
        void ConstMatrix( float *matrix );

        void SetSize( int w, int h );
    
    protected:
        GLint       _w;
        GLint       _h;
        GLboolean   _primary;
        GLuint      _numBackbuffers;
        GLboolean   _transparency;
        GLint       _viewport[4];
        GLfloat     _constantAlpha;
        GLenum      _srcBlend;
        GLenum      _dstBlend;
        GLfloat     _constColorRed;
        GLfloat     _constColorGreen;
        GLfloat     _constColorBlue;
        RectList    _clipRegion;
        GLuint      _enableFlags;
        GLfloat     _constMatrix[16];

        enum { CONST_MATRIX };
        void SetFlag( int flag ) { _enableFlags |= 1 << flag; }
        void ClrFlag( int flag ) { _enableFlags &= ~(1 << flag); }
        bool GetFlag( int flag ) { return _enableFlags & (1 << flag); }
    };

}; //Useless


#endif//__INCLUDED_USELESS_GL_CONTEXT_SURFACE_H__
