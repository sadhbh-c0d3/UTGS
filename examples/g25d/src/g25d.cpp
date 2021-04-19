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

struct TextelWriter : CInterface, IPixelWriter
{   
    GLuint               m_texId;
    GLuint               m_level;
    GLuint               m_priority;
    size_t               m_width;
    size_t               m_height;
    size_t               m_pitch;
    size_t               m_bytesPerPel;
    size_t               m_rowLength;
    ColorMode::Constant  m_colorMode;

    TextelWriter( GLuint texId, GLuint level, GLuint priority ) : m_texId( texId ), m_level( level ), m_priority( priority )
    {}

    ~TextelWriter()
    {
    }

    void SetupInput ( size_t width, size_t height, ColorMode::Constant mode, size_t pitch )
    {
        m_width = width;
        m_height = height;
        m_pitch = pitch;
        m_colorMode = mode;
        switch ( m_colorMode )
        {
            case ColorMode::BGRA:  m_bytesPerPel = 4; break;
            case ColorMode::RGBA:  m_bytesPerPel = 4; break;
            case ColorMode::BGR:   m_bytesPerPel = 3; break;
            case ColorMode::RGB:   m_bytesPerPel = 3; break;
            case ColorMode::GREY:  m_bytesPerPel = 1; break;
            case ColorMode::ALPHA: m_bytesPerPel = 1; break;
        }
        m_rowLength = m_pitch / m_bytesPerPel;
    
        glBindTexture( GL_TEXTURE_2D, m_texId );
  
        glPixelStorei( GL_UNPACK_ROW_LENGTH, m_rowLength );
        
        GLenum format;
        GLenum tf;
        switch ( m_colorMode )
        {
            case ColorMode::BGR:   format=GL_BGR_EXT;  tf=GL_RGB8;  break;
            case ColorMode::RGB:   format=GL_RGB;      tf=GL_RGB8;  break;
            case ColorMode::BGRA:  format=GL_BGRA_EXT; tf=GL_RGBA8; break;
            case ColorMode::RGBA:  format=GL_RGBA;     tf=GL_RGBA8; break;
            case ColorMode::GREY:  format=GL_LUMINANCE;tf=GL_LUMINANCE; break;
            case ColorMode::ALPHA: format=GL_ALPHA;    tf=GL_ALPHA; break;
            default: assert(!"Texture: unknown GL format!");
        };

        glTexImage2D( GL_TEXTURE_2D, m_level, tf, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, 0 );
        
        GLenum err= glGetError();
    }

    void WritePixels( const void *pointer, Rect4i area )
    {
        glBindTexture( GL_TEXTURE_2D, m_texId );
  
        glPixelStorei( GL_UNPACK_ROW_LENGTH, m_rowLength );

        GLenum format;
        GLenum tf;
        switch ( m_colorMode )
        {
            case ColorMode::BGR:   format=GL_BGR_EXT;  tf=GL_RGB8;  break;
            case ColorMode::RGB:   format=GL_RGB;      tf=GL_RGB8;  break;
            case ColorMode::BGRA:  format=GL_BGRA_EXT; tf=GL_RGBA8; break;
            case ColorMode::RGBA:  format=GL_RGBA;     tf=GL_RGBA8; break;
            case ColorMode::GREY:  format=GL_LUMINANCE;tf=GL_LUMINANCE; break;
            case ColorMode::ALPHA: format=GL_ALPHA;    tf=GL_ALPHA; break;
            default: assert(!"Texture: unknown GL format!");
        };

        glTexSubImage2D( GL_TEXTURE_2D, m_level, area.m_x, area.m_y, area.m_w, area.m_h,
                format, GL_UNSIGNED_BYTE, pointer );
        
        GLenum err= glGetError();

        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, m_priority );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    }
};

struct Texture : CInterface
{
    GLuint m_texId;

    Texture( Hand< IGraphix > graphix, std::string fileName )
    {
        glGenTextures( 1, &m_texId );
        Hand< IPixelWriter > writer = new TextelWriter( m_texId, 0, 0 );
        graphix->LoadPixels( fileName, writer.get() );
    }
    
    ~Texture()
    {
        glDeleteTextures( 1, &m_texId );
    }

    Message BindMsg()
    {
        return BindTextureMsg( TEXTURE_2D, m_texId );
    }
};

struct GraphicVirtualMachine1 : CInterface
{
    Hand< IInterfaceProvider >  m_provider;
    Hand< IGui >    m_gui;
    Hand< IWidget > m_area1;
    Hand< IWidget > m_area2;
    Hand< IWidget > m_desktop;
    Hand< IGraphix > m_graphix;
    Hand< Texture > m_texture[8];
    float m_camHead;
    float m_camPitch;
    float m_camFov;
    P3f   m_vertices[4];
    P2f   m_uvcoords[4];
    P4f   m_colors[4];
    P4u   m_faces[1];
    Message m_renderMsg;

    struct MyRenderCB : MessageCallback
    {
        GraphicVirtualMachine1 *m_this;
        MyRenderCB( GraphicVirtualMachine1 *ptr ): m_this( ptr ) {}

        Message Apply()
        {
            return TranslateMsg( P3f(0,0,0) ) +
                RotateMsg( m_this->m_camPitch, P3f(1,0,0)) +
                RotateMsg( m_this->m_camHead, P3f(0,1,0)) +
                EnableMsg( TEXTURE_2D ) +
                EnableMsg( DEPTH_TEST ) +
                ColorMsg( P4f(1,1,1,1) );
        }
    };

