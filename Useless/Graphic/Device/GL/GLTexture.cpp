#include "UselessPch.h"

#pragma warning (disable:4786)
//------------------------------
#include "Useless/Graphic/Device/GL/GLTexture.h"
#include "Useless/Graphic/Device/GL/GLResourcePool.h"
#include "Useless/Graphic/detail/PixelBuffer.h"
#include "Useless/Graphic/Linear.h"
#include "Useless/Graphic/Pixel/PixelTypes.h"
#include "Useless/Graphic/Pixel/PixelPtr.h"

namespace Useless {

GLint GLTexture::_Min_filter=GL_LINEAR;
GLint GLTexture::_Max_filter=GL_LINEAR;
GLint GLTexture::_Wrap_s=GL_REPEAT;
GLint GLTexture::_Wrap_t=GL_REPEAT;

GLTexture::GLTexture( int w, int h )
    :_w(w), _h(h), _bind_id(-1), _empty(true)
{
    _bind_id = GLResourcePool::Instance().AcquireTexture();
}

GLTexture::GLTexture( int w, int h, int row_length, int fmt, void *data, int level, int priority)
    :_w(w), _h(h), _bind_id(-1), _empty(true)
{
    _bind_id = GLResourcePool::Instance().AcquireTexture();
    LoadPixels( w, h, row_length, fmt, data, level, priority );
}

GLTexture::~GLTexture()
{
    if ( Unique() )
    {
        GLResourcePool::Instance().UnacquireTexture( _bind_id );
    }
}

template< class PixelType > void ScanForEmptyPixels( const PixelType&, void *data, int pitch, bool *empty, int _w, int _h )
{
    GenericPixelPtr< PixelType > ptr( data, pitch );
    int h =_h;
    for ( ;h-- ; ptr.Next() )
    {
        GenericPixelPtr< PixelType > xptr = ptr;
        int w =_w;
        for ( ;w-- ; ++xptr )
        {
			unsigned char alpha = xptr.Read().Get().c.a;
            if ( alpha > 0x4 )
            {
                *empty = false;
                return;
            }
        }
    }
    *empty = true;
}

void GLTexture::LoadPixels( int w, int h, int row_length, int fmt, void *data, int level, int priority)
{
    glBindTexture( GL_TEXTURE_2D, _bind_id);
  
    glPixelStorei( GL_UNPACK_ROW_LENGTH, row_length);

    GLenum format, tf;
    switch ( fmt)
    {
        case ImageFormat::B8G8R8:   format=GL_BGR_EXT;  tf=GL_RGB8; break;
        case ImageFormat::R8G8B8:   format=GL_RGB;      tf=GL_RGB8; break;
        case ImageFormat::B8G8R8A8: format=GL_BGRA_EXT; tf=GL_RGBA8; break;
        case ImageFormat::R8G8B8A8: format=GL_RGBA;     tf=GL_RGBA8; break;
        case ImageFormat::SINGLE8:  format=GL_LUMINANCE;tf=GL_LUMINANCE; break;
        default: assert(!"GLTexture: unknown GL format!");
    };

    glTexImage2D( GL_TEXTURE_2D, level, tf, _w, _h, 0, //< target texture format
                  format, GL_UNSIGNED_BYTE, data); //< format of source data

    GLenum err= glGetError();
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, priority);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _Min_filter);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _Max_filter);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _Wrap_s);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _Wrap_t);

    if ( data )
    {
        int pitch = row_length * ImageFormat::GetBpp( ImageFormat::Format( fmt ));
        __PIXEL_SWITCH( fmt, ScanForEmptyPixels(p, data, pitch, &_empty, w, h), p );
    }
    else {
        _empty = true;
    }
}

void GLTexture::LoadSubPixels( int x, int y, int w, int h, int row_length, int fmt, void *data, int level, int priority )
{
    if ( !data )
    {
        return;
    }
    if ( _empty )
    {
        int pitch = row_length * ImageFormat::GetBpp( ImageFormat::Format( fmt ));
        __PIXEL_SWITCH( fmt, ScanForEmptyPixels(p, data, pitch, &_empty, w, h), p );
        if ( _empty )
        {
            return;
        }
    }
    
    glBindTexture( GL_TEXTURE_2D, _bind_id);
  
    glPixelStorei( GL_UNPACK_ROW_LENGTH, row_length);

    GLenum format, tf;
    switch ( fmt)
    {
        case ImageFormat::B8G8R8:   format=GL_BGR_EXT;  tf=GL_RGB8; break;
        case ImageFormat::R8G8B8:   format=GL_RGB;      tf=GL_RGB8; break;
        case ImageFormat::B8G8R8A8: format=GL_BGRA_EXT; tf=GL_RGBA8; break;
        case ImageFormat::R8G8B8A8: format=GL_RGBA;     tf=GL_RGBA8; break;
        case ImageFormat::SINGLE8:  format=GL_LUMINANCE;tf=GL_LUMINANCE; break;
        default: assert(!"GLTexture: unknown GL format!");
    };

    glTexSubImage2D( GL_TEXTURE_2D, level, x, y, w, h, format, GL_UNSIGNED_BYTE, data );

    GLenum err= glGetError();
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, priority);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _Min_filter);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _Max_filter);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _Wrap_s);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _Wrap_t);
}

bool GLTexture::Use()
{
    if ( !_empty )
    {
        glBindTexture( GL_TEXTURE_2D, _bind_id);
        return true;
    }
    else {
        return false;
    }
}

}; //namespace Useless
