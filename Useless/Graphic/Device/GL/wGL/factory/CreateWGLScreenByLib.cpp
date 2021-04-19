#include "UselessPch.h"

#ifdef USELESS_DEFAULT_SCREEN_WGL

#pragma message("Default Screen will be WGL")

#include "Useless/Graphic/Device/GL/wGL/WGLScreen.h"

namespace Useless {

    Screen * CreateScreenByLib( const char *title )
    {
        return new WGLScreen( title);
    }

};//namespace Useless

#endif