#ifndef __INCLUDED_GVM_MESSAGE_H__
#define __INCLUDED_GVM_MESSAGE_H__

#include <assert.h>
#include <vector>
#include <iterator>
#include "Useless/Graphic/Linear.h"

namespace GVM {

    struct Message
    {
        union data_t
        {
            unsigned int m_uint;
            int     m_int;
            float   m_float;
            void   *m_pointer;
            data_t(): m_int(0) {}
            data_t( unsigned int x ): m_uint(x) {}
            data_t( int x ): m_int(x) {}
            data_t( float x ): m_float(x) {}
            data_t( void *x ): m_pointer(x) {}
        };
        typedef std::vector< data_t > message_t;
        message_t m_message;
        Message & operator += ( const Message &msg )
        {
            std::copy( msg.m_message.begin(), msg.m_message.end(), std::back_inserter( m_message ));
            return (*this);
        }
        Message & operator << ( const data_t &x )
        {
            m_message.push_back( x );
            return (*this);
        }
    };

    Message operator + ( Message msg1, const Message &msg2 )
    {
        return (msg1 += msg2 );
    }

    enum Enum {
        // Transformation
        CURRENT_MATRIX,
        PUSH_MATRIX,
        POP_MATRIX,
        LOAD_IDENTITY,
        LOAD_MATRIX,
        MULT_MATRIX,
        TRANSLATE,
        ROTATE,
        SCALE,
        // Making Blocks (state saving/restoring)
        BLOCK_BEGIN,
        BLOCK_END,
        SAVE,
        SAVE_LIGHT,
        SAVE_MATERIAL,
        // State
        ENABLE,
        DISABLE,
        BLEND_FUNC,
        BEGIN,
        END,
        VERTEX,
        COLOR,
        NORMAL,
        TEXTURE_COORD,
        // Lighting
        MATERIAL_PROPERTY_FLOAT_4,
        MATERIAL_PROPERTY_FLOAT,
        LIGHT_PROPERTY_FLOAT_4,
        LIGHT_PROPERTY_FLOAT_3,
        LIGHT_PROPERTY_FLOAT,
        LIGHT_MODEL_PROPERTY_FLOAT_4,
        LIGHT_MODEL_PROPERTY_INT,
        COLOR_MATERIAL_MODE,
        BACK,
        FRONT,
        FRONT_AND_BACK,
        AMBIENT,
        DIFFUSE,
        AMBIENT_AND_DIFFUSE,
        SPECULAR,
        EMISSION,
        SHININESS,
        POSITION,
        SPOT_DIRECTION,
        SPOT_EXPONENT,
        SPOT_CUTOFF,
        CONSTANT_ATTENUATION,
        LINEAR_ATTENUATION,
        QUADRATIC_ATTENUATION,
        LIGHT_MODEL_LOCAL_VIEWER,
        LIGHT_MODEL_TWO_SIDE,
        LIGHT_MODEL_AMBIENT,
        SINGLE_COLOR,
        // Flags
        BLEND,
        DEPTH_TEST,
        COLOR_MATERIAL,
        LIGHTING,
        LIGHT0,
        LIGHT1,
        LIGHT2,
        LIGHT3,
        LIGHT4,
        LIGHT5,
        LIGHT6,
        LIGHT7,
        // Client State
        ENABLE_CLIENT_STATE,
        DISABLE_CLIENT_STATE,
        VERTEX_ARRAY,
        NORMAL_ARRAY,
        COLOR_ARRAY,
        TEXTURE_COORD_ARRAY,
        INDEX_ARRAY,
        DRAW_ELEMENTS,
        // Primitive Types
        LINES,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        // Blending functions
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        ONE,
    };


    // Transformation Messages
    
    Message PushMatrixMsg()
    {
        return Message() << 2 << PUSH_MATRIX;
    }

    Message PopMatrixMsg()
    {
        return Message() << 2 << POP_MATRIX;
    }

    Message LoadIdentityMsg()
    {
        return Message() << 2 << LOAD_IDENTITY;
    }

    Message LoadMatrixMsg( const GAL::M4f &matrix )
    {
        return Message() << 18 << LOAD_MATRIX <<
            matrix[0][0] << matrix[0][1] << matrix[0][2] << matrix[0][3] <<
            matrix[1][0] << matrix[1][1] << matrix[1][2] << matrix[1][3] <<
            matrix[2][0] << matrix[2][1] << matrix[2][2] << matrix[2][3] <<
            matrix[3][0] << matrix[3][1] << matrix[3][2] << matrix[3][3];
    }

    Message MulMatrixMsg( const GAL::M4f &matrix )
    {
        return Message() << 18 << MULT_MATRIX <<
            matrix[0][0] << matrix[0][1] << matrix[0][2] << matrix[0][3] <<
            matrix[1][0] << matrix[1][1] << matrix[1][2] << matrix[1][3] <<
            matrix[2][0] << matrix[2][1] << matrix[2][2] << matrix[2][3] <<
            matrix[3][0] << matrix[3][1] << matrix[3][2] << matrix[3][3];
    }

    Message TranslateMsg( const GAL::P3f &vector )
    {
        return Message() << 5 << TRANSLATE << vector[0]  << vector[1] << vector[2];
    }

    Message RotateMsg( float angle, const GAL::P3f &vector )
    {
        return Message() << 6 << ROTATE << angle << vector[0] << vector[1] << vector[2];
    }
    
    Message ScaleMsg( const GAL::P3f &vector )
    {
        return Message() << 5 << SCALE << vector[0]  << vector[1] << vector[2];
    }

    // Block Making Messages

