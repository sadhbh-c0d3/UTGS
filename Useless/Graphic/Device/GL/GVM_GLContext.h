#ifndef __INCLUDED_GVM_GL_GRAPHIC_DEVICE_H__
#define __INCLUDED_GVM_GL_GRAPHIC_DEVICE_H__

#include "Useless/Graphic/Device/GL/GLResourcePool.h"
#include "Useless/Graphic/Device/GVM_Context.h"
#include "Useless/Graphic/Device/GVM_Message.h"

namespace Useless {

    namespace GVM {

        struct USELESS_API GLContext : Context
        {
            unsigned int *m_indexPointer;
            unsigned int  m_numIndices;

            std::vector< Message > m_restoreVector;

            GLContext();
            ~GLContext();

            Texture * CreateTexture( int width, int height, int color );

            void GetViewport ( int *rect );
            void SetViewport ( int x, int y, int w, int h );
            void InitViewport( int x, int y, int w, int h );

            void SetOrtho2D    ( double left, double right, double bottom, double top );
            void SetPerspective( double fov, double aspect, double znear, double zfar );
            
            void SetFrustum    ( double left, double right, double bottom, double top, double znear, double zfar );
            void SetOrtho      ( double left, double right, double bottom, double top, double znear, double zfar );

            void Save( int flag );
            void SaveLight( int light, int flag );
            void SaveMaterial( int face, int flag );
            void Restore();

            void Invoke( const Message &msg );

            static int BadEnumGL( int flag );
            static int BadEnumGVM( int flag );
            static int ToGLFlag( int gvm );
            static int ToGLClientFlag( int gvm );
            static int ToGLPrimitive( int gvm );
            static int ToGLBlendFun( int gvm );
            static int ToGLMaterial( int gvm );
            static int ToGLLight( int gvm );
            static int ToGLLightModel( int gvm );
            static int FromGLBlendFun( int gl );
            static int FromGLFace( int gl );
            static int FromGLMaterial( int gl );
        };//GLGraphicDevice

    };// namespace GVM
};// namespace Useless
#endif//__INCLUDED_GVM_GL_GRAPHIC_DEVICE_H__
