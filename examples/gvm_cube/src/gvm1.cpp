#include "utgs-config.h"
#include "GVM.h"
#include "GLGraphicDevice.h"
#include "Dynamo/interfaces.h"
#include "Dynamo/hook_utils.h"
#include "Dynamo/classes/CInterface.h"
#include "Useless/Graphic/Linear.h"
#include <cmath>


using namespace Dynamo;
using namespace GAL;
using namespace GVM;



struct GLGraphicDeviceUser : GVM::GLGraphicDevice
{
    int m_viewportOld[4];

    GLGraphicDeviceUser( Hand< IWidget > desktop, Hand< IWidget > widget )
    {
        GetViewport( m_viewportOld );
        Rect4i rcDesktop = desktop->GetRect();
        Rect4i rcArea = widget->GetRect();
        Rect4i arArea = widget->GetAbsoluteRect( rcArea );
        InitViewport( arArea.m_x, rcDesktop.m_h-arArea.m_h-arArea.m_y, arArea.m_w, arArea.m_h );
    }

    ~GLGraphicDeviceUser()
    {
        SetViewport( m_viewportOld[0], m_viewportOld[1], m_viewportOld[2], m_viewportOld[3] );
    }
};

struct GraphicVirtualMachine1 : CInterface
{
    Hand< IInterfaceProvider >  m_provider;
    Hand< IGui >    m_gui;
    Hand< IWidget > m_area1;
    Hand< IWidget > m_area2;
    Hand< IWidget > m_desktop;
    float m_angle1;
    float m_angle2;
    float m_camAngle;
    float m_camFov;
    P3f   m_vertices[8];
    P3f   m_normals[8];
    P4f   m_colors[8];
    //unsigned int m_indices[24];
    P4u   m_faces[6];
    Message m_renderMsg;

