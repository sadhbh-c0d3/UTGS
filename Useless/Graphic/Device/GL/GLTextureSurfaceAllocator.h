#ifndef __INCLUDED_GL_TEXTURE_SURFACE_ALLOCATOR_H__
#define __INCLUDED_GL_TEXTURE_SURFACE_ALLOCATOR_H__

#include "Useless/Graphic/Device/GL/GLTextureSurface.h"
#include <list>
#include <set>
#include <map>

namespace Useless {

    struct USELESS_API GLGarbage
    {
        int                         _age;
        const GLTextureSurface     *_surf;
        SPointer< GLImageTesselator > _tess;
    };

    typedef std::map< const GLTextureSurface *, GLGarbage * > GLSurfTessMap;
    typedef std::list< GLGarbage > GLGarbageCollection;

    struct USELESS_API GLTextureSurfaceAllocator
    {
        GLSurfTessMap       _glSurfTess;
        GLGarbageCollection _glGarbage;

        GLTextureSurfaceAllocator();
        ~GLTextureSurfaceAllocator();

        GLImageTesselator &   GetTesselator ( const GLTextureSurface *s );
        void              ReleaseTesselator ( const GLTextureSurface *s, GLImageTesselator & tess );

        void GLFreeGarbage();
        void GLFreeGarbage( GLTextureSurface *s );
        void GLFinishGarbage();

        virtual bool MakeCurrent() = 0; 

        static std::set< GLTextureSurfaceAllocator * > s_allocators;
    };

    USELESS_API void GLFreeSurfaceGarbage( GLTextureSurface *s );
    USELESS_API void GLUpdateMovieSamples( GLTextureSurfaceAllocator *pTexAlloc );


};//Useless
#endif//__INCLUDED_GL_TEXTURE_SURFACE_ALLOCATOR_H__
