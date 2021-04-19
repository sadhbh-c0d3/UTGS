#include "Useless/UselessLib.h"
#include "Useless/System/Application.h"
#include "Useless/System/factory/CreateScreen.h"
#include "Useless/Util/CopyingFactory.h"
#include "Useless/System/w32/ErrorDialog.h"

#pragma comment (lib,"strmiids.lib")
#pragma comment (lib,"Quartz.lib")

#include <GL/gl.h>

namespace Useless {
    const char * Application::_icon_name = 0;
    const char * Application::_app_title = "test002";
    const int    Application::_icon_resource_id = 0;
    const int    Application::_big_icon_resource_id = 0;
}

struct Paint
{
    Useless::Screen *m_Screen;
    Paint( Useless::Screen *s ): m_Screen( s )
    {}

    void Apply()
    {
        m_Screen->Swap();
        m_Screen->Clear( 0xFFFFFF00 );
        m_Screen->Swap();
    }
};

int Main()
{
    try {

        Useless::SPointer< Useless::Screen > l_Screen = Useless::CreateScreen("test002");
        l_Screen->Open( 800, 600, 16, 60 );
        
        Useless::Tie2Signal( l_Screen->OnPaint, new Paint( l_Screen.get() ), &Paint::Apply );
        l_Screen->OnPaint.Send();

        Useless::Application::Run();
        return 0;
    }
    catch(...)
    {
        Useless::ErrorDialog();
        throw;
    }
}
