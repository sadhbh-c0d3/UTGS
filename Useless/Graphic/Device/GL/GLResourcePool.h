#ifndef __INCLUDED_USELESS_GL_RESOURCE_POOL_H__
#define __INCLUDED_USELESS_GL_RESOURCE_POOL_H__

#ifdef WIN32
#   define MEAN_AND_LEAN
#   include <windows.h>
#   undef min
#   undef max
#   pragma comment(lib,"opengl32.lib")
#   pragma comment(lib,"glu32.lib")
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace Useless {

/*! OpenGL resources manager
 */
class USELESS_API GLResourcePool
{
public:
    GLuint AcquireTexture();
    void  UnacquireTexture( GLuint id);

    GLuint AcquireCallList();
    void  UnacquireCallList( GLuint id);

    GLResourcePool();
    ~GLResourcePool();

    static GLResourcePool &Instance()
    {
        static GLResourcePool pool;
        return pool;
    }

};

}; //namespace Useless
#endif//__INCLUDED_USELESS_GL_RESOURCE_POOL_H__
