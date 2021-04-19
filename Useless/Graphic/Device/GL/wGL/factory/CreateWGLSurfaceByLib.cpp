#include "UselessPch.h"

#ifdef USELESS_DEFAULT_SCREEN_WGL

#pragma message("Default Screen will be WGL")

#include "Useless/Graphic/Device/GL/wGL/WGLScreen.h"

namespace Useless {

    Surface* CreateSurfaceByLib()
    {
        return new WGLSurface();
    }

    Surface* CreateSurfaceByLib( const Surf::Properties &p )
    {
        //Surf::Properties pp(p);
        //return new WGLSurface( pp );
        throw Error("WGL: CreateWGLSurfaceProp - Not implemented!");
        return 0;
    }

};//namespace Useless

#endif