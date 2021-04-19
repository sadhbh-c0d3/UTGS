#include "Useless/Graphic/Device/DDraw/DDSurface.h"

namespace Useless {

    Surface* CreateSurfaceByLib()
    {
        return new DDSurface();
    }

    Surface* CreateSurfaceByLib( const Surf::Properties &p )
    {
        return new DDSurface( p);
    }

};//namespace Useless
