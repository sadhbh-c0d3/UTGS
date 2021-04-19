#include "Useless/Graphic/Device/DDraw/DDScreen.h"

namespace Useless {

    Screen* CreateDDScreen( const char *title )
    {
        return new DDScreen( title);
    }

    Surface* CreateDDSurface()
    {
        return new DDSurface();
    }

    Surface* CreateDDSurfaceProp( const Surf::Properties &p )
    {
        return new DDSurface( p);
    }
    
};//namespace Useless
