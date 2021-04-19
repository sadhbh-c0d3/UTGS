#include "UselessPch.h"

#include "Useless/Graphic/Device/GL/wGL/WGLScreen.h"

namespace Useless {

    Screen * CreateWGLScreen( const char *title )
    {
        return new WGLScreen( title);
    }

    Surface* CreateWGLSurface()
    {
        return new WGLSurface();
    }

    Surface* CreateWGLSurfaceProp( const Surf::Properties &p )
    {
        //Surf::Properties pp(p);
        //return new WGLSurface( pp );
        throw Error("WGL: CreateWGLSurfaceProp - Not implemented!");
        return 0;
    }

};//namespace Useless
