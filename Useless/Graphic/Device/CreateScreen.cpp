#include "UselessPch.h"

#include "CreateScreen.h"

namespace Useless {
    
    int g_screen_driver = NUM_SCREEN_DRIVERS;
    ScreenCtor *g_CreateScreen[NUM_SCREEN_DRIVERS];
    SurfaceCtor *g_CreateSurface[NUM_SCREEN_DRIVERS];
    SurfaceCtorProp *g_CreateSurfaceProp[NUM_SCREEN_DRIVERS];
    
    Screen * CreateScreen( const char *title)
    {   
        // Create registered screen
        if ( g_screen_driver != NUM_SCREEN_DRIVERS)
        {
            return (*g_CreateScreen[g_screen_driver])( title);
        }
        else // Unregistered screen type
        {
            return CreateScreenByLib( title);
        }
    }
    
    Surface* CreateSurface()
    {
        // Create registered surface
        if ( g_screen_driver != NUM_SCREEN_DRIVERS)
        {
            return (*g_CreateSurface[g_screen_driver])();
        }
        else // Unregistered surface type
        {
            return CreateSurfaceByLib();
        }
    }
    
    Surface* CreateSurface( const Surf::Properties &p)
    {
        // Create registered surface
        if ( g_screen_driver != NUM_SCREEN_DRIVERS)
        {
            return (*g_CreateSurfaceProp[g_screen_driver])(p);
        }
        else // Unregistered surface type
        {
            return CreateSurfaceByLib(p);
        }
    }

};//namespace Useless
