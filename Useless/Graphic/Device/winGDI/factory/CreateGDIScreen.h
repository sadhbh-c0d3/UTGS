#include "Useless/Graphic/Device/CreateScreen.h"

namespace Useless {
    
    enum GDIScreenDriverId
    {
        WINDOWS_GDI_SCREEN=2
    };
    
    extern Screen* CreateGDIScreen( const char *title);
    extern Surface* CreateGDISurface();
    extern Surface* CreateGDISurfaceProp( const Surf::Properties &);

    static ScreenDriverRegistrator __GDI_scr_drv_reg( WINDOWS_GDI_SCREEN,
            &CreateGDIScreen, &CreateGDISurface, &CreateGDISurfaceProp);
};