    Message BlockBeginMsg()
    {
        return Message() << 2 << BLOCK_BEGIN;
    }

    Message BlockEndMsg()
    {
        return Message() << 2 << BLOCK_END;
    }

    Message BlockMsg( const Message &msg )
    {
        return BlockBeginMsg() + msg + BlockEndMsg();
    }

    Message SaveMsg( int flag )
    {
        return Message() << 3 << SAVE << flag;
    }

    Message SaveLightMsg( int light, int flag )
    {
        return Message() << 4 << SAVE_LIGHT << light << flag;
    }

    Message SaveMaterialMsg( int material, int flag )
    {
        return Message() << 4 << SAVE_MATERIAL << material << flag;
    }

    // State Messages

    Message EnableMsg( int flag )
    {
        return Message() << 3 << ENABLE << flag ;
    }

    Message DisableMsg( int flag )
    {
        return Message() << 3 << DISABLE << flag ;
    }

    Message BlendFuncMsg( int sf, int df )
    {
        return Message() << 4 << BLEND_FUNC << sf << df;
    }

    Message BeginMsg( int type )
    {
        return Message() << 3 << BEGIN << type;
    }

    Message EndMsg()
    {
        return Message() << 2 << END;
    }

    Message ShapeMsg( int type, const Message &msg )
    {
        return BeginMsg( type ) + msg + EndMsg();
    }

    Message VertexMsg( GAL::P3f vertex )
    {
        return Message() << 5 << VERTEX << vertex[0] << vertex[1] << vertex[2]; 
    }
    
    Message NormalMsg( GAL::P3f normal )
    {
        return Message() << 5 << NORMAL << normal[0] << normal[1] << normal[2]; 
    }
    
    Message ColorMsg( GAL::P4f color )
    {
        return Message() << 6 << COLOR << color[0] << color[1] << color[2] << color[3];
    }

    Message TexCoordMsg( GAL::P2f texc )
    {
        return Message() << 4 << TEXTURE_COORD << texc[0] << texc[1];
    }

    Message MaterialMsg( int face, int property, GAL::P4f color )
    {
        return Message() << 8 << MATERIAL_PROPERTY_FLOAT_4 << face << property << color[0] << color[1] << color[2] << color[3];
    }

    Message MaterialMsg( int light, int property, float value )
    {
        return Message() << 5 << MATERIAL_PROPERTY_FLOAT << light << property << value;
    }

    Message LightMsg( int light, int property, GAL::P4f color )
    {
        return Message() << 8 << LIGHT_PROPERTY_FLOAT_4 << light << property << color[0] << color[1] << color[2] << color[3];
    }
    
    Message LightMsg( int light, int property, GAL::P3f vect )
    {
        return Message() << 7 << LIGHT_PROPERTY_FLOAT_3 << light << property << vect[0] << vect[1] << vect[2];
    }

    Message LightMsg( int light, int property, float value )
    {
        return Message() << 5 << LIGHT_PROPERTY_FLOAT << light << property << value;
    }

    Message LightModelMsg( int property, GAL::P4f color )
    {
        return Message() << 7 << LIGHT_MODEL_PROPERTY_FLOAT_4 << property << color[0] << color[1] << color[2] << color[3];
    }

    Message LightModelMsg( int property, int value )
    {
        return Message() << 4 << LIGHT_MODEL_PROPERTY_INT << property << value;
    }

    Message ColorMaterialMsg( int face, int mode )
    {
        return Message() << 4 << COLOR_MATERIAL_MODE << face << mode;
    }



    // Client State Messages

    Message EnableClientStateMsg( int flag )
    {
        return Message() << 3 << ENABLE_CLIENT_STATE  << flag;
    }

    Message DisableClientStateMsg( int flag )
    {
        return Message() << 3 << DISABLE_CLIENT_STATE << flag;
    }

    Message VertexArrayMsg( GAL::P3f *pointer, unsigned int size )
    {
        return Message() << 4 << VERTEX_ARRAY << pointer << size;
    }

    Message NormalArrayMsg( GAL::P3f *pointer, unsigned int size )
    {
        return Message() << 4 << NORMAL_ARRAY << pointer << size;
    }

    Message ColorArrayMsg( GAL::P4f *pointer, unsigned int size )
    {
        return Message() << 4 << COLOR_ARRAY << pointer << size;
    }

    Message TexCoordArrayMsg( GAL::P2f *pointer, unsigned int size )
    {
        return Message() << 4 << TEXTURE_COORD_ARRAY << pointer << size;
    }

    Message IndexArrayMsg( unsigned int *pointer, unsigned int size )
    {
        return Message() << 4 << INDEX_ARRAY << pointer << size;
    }

    Message DrawElementsMsg( int type, unsigned int first, unsigned int count )
    {
        return Message() << 5 << DRAW_ELEMENTS << type << first << count;
    }

    struct IGraphicDevice
    {
        IGraphicDevice()
        {
        }
        
        virtual ~IGraphicDevice()
        {
        }

        // Window properites
        virtual void GetViewport ( int *rect ) = 0;
        virtual void InitViewport( int x, int y, int w, int h ) = 0;
        virtual void SetViewport ( int x, int y, int w, int h ) = 0;

        // Camera properties
        virtual void SetOrtho2D( float left, float right, float bottom, float top ) = 0; 
        virtual void SetPerspective( float fov, float aspect, float znear, float zfar ) = 0;
        
        // Dispatch message
        virtual void Invoke( const Message &msg ) = 0;
    };
    
};// namespace GVM
#endif//__INCLUDED_GVM_MESSAGE_H__
