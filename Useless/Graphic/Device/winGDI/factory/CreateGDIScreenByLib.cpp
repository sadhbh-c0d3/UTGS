#include "UselessPch.h"

#ifdef USELESS_DEFAULT_SCREEN_GDI

#include "Useless/Graphic/Device/winGDI/GDIScreen.h"

#pragma message("Default Screen will be GDI")

namespace Useless {

    Screen * CreateScreenByLib( const char *title )
    {
        return new GDIScreen( title);
    }

};//namespace Useless

#endif