    GraphicVirtualMachine1( IInterfaceProvider *provider, IXmlProgram *prog, IXmlScope *scope )
    {
        m_provider  =   provider;
       
        /*
        m_graphics  = m_provider->ProvideIGraphics();
        Hand< IGraphicPlane > image1 = m_graphics->GetImage("image1");
        Hand< IPixelReader > rdr1 = image1->ProvideIPixelReader();
        void *data = new char[ image1->GetWidth() * image1->GetHeight() * 4 ];
        rdr1->ReadPixels( data, 32, 4 * image1->GetWidth(), Rect4i(0,0, image1->GetWidth(), image1->GetHeight()), ColorMode::RGBA );

        struct GLPixelWriter : IPixelWriter
        {
            void WritePixels( void *data, int bits, int pitch, Rect4i area )
            {
                TexSubImage2D( ... data);
            }
            
        } wrt;
        m_graphics->LoadImage("image1.tga", &wrt );
        */
                
        m_gui   =   m_provider->ProvideIGui();
        m_desktop = m_gui->GetWidget("gui.desktop");
        m_area1 =   m_gui->GetWidget("gui.desktop.minimap");
        m_area2 =   m_gui->GetWidget("gui.desktop.area");
        m_area1 ->  SetHook_Paint( make_hook_mp< IHook_Paint, IPaint * >( this, &GraphicVirtualMachine1::PaintArea1 ));
        m_area2 ->  SetHook_Paint( make_hook_mp< IHook_Paint, IPaint * >( this, &GraphicVirtualMachine1::PaintArea2 ));
        scope   ->  AddMethod("Tick", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::Tick ));
        scope   ->  AddMethod("ChangeCameraAngle", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::ChangeCameraAngle ));
        scope   ->  AddMethod("ChangeCameraFOV", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::ChangeCameraFOV ));
        m_angle1 = 0;
        m_angle2 = 0;
        m_camAngle = 35.0;
        m_camFov = 57.0;

        m_vertices[0] = P3f(0,0,0);
        m_vertices[1] = P3f(0,0,1);
        m_vertices[2] = P3f(1,0,1);
        m_vertices[3] = P3f(1,0,0);
        m_vertices[4] = P3f(0,1,0);
        m_vertices[5] = P3f(0,1,1);
        m_vertices[6] = P3f(1,1,1);
        m_vertices[7] = P3f(1,1,0);
        
        m_normals[0] = P3f(-0.77,-0.77,-0.77);
        m_normals[1] = P3f(-0.77,-0.77,0.77);
        m_normals[2] = P3f(0.77,-0.77,0.77);
        m_normals[3] = P3f(0.77,-0.77,-0.77);
        m_normals[4] = P3f(-0.77,0.77,-0.77);
        m_normals[5] = P3f(-0.77,0.77,0.77);
        m_normals[6] = P3f(0.77,0.77,0.77);
        m_normals[7] = P3f(0.77,0.77,-0.77);

        m_colors[0] = P4f(0,0,1,1);
        m_colors[1] = P4f(0,0,1,1);
        m_colors[2] = P4f(0,0,1,1);
        m_colors[3] = P4f(0,0,1,1);
        m_colors[4] = P4f(0,1,1,1);
        m_colors[5] = P4f(0,1,1,1);
        m_colors[6] = P4f(0,1,1,1);
        m_colors[7] = P4f(0,1,1,1);

        m_faces[0] = P4u(0,1,3,2);
        m_faces[1] = P4u(4,5,7,6);
        m_faces[2] = P4u(0,4,1,5);
        m_faces[3] = P4u(1,5,2,6);
        m_faces[4] = P4u(2,6,3,7);
        m_faces[5] = P4u(3,7,0,4);


        m_renderMsg += BlockMsg(
                SaveMsg( CURRENT_MATRIX ) +
                ScaleMsg( P3f( 0.5, 0.5, 0.5 )) +
                TranslateMsg( P3f(-0.5, -0.5, -0.5) ) +
                
                VertexArrayMsg( m_vertices, 8 ) +
                EnableClientStateMsg( VERTEX_ARRAY ) +

                NormalArrayMsg( m_normals, 8 ) +
                EnableClientStateMsg( NORMAL_ARRAY ) +

                IndexArrayMsg( m_faces[0], 24 ) +
                EnableClientStateMsg( INDEX_ARRAY ) +


                EnableMsg( COLOR_MATERIAL ) +
                MaterialMsg( FRONT_AND_BACK, SHININESS, 0.0 ) +

                BlockMsg(
                    SaveMsg( COLOR_ARRAY ) +
                    SaveMsg( COLOR_MATERIAL_MODE ) +
                    SaveMaterialMsg( FRONT_AND_BACK, SHININESS ) +

                    ColorArrayMsg( m_colors, 8 ) +
                    EnableClientStateMsg( COLOR_ARRAY ) +

                    //ColorMaterialMsg( FRONT_AND_BACK, DIFFUSE ) +
                    MaterialMsg( FRONT_AND_BACK, SHININESS, 0.1 ) +

                    DrawElementsMsg( TRIANGLE_STRIP, 0, 4 ) +
                    DrawElementsMsg( TRIANGLE_STRIP, 4, 4 )
                    ) +

                BlockMsg(
                    SaveMsg( BLEND ) + SaveMsg( BLEND_FUNC ) +
                    EnableMsg( BLEND ) + BlendFuncMsg( ONE, ONE_MINUS_SRC_ALPHA ) +
                    
                    ColorMsg( P4f( 1.0, 0.8, 0.0, 0.5 )) +
                    DrawElementsMsg( TRIANGLE_STRIP, 8, 4 ) +

                    ColorMsg( P4f( 1.0, 0.0, 0.8, 0.5 )) +
                    DrawElementsMsg( TRIANGLE_STRIP, 12, 4 ) +

                    ColorMsg( P4f( 0.0, 1.0, 0.8, 0.5 )) +
                    DrawElementsMsg( TRIANGLE_STRIP, 16, 4 ) +

                    ColorMsg( P4f( 0.8, 0.0, 1.0, 0.5 )) +
                    DrawElementsMsg( TRIANGLE_STRIP, 20, 4 )
                    )
                );
    }

    ~GraphicVirtualMachine1()
    {
    }

    void PaintArea1( IPaint *paint )
    {
        GLGraphicDeviceUser gl( m_desktop, m_area1 );
        Rect4i rcArea1 = m_area1->GetRect();
        double aspect = ((float)rcArea1.m_w)/(float)rcArea1.m_h;
        gl.SetOrtho2D( -aspect, aspect, -1.0, 1.0 );
        gl.Invoke(
                RotateMsg( m_angle1, P3f(0,0,1)) +
                ScaleMsg( P3f(0.5, 0.5, 0.5)) +
                TranslateMsg( P3f(-1,-1,0) ) +
                ShapeMsg( TRIANGLE_STRIP, ColorMsg( P4f(1,1,0,1)) +
                    VertexMsg( P3f(0,0,0) ) +
                    VertexMsg( P3f(0,2,0) ) +
                    VertexMsg( P3f(2,0,0) ) +
                    VertexMsg( P3f(2,2,0) )
                    ));
        m_area1->SetDirty( m_area1->GetRect() );
    }

    void PaintArea2( IPaint *paint )
    {
        GLGraphicDeviceUser gl( m_desktop, m_area2 );
        Rect4i rcArea2 = m_area2->GetRect();
        gl.SetPerspective( m_camFov, ((float)rcArea2.m_w)/(float)rcArea2.m_h, 0.01, 1000.0);
        gl.Invoke(
                LightMsg( LIGHT0, POSITION, P4f(0,0,0,1)) +
                LightMsg( LIGHT0, DIFFUSE, P4f(0.7,0.7,0.7,1)) +
                LightMsg( LIGHT0, SPECULAR, P4f(0.1,0.1,0.1,1)) +
                LightMsg( LIGHT0, CONSTANT_ATTENUATION, 0 ) +
                LightMsg( LIGHT0, LINEAR_ATTENUATION, 0 ) +
                LightMsg( LIGHT0, QUADRATIC_ATTENUATION, 1 ) +
                LightMsg( LIGHT0, SPOT_CUTOFF, 180 ) +
                LightModelMsg( LIGHT_MODEL_TWO_SIDE, 0 ) +
                LightModelMsg( LIGHT_MODEL_LOCAL_VIEWER, 1 ) +
                EnableMsg( LIGHTING ) +
                EnableMsg( LIGHT0 ) +
                TranslateMsg( P3f(0,0,-67.0/m_camFov)) +
                RotateMsg( m_camAngle, P3f(1,0,0)) +
                RotateMsg( m_angle2, P3f(0,1,0)) +
                LightMsg( LIGHT0, POSITION, P4f(0,1,0,1)) +
                m_renderMsg +
                DisableMsg( LIGHTING )
                );
        //GLUquadric *q = gluNewQuadric();
        //gluSphere( q, 0.5, 64, 64 );
        //gluDeleteQuadric( q );
        gl.Invoke( DisableMsg( LIGHTING ));
        m_area2->SetDirty( m_area2->GetRect() );
    }

    void Tick( IXmlScope *scope )
    {
        int timeDelta = scope->GetChunk("timeDelta")->AsInt();
        double dt = std::min( 20, timeDelta ) / 1000.0 * 1.0;
        m_angle1 += 20.0 * dt;
        m_angle2 += 10.0 * dt;
    }

    void ChangeCameraAngle( IXmlScope *scope )
    {
        m_camAngle = scope->GetChunk("angle")->AsFloat();
    }

    void ChangeCameraFOV( IXmlScope *scope )
    {
        m_camFov = scope->GetChunk("fov")->AsFloat();
    }
};


Dynamo_DEFINE_CREATE_INSTANCE( GraphicVirtualMachine1 );
