#ifndef __INCLUDED_USELESS_WGL_SURFACE_H__
#define __INCLUDED_USELESS_WGL_SURFACE_H__

#include "Useless/Graphic/Device/GL/GLContextSurface.h"
#include "Useless/Graphic/Device/GL/wGL/WGLExtensions.h"

namespace Useless {

    class USELESS_API WGLSurface : public GLContextSurface
    {
        public:
            WGLSurface();

            WGLSurface( HDC hdc, Surf::Properties &prop );

            virtual ~WGLSurface();

            virtual bool MakeCurrent();

            virtual GLContextSurface *CreateContext( Surf::Properties &properties ) const;

            HDC     GetParentDC()    const { return _parentDC; }
            HDC     GetHDC()         const { return _devContext; }
            HGLRC   GetHGLRC()       const { return _glContext; }
        
            bool IsValidBlitSource( const Surface &surface ) const;
    
            void Blit( int x, int y,                           // where to blit
                       const Surface &src,                     // what is to be blit
                       const Useless::Rect &src_rect,          // where from
                       const Surf::BlitFX &fx = Surf::BlitFX() // what effect should be applied
                      );

            void MultiBlit( const Surface &sur,
                            const PointList &p,
                            const RectList& r,
                            const Surf::BlitFX &fx = Surf::BlitFX() );

        private:
            HDC         _parentDC;
            HDC         _devContext;
            HPBUFFERARB _glPbuffer;
            HGLRC       _glContext;
            friend class WGLScreen;
    };

};//namespace Useless
#endif//__INCLUDED_USELESS_WGL_SURFACE_H__
