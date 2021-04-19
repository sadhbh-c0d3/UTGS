#include "UselessPch.h"

#include "Useless/Graphic/Device/GL/GLTextureSurfaceAllocator.h"
#include "Useless/Graphic/Device/GL/GVM_GLContext.h"
#include "Useless/Graphic/Device/GVM_Message.h"
#include "Dynamo/classes/CInterface.h"

namespace Useless {

    namespace GVM {

        struct GLTexture : Texture, Dynamo::CInterface
        {
            int     m_width;
            int     m_height;
            GLint   m_bindId;

            GLenum BadEnum( int e )
            {
                return -1;
            }

            GLenum Color2GL( int color )
            {
                switch( color )
                {
                    case 3: return 3;
                    case 4: return 4;
                    case RGB: return GL_RGB; 
                    case RGBA: return GL_RGBA; 
                    case RGB8: return GL_RGB8; 
                    case RGBA8: return GL_RGBA8;
                    case RGB16: return GL_RGB16; 
                    case RGBA16: return GL_RGBA16; 
                    case ALPHA8: return GL_ALPHA8; 
                    default: return BadEnum( color );
                }
            }

            GLenum Type2GL( int type )
            {
                switch( type )
                {
                    case UBYTE: return GL_UNSIGNED_BYTE; 
                    case USHORT: return GL_UNSIGNED_SHORT;
                    case UINT: return GL_UNSIGNED_INT;
                    case FLOAT: return GL_FLOAT;
                    default: return BadEnum( type );
                }
            }

            GLenum Format2GL( int format )
            {
                switch( format )
                {
                    case RGB: return GL_RGB; 
                    case RGBA: return GL_RGBA; 
                    case BGR: return GL_BGR_EXT; 
                    case BGRA: return GL_BGRA_EXT; 
                    case ALPHA8: return GL_ALPHA; 
                    default: return BadEnum( format );
                }
            }

            GLenum Filter2GL( int filter )
            {
                switch( filter )
                {
                    case NEAREST: return GL_NEAREST; 
                    case LINEAR:  return GL_LINEAR;
                    case NEAREST_MIPMAP_NEAREST:  return GL_NEAREST_MIPMAP_NEAREST;
                    case LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
                    case NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
                    case LINEAR_MIPMAP_LINEAR:  return GL_LINEAR_MIPMAP_LINEAR;
                    default: return BadEnum( filter );
                }
            }

            GLenum Wrap2GL( int wrap )
            {
                switch( wrap )
                {
                    case CLAMP: return GL_CLAMP;
                    case REPEAT: return GL_REPEAT;
                    default: return BadEnum( wrap );
                }
            }

            GLTexture( int width, int height, int color ): m_width( width ), m_height( height )
            {
                m_bindId = GLResourcePool::Instance().AcquireTexture();
                glBindTexture( GL_TEXTURE_2D, m_bindId );
                glTexImage2D( GL_TEXTURE_2D, 0, Color2GL( color ), m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
                GLenum err= glGetError();
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0.0 );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            }

            ~GLTexture()
            {
                GLResourcePool::Instance().UnacquireTexture( m_bindId );
            }

            void LoadSubPixels( void *data, int level, int x, int y, int width, int height, int rowLen, int pixType, int pixFmt )
            {
                glBindTexture( GL_TEXTURE_2D, m_bindId );
                glPixelStorei( GL_UNPACK_ROW_LENGTH, rowLen );
                glTexSubImage2D( GL_TEXTURE_2D, level, x, y, width, height, Format2GL( pixFmt ), Type2GL( pixType ), data );
                GLenum err= glGetError();
            }

            void SetFilter( int minFlt, int magFlt )
            {
                glBindTexture( GL_TEXTURE_2D, m_bindId );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter2GL( magFlt ));
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter2GL( minFlt ));
            }

