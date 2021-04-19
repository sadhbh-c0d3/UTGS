#ifndef __INCLUDED_GVM_MESSAGE_H__
#define __INCLUDED_GVM_MESSAGE_H__

#include <assert.h>
#include <vector>
#include "Useless/Graphic/Linear.h"

namespace Useless {

    namespace GVM {

        struct USELESS_API Message
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
            inline Message & operator += ( const Message &msg )
            {
                std::copy( msg.m_message.begin(), msg.m_message.end(), std::back_inserter( m_message ));
                return (*this);
            }
            inline Message & operator << ( const data_t &x )
            {
                m_message.push_back( x );
                return (*this);
            }
        };

        inline Message operator + ( Message msg1, const Message &msg2 )
        {
            return (msg1 += msg2 );
        }

        struct MessageCallback
        {
            virtual inline Message Apply() = 0;
        };

        enum Enum {
            // Transformation
            MATRIX_MODE,
            PROJECTION,
            MODELVIEW,
            TEXTURE,
            /*COLOR,*/
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
            BIND_TEXTURE,
            BEGIN,
            END,
            VERTEX,
            COLOR,
            NORMAL,
            TEXTURE_COORD,
            TEXTURE_2D,
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
            LINE_LOOP,
            TRIANGLES,
            TRIANGLE_STRIP,
            // Blending functions
            SRC_ALPHA,
            ONE_MINUS_SRC_ALPHA,
            ONE,
            // Callbacks
            MESSAGE_CB
        };


        // Transformation Messages
        inline Message MatrixModeMsg( int mode )
        {
            return Message() << 3 << MATRIX_MODE << mode;
        }

        inline Message PushMatrixMsg()
        {
            return Message() << 2 << PUSH_MATRIX;
        }

        inline Message PopMatrixMsg()
        {
            return Message() << 2 << POP_MATRIX;
        }

        inline Message LoadIdentityMsg()
        {
            return Message() << 2 << LOAD_IDENTITY;
        }

        inline Message LoadMatrixMsg( const GAL::M4f &matrix )
        {
            return Message() << 18 << LOAD_MATRIX <<
                matrix[0][0] << matrix[0][1] << matrix[0][2] << matrix[0][3] <<
                matrix[1][0] << matrix[1][1] << matrix[1][2] << matrix[1][3] <<
                matrix[2][0] << matrix[2][1] << matrix[2][2] << matrix[2][3] <<
                matrix[3][0] << matrix[3][1] << matrix[3][2] << matrix[3][3];
        }

        inline Message MulMatrixMsg( const GAL::M4f &matrix )
        {
            return Message() << 18 << MULT_MATRIX <<
                matrix[0][0] << matrix[0][1] << matrix[0][2] << matrix[0][3] <<
                matrix[1][0] << matrix[1][1] << matrix[1][2] << matrix[1][3] <<
                matrix[2][0] << matrix[2][1] << matrix[2][2] << matrix[2][3] <<
                matrix[3][0] << matrix[3][1] << matrix[3][2] << matrix[3][3];
        }

        inline Message TranslateMsg( const GAL::P3f &vector )
        {
            return Message() << 5 << TRANSLATE << vector[0]  << vector[1] << vector[2];
        }

        inline Message RotateMsg( float angle, const GAL::P3f &vector )
        {
            return Message() << 6 << ROTATE << angle << vector[0] << vector[1] << vector[2];
        }

        inline Message ScaleMsg( const GAL::P3f &vector )
        {
            return Message() << 5 << SCALE << vector[0]  << vector[1] << vector[2];
        }

        // Block Making Messages

        inline Message BlockBeginMsg()
        {
            return Message() << 2 << BLOCK_BEGIN;
        }

        inline Message BlockEndMsg()
        {
            return Message() << 2 << BLOCK_END;
        }

        inline Message BlockMsg( const Message &msg )
        {
            return BlockBeginMsg() + msg + BlockEndMsg();
        }

        inline Message SaveMsg( int flag )
        {
            return Message() << 3 << SAVE << flag;
        }

        inline Message SaveLightMsg( int light, int flag )
        {
            return Message() << 4 << SAVE_LIGHT << light << flag;
        }

        inline Message SaveMaterialMsg( int material, int flag )
        {
            return Message() << 4 << SAVE_MATERIAL << material << flag;
        }

        // State Messages

        inline Message EnableMsg( int flag )
        {
            return Message() << 3 << ENABLE << flag ;
        }

        inline Message DisableMsg( int flag )
        {
            return Message() << 3 << DISABLE << flag ;
        }

        inline Message BlendFuncMsg( int sf, int df )
        {
            return Message() << 4 << BLEND_FUNC << sf << df;
        }

        inline Message BindTextureMsg( int target, int index )
        {
            return Message() << 4 << BIND_TEXTURE << target << index;
        }

        inline Message BeginMsg( int type )
        {
            return Message() << 3 << BEGIN << type;
        }

