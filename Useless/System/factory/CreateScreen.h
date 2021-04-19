#ifndef __INCLUDED__USELESS__CREATE_SCREEN_H__
#define __INCLUDED__USELESS__CREATE_SCREEN_H__

#include "Useless/System/factory/CreateBase.h"
#include "Useless/Graphic/Device/Screen.h"
#include "Useless/Graphic/Device/Surface.h"

namespace Useless {

extern Screen * CreateScreen ( const char *title );
extern Surface* CreateSurface();
extern Surface* CreateSurface( const Surf::Properties &p );

/* Screen
------------------------------------------------------------*/
template<> class Creator< Screen >
{
public:
    typedef Screen type;

    static Screen* Create( int w, int h, int bpp, const char *title="IAV Game" )
    {
        Screen *p = CreateScreen( title);
        p->Open( w, h, bpp);
        return p;
    }

    static Screen* Create( int w, int h, const char *title="IAV Game" )
    {
        Screen *p = CreateScreen( title);
        p->OpenWindowed( w, h);
        return p;
    }

    static Screen* Create( const char *title="IAV Game" )
    {
        return CreateScreen( title);
    }
};

/* Surface
------------------------------------------------------------*/
template<> class Creator< Surface >
{
public:
    typedef Surface type;

    static Surface* Create( const Surf::Properties &p )
    {
        return CreateSurface(p);
    }

    static Surface* Create()
    {
        return CreateSurface();
    }
};

};//namespace Useless

#endif//__INCLUDED__USELESS__CREATE_SCREEN_H__
