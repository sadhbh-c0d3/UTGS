#include "Useless/Graphic/Device/DDraw/DDScreen.h"

namespace Useless {

    Screen* CreateScreenByLib( const char *title )
    {
        return new DDScreen( title);
    }

};//namespace Useless
