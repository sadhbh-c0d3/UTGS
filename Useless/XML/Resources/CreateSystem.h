#ifndef __INCLUDED_USELESS_CREATE_SYSTEM_H__
#define __INCLUDED_USELESS_CREATE_SYSTEM_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/Graphic/Device/Screen.h"
#include "Useless/GUI/GUIMaster.h"
#include "Useless/GUI/Display/Hints.h"

namespace Useless {

/* Create methods
---------------------------------------------------------------------------------------------*/
USELESS_RESOURCES_API Resource* CreateScreen( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_screen( "system::screen", CreateScreen );

USELESS_RESOURCES_API Resource* CreateGUI( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_gui( "system::gui", CreateGUI );

USELESS_RESOURCES_API Resource* CreateImageCursor( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_image_cursor( "cursors::image", CreateImageCursor );


USELESS_RESOURCES_API Resource* CreateSysOutputOpen( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_sys_open_output("system::open-output", CreateSysOutputOpen );

USELESS_RESOURCES_API Resource* CreateSysOutputClose( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_sys_close_output("system::close-output", CreateSysOutputClose );

USELESS_RESOURCES_API Resource* CreateSysOutputImage( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_sys_image_out("system::image-out", CreateSysOutputImage );

/* Query methods
---------------------------------------------------------------------------------------------*/

typedef ResourceQuery< SPointer< Screen > > ScreenResource;
typedef ResourceQuery< SPointer< GUIMaster > > GUIMasterResource;
typedef ResourceQuery< SPointer< Workspace > > WorkspaceQuery;

inline Screen& QueryScreen( const std::string &id )
{
    SPointer<Screen> *p_screen;
    Resources::Instance().Query("system", id, p_screen);
    return **p_screen;
}

inline GUIMaster& QueryGUI( const std::string &id )
{
    SPointer<GUIMaster> *p_gui;
    Resources::Instance().Query("system", id, p_gui);
    return **p_gui;
}

inline Workspace& QueryWorkspace( const std::string &id )
{
    SPointer<GUIMaster> *p_gui;
    Resources::Instance().Query("system", id, p_gui);
    return (**p_gui).GetWorkspace();
}

//-------------------------------------------------------------------
USELESS_RESOURCES_API SPointer< MouseCursor > MakeMouseCursor( const std::string &id_expr );

};//namespace Useless
#endif//__INCLUDED_USELESS_CREATE_SYSTEM_H__
