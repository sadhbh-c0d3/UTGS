#include "UselessPch.h"

#include "GLResourcePool.h"
#include <algorithm>

namespace Useless {

GLResourcePool::GLResourcePool()
{
}

GLResourcePool::~GLResourcePool()
{
}

GLuint GLResourcePool::AcquireTexture()
{
    GLuint id;
    glGenTextures( 1, &id );
    return id;
}

void GLResourcePool::UnacquireTexture( GLuint id)
{
    glDeleteTextures( 1, &id );
}

GLuint GLResourcePool::AcquireCallList()
{
    GLuint id = glGenLists( 1 );
    return id;
}

void GLResourcePool::UnacquireCallList( GLuint id)
{
    glDeleteLists( id, 1 );
}

}; //namespace Useless
