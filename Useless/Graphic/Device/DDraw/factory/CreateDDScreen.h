#include "Useless/Graphic/Device/CreateScreen.h"

namespace Useless {

    //! In order to use multiple screen drivers, please
    //  recompile DDraw library with
    //  defined SUPPRESS_DIRECT_DRAW_INITIALIZE
    
    enum DDScreenDriverId
    {
        DIRECT_DRAW_SCREEN=0
    };
    
    extern Screen* CreateDDScreen( const char *title);
    extern Surface* CreateDDSurface();
    extern Surface* CreateDDSurfaceProp( const Surf::Properties &);

    static ScreenDriverRegistrator __DD_scr_drv_reg( DIRECT_DRAW_SCREEN,
            &CreateDDScreen, &CreateDDSurface, &CreateDDSurfaceProp);
};