        inline Message EndMsg()
        {
            return Message() << 2 << END;
        }

        inline Message ShapeMsg( int type, const Message &msg )
        {
            return BeginMsg( type ) + msg + EndMsg();
        }

        inline Message VertexMsg( GAL::P3f vertex )
        {
            return Message() << 5 << VERTEX << vertex[0] << vertex[1] << vertex[2]; 
        }

        inline Message NormalMsg( GAL::P3f normal )
        {
            return Message() << 5 << NORMAL << normal[0] << normal[1] << normal[2]; 
        }

        inline Message ColorMsg( GAL::P4f color )
        {
            return Message() << 6 << COLOR << color[0] << color[1] << color[2] << color[3];
        }

        inline Message TexCoordMsg( GAL::P2f texc )
        {
            return Message() << 4 << TEXTURE_COORD << texc[0] << texc[1];
        }

        inline Message MaterialMsg( int face, int property, GAL::P4f color )
        {
            return Message() << 8 << MATERIAL_PROPERTY_FLOAT_4 << face << property << color[0] << color[1] << color[2] << color[3];
        }

        inline Message MaterialMsg( int light, int property, float value )
        {
            return Message() << 5 << MATERIAL_PROPERTY_FLOAT << light << property << value;
        }

        inline Message LightMsg( int light, int property, GAL::P4f color )
        {
            return Message() << 8 << LIGHT_PROPERTY_FLOAT_4 << light << property << color[0] << color[1] << color[2] << color[3];
        }

        inline Message LightMsg( int light, int property, GAL::P3f vect )
        {
            return Message() << 7 << LIGHT_PROPERTY_FLOAT_3 << light << property << vect[0] << vect[1] << vect[2];
        }

        inline Message LightMsg( int light, int property, float value )
        {
            return Message() << 5 << LIGHT_PROPERTY_FLOAT << light << property << value;
        }

        inline Message LightModelMsg( int property, GAL::P4f color )
        {
            return Message() << 7 << LIGHT_MODEL_PROPERTY_FLOAT_4 << property << color[0] << color[1] << color[2] << color[3];
        }

        inline Message LightModelMsg( int property, int value )
        {
            return Message() << 4 << LIGHT_MODEL_PROPERTY_INT << property << value;
        }

        inline Message ColorMaterialMsg( int face, int mode )
        {
            return Message() << 4 << COLOR_MATERIAL_MODE << face << mode;
        }



        // Client State Messages

        inline Message EnableClientStateMsg( int flag )
        {
            return Message() << 3 << ENABLE_CLIENT_STATE  << flag;
        }

        inline Message DisableClientStateMsg( int flag )
        {
            return Message() << 3 << DISABLE_CLIENT_STATE << flag;
        }

        inline Message VertexArrayMsg( GAL::P3f *pointer, unsigned int size )
        {
            return Message() << 4 << VERTEX_ARRAY << pointer << size;
        }

        inline Message NormalArrayMsg( GAL::P3f *pointer, unsigned int size )
        {
            return Message() << 4 << NORMAL_ARRAY << pointer << size;
        }

        inline Message ColorArrayMsg( GAL::P4f *pointer, unsigned int size )
        {
            return Message() << 4 << COLOR_ARRAY << pointer << size;
        }

        inline Message TexCoordArrayMsg( GAL::P2f *pointer, unsigned int size )
        {
            return Message() << 4 << TEXTURE_COORD_ARRAY << pointer << size;
        }

        inline Message IndexArrayMsg( unsigned int *pointer, unsigned int size )
        {
            return Message() << 4 << INDEX_ARRAY << pointer << size;
        }

        inline Message DrawElementsMsg( int type, unsigned int first, unsigned int count )
        {
            return Message() << 5 << DRAW_ELEMENTS << type << first << count;
        }

        template< class _CBType > inline Message CallbackMsg( const _CBType &cb )
        {
            // chack if cb is valid <MessageCallback> type
            const MessageCallback &is_message_callback = cb;
            // calculate offset between <_CBType *> and <MessageCallback *> pointers
            const char *vpMCB = reinterpret_cast< const char *>( &is_message_callback );
            const char *vpCCB = reinterpret_cast< const char *>( &cb );
            int mcbOffset = vpCCB - vpMCB;
            // calculate number of <Message::data_t> slices
            size_t cbSize = sizeof( cb );
            size_t numSlices = cbSize / sizeof( inline Message::data_t ) + (0 != cbSize % sizeof( Message::data_t ));
            // create message header
            Message msg;
            msg << 3 + numSlices;
            msg << MESSAGE_CB;
            msg << mcbOffset;
            // create message with data
            Message m1;
            m1.m_message.resize( numSlices );
            std::memcpy( &m1.m_message[0], &cb, cbSize );
            // return message = header + data
            return msg + m1;
        }

    };// namespace GVM
};// namespace Useless
#endif//__INCLUDED_GVM_MESSAGE_H__
