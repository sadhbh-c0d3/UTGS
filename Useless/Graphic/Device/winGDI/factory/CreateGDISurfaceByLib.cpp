#include "UselessPch.h"

#ifdef USELESS_DEFAULT_SCREEN_GDI

#pragma message("Default Screen will be GDI")

#include "Useless/Graphic/Device/winGDI/GDIScreen.h"

namespace Useless {

    Surface* CreateSurfaceByLib()
    {
        return new GDISurface();
    }

    Surface* CreateSurfaceByLib( const Surf::Properties &p )
    {
        return new GDISurface( p, GDIScreen::s_hwnd);
    }

};//namespace Useless

#endif