    GraphicVirtualMachine1( IInterfaceProvider *provider, IXmlProgram *prog, IXmlScope *scope )
    {
        m_provider  =   provider;
        m_graphix = m_provider->ProvideIGraphix();
        m_gui   =   m_provider->ProvideIGui();
        m_desktop = m_gui->GetWidget("gui.desktop");
        m_area1 =   m_gui->GetWidget("gui.desktop.minimap");
        m_area2 =   m_gui->GetWidget("gui.desktop.area");
        m_area1 ->  SetHook_Paint( make_hook_mp< IHook_Paint, IPaint * >( this, &GraphicVirtualMachine1::PaintArea1 ));
        m_area2 ->  SetHook_Paint( make_hook_mp< IHook_Paint, IPaint * >( this, &GraphicVirtualMachine1::PaintArea2 ));
        scope   ->  AddMethod("Tick", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::Tick ));
        scope   ->  AddMethod("ChangeCameraHead", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::ChangeCameraHead ));
        scope   ->  AddMethod("ChangeCameraPitch", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::ChangeCameraPitch ));
        scope   ->  AddMethod("ChangeCameraFOV", make_hook_mp< IXmlHook, IXmlScope * >( this, &GraphicVirtualMachine1::ChangeCameraFOV ));

        m_texture[0] = new Texture( m_graphix, "Room1/00.png");
        m_texture[1] = new Texture( m_graphix, "Room1/01.png");
        m_texture[2] = new Texture( m_graphix, "Room1/02.png");
        m_texture[3] = new Texture( m_graphix, "Room1/03.png");
        m_texture[4] = new Texture( m_graphix, "Room1/04.png");
        m_texture[5] = new Texture( m_graphix, "Room1/05.png");
        m_texture[6] = new Texture( m_graphix, "Room1/06.png");
        m_texture[7] = new Texture( m_graphix, "Room1/07.png");

        m_camHead = 0.0;
        m_camPitch = 0.0;
        m_camFov = 90.0;

        m_vertices[0] = P3f(-1,-1,0);
        m_vertices[1] = P3f( 1,-1,0);
        m_vertices[2] = P3f(-1, 1,0);
        m_vertices[3] = P3f( 1, 1,0);

        m_uvcoords[0] = P2f(0,1);
        m_uvcoords[1] = P2f(1,1);
        m_uvcoords[2] = P2f(0,0);
        m_uvcoords[3] = P2f(1,0);


        m_colors[0] = P4f(1,1,1,1);
        m_colors[1] = P4f(1,1,1,1);
        m_colors[2] = P4f(1,1,1,1);
        m_colors[3] = P4f(1,1,1,1);

        m_faces[0] = P4u(0,1,2,3);


        Message renderPlaneMsg = BlockMsg(
                SaveMsg( CURRENT_MATRIX ) +
                
                VertexArrayMsg( m_vertices, 4 ) + EnableClientStateMsg( VERTEX_ARRAY ) +
                IndexArrayMsg( m_faces[0], 4 ) + EnableClientStateMsg( INDEX_ARRAY ) +
                TexCoordArrayMsg( m_uvcoords, 4 ) + EnableClientStateMsg( TEXTURE_COORD_ARRAY ) +
                
                DrawElementsMsg( TRIANGLE_STRIP, 0, 4 )

                );
        
        m_renderMsg += CallbackMsg( MyRenderCB( this ));
        
        for ( int i=0; i<8; ++i )
        {
            m_renderMsg +=
                BlockMsg(
                    SaveMsg( CURRENT_MATRIX ) +
                    RotateMsg( i * -45.0, P3f(0,1,0)) +
                    TranslateMsg( P3f(0,0,-1.0) ) + 
                    m_texture[ i ]->BindMsg() + renderPlaneMsg
                    );
        }
        

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
                RotateMsg( -m_camHead, P3f(0,0,1)) +
                ScaleMsg( P3f(0.5, 0.5, 0.5)) +
                TranslateMsg( P3f(0,-1,0) ) +
                ShapeMsg( TRIANGLE_STRIP, ColorMsg( P4f(0,0,0,1)) +
                    VertexMsg( P3f(-0.2,0,0) ) +
                    VertexMsg( P3f(0,2,0) ) +
                    VertexMsg( P3f(0.2,0,0) )
                    ));
        m_area1->SetDirty( m_area1->GetRect() );
    }

    void PaintArea2( IPaint *paint )
    {
        GLGraphicDeviceUser gl( m_desktop, m_area2 );
        Rect4i rcArea2 = m_area2->GetRect();
        gl.SetPerspective( m_camFov, ((float)rcArea2.m_w)/(float)rcArea2.m_h, 0.01, 10.0);
        gl.Invoke( m_renderMsg );
        m_area2->SetDirty( m_area2->GetRect() );
    }

    void Tick( IXmlScope *scope )
    {
        int timeDelta = scope->GetChunk("timeDelta")->AsInt();
        double dt = std::min( 20, timeDelta ) / 1000.0 * 1.0;
    }

    void ChangeCameraHead( IXmlScope *scope )
    {
        m_camHead = scope->GetChunk("head")->AsFloat();
    }

    void ChangeCameraPitch( IXmlScope *scope )
    {
        m_camPitch = scope->GetChunk("pitch")->AsFloat();
    }

    void ChangeCameraFOV( IXmlScope *scope )
    {
        m_camFov = scope->GetChunk("fov")->AsFloat();
    }
};


Dynamo_DEFINE_CREATE_INSTANCE( GraphicVirtualMachine1 );
