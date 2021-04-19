#include "UselessPch.h"

#include "Useless/Graphic/Device/winGDI/GDIScreen.h"

namespace Useless {

    Screen * CreateGDIScreen( const char *title )
    {
        return new GDIScreen( title);
    }

    Surface* CreateGDISurface()
    {
        return new GDISurface();
    }

    Surface* CreateGDISurfaceProp( const Surf::Properties &p )
    {
        return new GDISurface( p, GDIScreen::s_hwnd);
    }

};//namespace Useless
