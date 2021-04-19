#include "UselessPch.h"

#include "Useless/Graphic/Device/GL/GLTextureSurfaceAllocator.h"

namespace Useless {

    //
    //
    // --== Assign/Free GLImageTesselator with GLTextureSurface ==--
    //
    //

    GLImageTesselator & GLTextureSurfaceAllocator::GetTesselator( const GLTextureSurface *s )
    {
        // I assume current gl context is correct
        // MakeCurrent() is not needed

        // It is safe to use <SPointer<*> > and to return <GLImageTesselator &>,
        // since the only owner of GLImageTesselator is always GLGarbage.
        // GLImageTesselator shall exists only if its GLGarbage exists in GLGarbageCollection.
        // After GLGarbage removeal from GLGarbageCollection, GLImageTesselator shall be destroyed.
        SPointer< GLImageTesselator > tesselator;

        // surface already has assigned GL resources
        GLSurfTessMap::iterator it = _glSurfTess.find( s );
        // surface has assigned tesselator
        if ( it != _glSurfTess.end() )
        {
            assert( s == (*it).second->_surf );
            if ( (*it).second->_age > 0 )
            {
                --(*it).second->_age;
            }
            tesselator = (*it).second->_tess;
            if ( s->_dirty )
            {
                tesselator->UpdateAllTextures( s->_pixelBuffer );
                s->_dirty = false;
            }
            return *tesselator;
        }
        // allocate new tesselator
        else
        {
            // try existing one ( recycle )
            for ( GLGarbageCollection::iterator it = _glGarbage.begin();
                    it != _glGarbage.end(); ++it )
            {
                if ( 0 == (*it)._surf )
                {
                    if (( (*it)._tess->GetWidth() == s->GetWidth() ) &&
                            ( (*it)._tess->GetHeight() == s->GetHeight() ))
                    {
                        (*it)._age = 0;
                        (*it)._surf = s;                    
                        tesselator = (*it)._tess;                    
                        tesselator->UpdateAllTextures( s->_pixelBuffer );
                        s->_dirty = false;
                        _glSurfTess.insert( std::make_pair( s, &(*it) ));
                        return *tesselator;
                    }
                }
            }
            // allocate new one
            tesselator.Reset( new GLImageTesselator( s->_pixelBuffer ));
            s->_dirty = false;
            GLGarbage g;
            g._age = 0;
            g._surf = s;
            g._tess = tesselator;
            _glSurfTess.insert( std::make_pair( s, &(*_glGarbage.insert( _glGarbage.end(), g )) ));
            return *tesselator;
        }
    }

    void GLTextureSurfaceAllocator::ReleaseTesselator( const GLTextureSurface *s, GLImageTesselator &tess )
    {
    }

    void GLTextureSurfaceAllocator::GLFreeGarbage()
    {
        static int s_GLFreeTesselThreshold = 30;
        static int s_GLFreeGarbage = 100;
        static int s_GLGarbageRefSize = 128 * 128;
        
        // make sure we're in proper context
        MakeCurrent();
        
        for ( GLGarbageCollection::iterator itNext, it = _glGarbage.begin();
                it != _glGarbage.end(); it = itNext )
        {
            itNext = it; ++itNext;

            int imageSize = ((*it)._tess->GetWidth() * (*it)._tess->GetHeight());        

            int sizeRatio = imageSize / s_GLGarbageRefSize;

            int sizeC = (sizeRatio < 2 ? 1 : (sizeRatio < 4 ? 2 : (sizeRatio < 9 ? 3 : (sizeRatio < 16 ? 4 : 5) )));

            ++(*it)._age;

            if ( 0 != (*it)._surf )
            {
                if ( (*it)._age > s_GLFreeTesselThreshold / sizeC )
                {
                    GLSurfTessMap::iterator itST = _glSurfTess.find( (*it)._surf );
                    if ( itST != _glSurfTess.end() )
                    {
                        _glSurfTess.erase( itST );
                    }
                    (*it)._surf = 0;
                }
            }
            else
            {
                if ( (*it)._age > s_GLFreeGarbage / sizeC )
                {
                    _glGarbage.erase( it );
                }
            }
        }
    }

    void GLTextureSurfaceAllocator::GLFreeGarbage( GLTextureSurface *s )
    {
        // make sure we're in proper context
        MakeCurrent();

        GLSurfTessMap::iterator itST = _glSurfTess.find( s );
        if ( itST != _glSurfTess.end() )
        {
            for ( GLGarbageCollection::iterator itNext, it = _glGarbage.begin();
                    it != _glGarbage.end(); it = itNext )
            {
                itNext = it; ++itNext;

                if ( (*it)._surf == s )
                {
                    _glGarbage.erase( it );
                    break;
                }
            }
            _glSurfTess.erase( itST );
        }
    }

    std::set< GLTextureSurfaceAllocator * > GLTextureSurfaceAllocator::s_allocators;

    GLTextureSurfaceAllocator::GLTextureSurfaceAllocator()
    {
        s_allocators.insert( this );
    }

    GLTextureSurfaceAllocator::~GLTextureSurfaceAllocator()
    {
        s_allocators.erase( this );
        if ( !_glGarbage.empty() || !_glSurfTess.empty() )
        {
            throw Error("GLTextureSurfaceAllocator: GLFinishGarbage() was not called before GLTextureSurfaceAllocator destructor.");
        }
    }

    void GLTextureSurfaceAllocator::GLFinishGarbage()
    {
        MakeCurrent();
        _glGarbage.clear();
        _glSurfTess.clear();
    }
    
    void GLFreeSurfaceGarbage( GLTextureSurface *s )
    {
        for ( std::set< GLTextureSurfaceAllocator *>::iterator
			iter = GLTextureSurfaceAllocator::s_allocators.begin();
            iter != GLTextureSurfaceAllocator::s_allocators.end();
		    ++iter )
        {
            (*iter)->GLFreeGarbage( s );
        }
    }
/* implemented in Useless/Movie/DShow/CSurfaceRenderer.cpp
	void GLUpdateMovieSamples( GLTextureSurfaceAllocator *pTexAlloc )
	{
		// DEFAULT IMPLEMENTATION....
	}
*/
};//Useless