            void SetWrap( int wrapS, int wrapT )
            {
                glBindTexture( GL_TEXTURE_2D, m_bindId );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap2GL( wrapS ));
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap2GL( wrapT ));
            }

            void SetPriority( float priority )
            {
                glBindTexture( GL_TEXTURE_2D, m_bindId );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0.0 );
            }

            Message BindMsg()
            {
                return BindTextureMsg( TEXTURE_2D, m_bindId );
            }

            int GetWidth()
            {
                return m_width;
            }

            int GetHeight()
            {
                return m_height;
            }
        };

        GLContext::GLContext(): m_indexPointer(0)
        {
        }

        GLContext::~GLContext()
        {
        }

        Texture * GLContext::CreateTexture( int width, int height, int color )
        {
            return new GLTexture( width, height, color );
        }

        void GLContext::GetViewport( int *rect )
        {
            glGetIntegerv( GL_VIEWPORT, rect );
        }

        void GLContext::SetViewport( int x, int y, int w, int h )
        {
            glViewport( x, y, w, h );
            glScissor( x, y, w, h );
        }

        void GLContext::InitViewport( int x, int y, int w, int h )
        {
            glViewport( x, y, w, h );
            glScissor( x, y, w, h );
            glEnable( GL_SCISSOR_TEST );
            glClear( GL_DEPTH_BUFFER_BIT );
            glMatrixMode( GL_TEXTURE );
            glLoadIdentity();
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glEnable( GL_DEPTH_TEST );
            glDisable( GL_TEXTURE_2D );
            glDisable( GL_BLEND );
        }

        void GLContext::SetOrtho2D( double left, double right, double bottom, double top )
        {
            gluOrtho2D( left, right, bottom, top );
        }

        void GLContext::SetPerspective( double fov, double aspect, double znear, double zfar )
        {
            gluPerspective( fov, aspect, znear, zfar );
        }

        void GLContext::SetFrustum    ( double left, double right, double bottom, double top, double znear, double zfar )
        {
            glFrustum( left, right, bottom, top, znear, zfar );
        }

        void GLContext::SetOrtho      ( double left, double right, double bottom, double top, double znear, double zfar )
        {
            glOrtho( left, right, bottom, top, znear, zfar );
        }


        int GLContext::BadEnumGL( int flag )
        {
            assert(0);
            return -1;
        }

        int GLContext::BadEnumGVM( int flag )
        {
            assert(0);
            return -1;
        }

        int GLContext::ToGLFlag( int gvm )
        {
            switch( gvm )
            {
                case PROJECTION: return GL_PROJECTION;
                case MODELVIEW: return GL_MODELVIEW;
                case TEXTURE: return GL_TEXTURE;
                case COLOR: return GL_COLOR;
                case BLEND: return GL_BLEND;
                case DEPTH_TEST: return GL_DEPTH_TEST;
                case TEXTURE_2D: return GL_TEXTURE_2D;
                case COLOR_MATERIAL: return GL_COLOR_MATERIAL;
                case LIGHTING: return GL_LIGHTING;
                case LIGHT0: return GL_LIGHT0;
                case LIGHT1: return GL_LIGHT1;
                case LIGHT2: return GL_LIGHT2;
                case LIGHT3: return GL_LIGHT3;
                case LIGHT4: return GL_LIGHT4;
                case LIGHT5: return GL_LIGHT5;
                case LIGHT6: return GL_LIGHT6;
                case LIGHT7: return GL_LIGHT7;
                case BACK: return GL_BACK;
                case FRONT: return GL_FRONT;
                case FRONT_AND_BACK: return GL_FRONT_AND_BACK;
                default: return BadEnumGVM( gvm );
            };
        }

        int GLContext::ToGLClientFlag( int gvm )
        {
            switch( gvm )
            {
                case VERTEX_ARRAY: return GL_VERTEX_ARRAY;
                case NORMAL_ARRAY: return GL_NORMAL_ARRAY;
                case COLOR_ARRAY: return GL_COLOR_ARRAY;
                case TEXTURE_COORD_ARRAY: return GL_TEXTURE_COORD_ARRAY;
                default: return BadEnumGVM( gvm );
            };
        }

        int GLContext::ToGLPrimitive( int gvm )
        {
            switch( gvm )
            {
                case LINES: return GL_LINES;
                case LINE_STRIP: return GL_LINE_STRIP;
                case LINE_LOOP: return GL_LINE_LOOP;
                case TRIANGLES: return GL_TRIANGLES;
                case TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
                default: return BadEnumGVM( gvm );
            };
        }

        int GLContext::ToGLBlendFun( int gvm )
        {
            switch( gvm )
            {
                case SRC_ALPHA: return GL_SRC_ALPHA;
                case ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
                case ONE: return GL_ONE;
                default: return BadEnumGVM( gvm );
            };
        }

        int GLContext::ToGLMaterial( int gvm )
        {
            switch( gvm )
            {
                case AMBIENT: return GL_AMBIENT;
                case DIFFUSE: return GL_DIFFUSE;
                case AMBIENT_AND_DIFFUSE: return GL_AMBIENT_AND_DIFFUSE;
                case SPECULAR: return GL_SPECULAR;
                case EMISSION: return GL_EMISSION;
                case SHININESS: return GL_SHININESS;
                default: return BadEnumGVM( gvm );
            };
        }

        int GLContext::ToGLLight( int gvm )
        {
            switch( gvm )
            {
                case AMBIENT: return GL_AMBIENT;
                case DIFFUSE: return GL_DIFFUSE;
                case SPECULAR: return GL_SPECULAR;
                case POSITION: return GL_POSITION;
                case SPOT_DIRECTION: return GL_SPOT_DIRECTION;
                case SPOT_EXPONENT: return GL_SPOT_EXPONENT;
                case SPOT_CUTOFF: return GL_SPOT_CUTOFF;
                case CONSTANT_ATTENUATION: return GL_CONSTANT_ATTENUATION;
                case LINEAR_ATTENUATION: return GL_LINEAR_ATTENUATION;
                case QUADRATIC_ATTENUATION: return GL_QUADRATIC_ATTENUATION;
                default: return BadEnumGVM( gvm );
            };
        }

        int GLContext::ToGLLightModel( int gvm )
        {
            switch( gvm )
            {
                case LIGHT_MODEL_AMBIENT: return GL_LIGHT_MODEL_AMBIENT;
                case LIGHT_MODEL_LOCAL_VIEWER: return GL_LIGHT_MODEL_LOCAL_VIEWER;
                case LIGHT_MODEL_TWO_SIDE: return GL_LIGHT_MODEL_TWO_SIDE;
                default: return BadEnumGVM( gvm );
            };
        }


        int GLContext::FromGLBlendFun( int gl )
        {
            switch( gl )
            {
                case GL_SRC_ALPHA: return SRC_ALPHA;
                case GL_ONE_MINUS_SRC_ALPHA: return ONE_MINUS_SRC_ALPHA;
                case GL_ONE: return ONE;
                default: return BadEnumGL( gl );
            };
        }

        int GLContext::FromGLFace( int gl )
        {
            switch( gl )
            {
                case GL_FRONT: return FRONT;
                case GL_BACK: return BACK;
                case GL_FRONT_AND_BACK: return FRONT_AND_BACK;
                default: return BadEnumGL( gl );
            };
        }

        int GLContext::FromGLMaterial( int gl )
        {
            switch( gl )
            {
                case GL_AMBIENT: return AMBIENT;
                case GL_EMISSION: return EMISSION;
                case GL_SPECULAR: return SPECULAR;
                case GL_AMBIENT_AND_DIFFUSE: return AMBIENT_AND_DIFFUSE;
                default: return BadEnumGL( gl );
            }
        }


        void GLContext::Save( int flag )
        {
            assert( !m_restoreVector.empty() );
            Message &msg = *m_restoreVector.rbegin();
            switch( flag )
            {
                case CURRENT_MATRIX:
                    {
                        glPushMatrix();
                        msg += PopMatrixMsg();
                    }
                    break;
                case BLEND_FUNC:
                    {
                        GLint s,d;
                        glGetIntegerv( GL_BLEND_SRC, &s );
                        glGetIntegerv( GL_BLEND_DST, &d );
                        msg += BlendFuncMsg( FromGLBlendFun( s ), FromGLBlendFun( d ));
                    }
                    break;
                case COLOR_MATERIAL_MODE:
                    {
                        GLboolean b;
                        GLint f,m;
                        glGetIntegerv( GL_COLOR_MATERIAL_FACE, &f );
                        glGetIntegerv( GL_COLOR_MATERIAL_PARAMETER, &m );
                        if ( glIsEnabled( GL_COLOR_MATERIAL ))
                        {
                            msg += EnableMsg( COLOR_MATERIAL );
                        }
                        else
                        {
                            msg += DisableMsg( COLOR_MATERIAL );
                        }
                        msg += ColorMaterialMsg( FromGLFace( f ), FromGLMaterial( m ));
                    }
                    break;
                case VERTEX_ARRAY:
                case NORMAL_ARRAY:
                case COLOR_ARRAY:
                case TEXTURE_COORD_ARRAY:
                    {
                        GLboolean b = glIsEnabled( ToGLClientFlag( flag ));
                        if ( 0 != b ) {
                            msg += EnableClientStateMsg( flag );
                        }
                        else {
                            msg += DisableClientStateMsg( flag );
                        }
                    }
                    break;
                default:
                    {
                        GLboolean b = glIsEnabled( ToGLFlag( flag ));
                        if ( 0 != b ) {
                            msg += EnableMsg( flag );
                        }
                        else {
                            msg += DisableMsg( flag );
                        }
                    }
                    break;
            };
        }

        void GLContext::SaveLight( int light, int flag )
        {
            assert( !m_restoreVector.empty() );
            Message &msg = *m_restoreVector.rbegin();
            switch( flag )
            {
                case AMBIENT:
                case DIFFUSE:
                case SPECULAR:
                case AMBIENT_AND_DIFFUSE:
                case POSITION:
                    {
                        GAL::P4f value;
                        glGetLightfv( ToGLFlag( light ), ToGLLight( flag ), value );
                        msg += LightMsg( light, flag, value );
                    }
                    break;

                case SPOT_DIRECTION:
                    {
                        GAL::P3f value;
                        glGetLightfv( ToGLFlag( light ), GL_SPOT_DIRECTION, value );
                        msg += LightMsg( light, SPOT_DIRECTION, value );
                    }
                    break;

                case SPOT_EXPONENT:
                case SPOT_CUTOFF:
                case CONSTANT_ATTENUATION:
                case LINEAR_ATTENUATION:
                case QUADRATIC_ATTENUATION:
                    {
                        GLint value;
                        glGetLightiv( ToGLFlag( light ), ToGLLight( flag ), &value );
                        msg += LightMsg( light, flag, value );
                    }
                    break;
                default:
                    break;
            };
        }

        void GLContext::SaveMaterial( int face, int flag )
        {
            assert( !m_restoreVector.empty() );
            Message &msg = *m_restoreVector.rbegin();
            switch( flag )
            {
                case AMBIENT:
                case DIFFUSE:
                case SPECULAR:
                case AMBIENT_AND_DIFFUSE:
                case EMISSION:
                    {
                        GAL::P4f value;
                        glGetMaterialfv( ToGLFlag( face ), ToGLMaterial( flag ), value );
                        msg += MaterialMsg( face, flag, value );
                    }
                    break;
                case SHININESS:
                    {
                        GLfloat value;
                        glGetMaterialfv( ToGLFlag( face ), GL_SHININESS, &value );
                        msg += MaterialMsg( face, SHININESS, value );
                    }
                    break;
                default:
                    break;
            };
        }

        void GLContext::Restore()
        {
            assert( !m_restoreVector.empty() );
            Invoke( *m_restoreVector.rbegin() );
        }

        void GLContext::Invoke( const Message &msg )
        {
            for ( Message::message_t::const_iterator &it = msg.m_message.begin();
                    it != msg.m_message.end(); ++it )
            {
                unsigned int size = (*it).m_int;
                int opcode = (*++it).m_int;
                switch( opcode )
                {
                    // Transformation
                    case MATRIX_MODE:
                        assert( size == 3 );
                        {
                            int mode = ToGLFlag( (*++it).m_int );
                            glMatrixMode( mode );
                        }
                        break;
                        
                    case PUSH_MATRIX:
                        assert( size == 2 );
                        {
                            glPushMatrix();
                        }
                        break;

                    case POP_MATRIX:
                        assert( size == 2 );
                        {
                            glPopMatrix();
                        }
                        break;

                    case LOAD_IDENTITY:
                        assert( size == 2 );
                        {
                            glLoadIdentity();
                        }
                        break;

                    case LOAD_MATRIX:
                        assert( size == 18 );
                        {
                            float x[16];
                            for ( int i=0; i<16; ++i )
                            {
                                x[i] = (*++it).m_float;
                            }
                            glLoadMatrixf( x );
                        }
                        break;

                    case MULT_MATRIX:
                        assert( size == 18 );
                        {
                            float x[16];
                            for ( int i=0; i<16; ++i )
                            {
                                x[i] = (*++it).m_float;
                            }
                            glMultMatrixf( x );
                        }
                        break;

                    case TRANSLATE:
                        assert( size == 5 );
                        {
                            float x = (*++it).m_float;
                            float y = (*++it).m_float;
                            float z = (*++it).m_float;
                            glTranslatef( x, y ,z );
                        }
                        break;

                    case ROTATE:
                        assert( size == 6 );
                        {
                            float angle = (*++it).m_float;
                            float x = (*++it).m_float;
                            float y = (*++it).m_float;
                            float z = (*++it).m_float;
                            glRotatef( angle, x ,y ,z );
                        }
                        break;

                    case SCALE:
                        assert( size == 5 );
                        {
                            float x = (*++it).m_float;
                            float y = (*++it).m_float;
                            float z = (*++it).m_float;
                            glScalef( x, y ,z );
                        }
                        break;

                        // Blocks
                    case BLOCK_BEGIN:
                        {
                            m_restoreVector.push_back( Message() );
                        }
                        break;

                    case BLOCK_END:
                        {
                            Restore();
                            m_restoreVector.pop_back();
                        }
                        break;

                    case SAVE:
                        assert( size == 3 );
                        {
                            int flag = (*++it).m_int;
                            Save( flag );
                        }
                        break;

                    case SAVE_LIGHT:
                        assert( size == 4 );
                        {
                            int light = (*++it).m_int;
                            int flag = (*++it).m_int;
                            SaveLight( light, flag );
                        }
                        break;

                    case SAVE_MATERIAL:
                        assert( size == 4 );
                        {
                            int face = (*++it).m_int;
                            int flag = (*++it).m_int;
                            SaveMaterial( face, flag );
                        }
                        break;

                        // State
                    case ENABLE:
                        assert( size == 3 );
                        {
                            int flag = (*++it).m_int;
                            glEnable( ToGLFlag( flag ));
                        }
                        break;

                    case DISABLE:
                        assert( size == 3 );
                        {
                            int flag = (*++it).m_int;
                            glDisable( ToGLFlag( flag ));
                        }
                        break;

                    case BLEND_FUNC:
                        assert( size == 4 );
                        {
                            int sf = (*++it).m_int;
                            int df = (*++it).m_int;
                            glBlendFunc( ToGLBlendFun( sf ), ToGLBlendFun( df ));
                        }
                        break;

                    case BIND_TEXTURE:
                        assert( size == 4 );
                        {
                            int target = ToGLFlag( (*++it).m_int );
                            int index = (*++it).m_int;
                            glBindTexture( target, index );
                        }
                        break;

                    case MATERIAL_PROPERTY_FLOAT_4:
                        assert ( size == 8 );
                        {
                            int face = ToGLFlag( (*++it).m_int );
                            int prop = ToGLMaterial( (*++it).m_int );
                            float color[4];
                            for ( int i=0; i<4; ++i ) { color[i] = (*++it).m_float; }
                            glMaterialfv( face, prop, color );
                        }
                        break;

                    case MATERIAL_PROPERTY_FLOAT:
                        assert( size == 5 );
                        {
                            int face = ToGLFlag( (*++it).m_int );
                            int prop = ToGLMaterial( (*++it).m_int );
                            float x = (*++it).m_float;
                            glMaterialf( face, prop, x );
                        }
                        break;

                    case LIGHT_PROPERTY_FLOAT_4:
                        assert ( size == 8 );
                        {
                            int light = ToGLFlag( (*++it).m_int );
                            int prop = ToGLLight( (*++it).m_int );
                            float color[4];
                            for ( int i=0; i<4; ++i ) { color[i] = (*++it).m_float; }
                            glLightfv( light, prop, color );
                        }
                        break;

                    case LIGHT_PROPERTY_FLOAT_3:
                        assert ( size == 7 );
                        {
                            int light = ToGLFlag( (*++it).m_int );
                            int prop = ToGLLight( (*++it).m_int );
                            float direction[3];
                            for ( int i=0; i<3; ++i ) { direction[i] = (*++it).m_float; }
                            glLightfv( light, prop, direction );
                        }
                        break;

                    case LIGHT_PROPERTY_FLOAT:
                        assert( size == 5 );
                        {
                            int light = ToGLFlag( (*++it).m_int );
                            int prop = ToGLLight( (*++it).m_int );
                            float x = (*++it).m_float;
                            glLightf( light, prop, x );
                        }
                        break;

                    case LIGHT_MODEL_PROPERTY_FLOAT_4:
                        assert( size == 7 );
                        {
                            int prop = (*++it).m_int;
                            float color[4];
                            for ( int i=0; i<4; ++i ) { color[i] = (*++it).m_float; }
                            glLightModelfv( prop, color );
                        }
                        break;

                    case LIGHT_MODEL_PROPERTY_INT:
                        assert( size == 4 );
                        {
                            int prop = ToGLLightModel( (*++it).m_int );
                            int x = (*++it).m_int;
                            glLightModeli( prop, x );
                        }
                        break;

                    case COLOR_MATERIAL_MODE:
                        assert( size == 4 );
                        {
                            int face = ToGLFlag( (*++it).m_int );
                            int mode = ToGLMaterial( (*++it).m_int );
                            glColorMaterial( face, mode );
                        }
                        break;

                    case BEGIN:
                        assert( size == 3 );
                        {
                            int type = (*++it).m_int;
                            glBegin( ToGLPrimitive( type ));
                        }
                        break;

                    case END:
                        assert( size == 2 );
                        glEnd();
                        break;

                    case VERTEX:
                        assert( size == 5 );
                        {
                            float x = (*++it).m_float;
                            float y = (*++it).m_float;
                            float z = (*++it).m_float;
                            glVertex3f(x,y,z);
                        }
                        break;

                    case NORMAL:
                        assert( size == 5 );
                        {
                            float x = (*++it).m_float;
                            float y = (*++it).m_float;
                            float z = (*++it).m_float;
                            glNormal3f(x,y,z);
                        }
                        break;

                    case COLOR:
                        assert( size == 6 );
                        {
                            float r = (*++it).m_float;
                            float g = (*++it).m_float;
                            float b = (*++it).m_float;
                            float a = (*++it).m_float;
                            glColor4f(r,g,b,a);
                        }
                        break;

                    case TEXTURE_COORD:
                        assert( size == 4 );
                        {
                            float u = (*++it).m_float;
                            float v = (*++it).m_float;
                            glTexCoord2f(u,v);
                        }
                        break;

                        // Client State
                    case ENABLE_CLIENT_STATE:
                        assert( size == 3 );
                        {
                            int flag = (*++it).m_int;
                            if ( flag != INDEX_ARRAY )
                            {
                                glEnableClientState( ToGLClientFlag( flag ));
                            }
                        }
                        break;

                    case DISABLE_CLIENT_STATE:
                        assert( size == 3 );
                        {
                            int flag = (*++it).m_int;
                            if ( flag != INDEX_ARRAY )
                            {
                                glDisableClientState( ToGLClientFlag( flag ));
                            }
                        }
                        break;

                    case VERTEX_ARRAY:
                        assert( size == 4 );
                        {
                            void *ptr = (*++it).m_pointer;
                            unsigned int s = (*++it).m_uint;
                            glVertexPointer( 3, GL_FLOAT, sizeof( GAL::P3f ), ptr );
                        }
                        break;

                    case NORMAL_ARRAY:
                        assert( size == 4 );
                        {
                            void *ptr = (*++it).m_pointer;
                            unsigned int s = (*++it).m_uint;
                            glNormalPointer( GL_FLOAT, sizeof( GAL::P3f ), ptr );
                        }
                        break;

                    case COLOR_ARRAY:
                        assert( size == 4 );
                        {
                            void *ptr = (*++it).m_pointer;
                            unsigned int s = (*++it).m_uint;
                            glColorPointer( 4, GL_FLOAT, sizeof( GAL::P4f ), ptr );
                        }
                        break;

                    case TEXTURE_COORD_ARRAY:
                        assert( size == 4 );
                        {
                            void *ptr = (*++it).m_pointer;
                            unsigned int s = (*++it).m_uint;
                            glTexCoordPointer( 2, GL_FLOAT, sizeof( GAL::P2f ), ptr );
                        }
                        break;

                    case INDEX_ARRAY:
                        assert( size == 4 );
                        {
                            void *ptr = (*++it).m_pointer;
                            unsigned int s = (*++it).m_uint;
                            m_indexPointer = reinterpret_cast< unsigned int *>( ptr );
                            m_numIndices = s;
                        }
                        break;

                    case DRAW_ELEMENTS:
                        assert( size == 5 );
                        {
                            int type = (*++it).m_int;
                            unsigned int first = (*++it).m_int;
                            unsigned int count = (*++it).m_int;
                            glDrawElements( ToGLPrimitive( type ), count, GL_UNSIGNED_INT, m_indexPointer + first );
                        }
                        break;

                    case MESSAGE_CB:
                        {
                            int cbOffset = (*++it).m_int;                            
                            char *cbPtr = reinterpret_cast< char *>( const_cast< int * >( &((*++it).m_int )));
                            cbPtr += cbOffset;
                            MessageCallback *pCB = reinterpret_cast< MessageCallback * >( cbPtr );
                            Invoke( pCB->Apply());
                            std::advance( it, size-4 );
                        }
                        break;

                    default:
                        std::advance( it, size-2 );
                        break;
                };
            }
        }

    };// namespace GVM
};// namespace Useless
