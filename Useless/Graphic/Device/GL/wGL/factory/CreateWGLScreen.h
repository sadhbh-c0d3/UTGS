#include "Useless/Graphic/Device/CreateScreen.h"

namespace Useless {
    
    enum GDIScreenDriverId
    {
        WINDOWS_OPEN_GL_SCREEN=2
    };
    
    extern Screen*  USELESS_API CreateWGLScreen( const char *title);
    extern Surface* USELESS_API CreateWGLSurface();
    extern Surface* USELESS_API CreateWGLSurfaceProp( const Surf::Properties &);

    static ScreenDriverRegistrator __WGL_scr_drv_reg( WINDOWS_OPEN_GL_SCREEN,
            &CreateWGLScreen, &CreateWGLSurface, &CreateWGLSurfaceProp);
};
