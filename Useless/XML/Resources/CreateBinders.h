#ifndef __INCLUDED_CREATE_BINDERS_H__
#define __INCLUDED_CREATE_BINDERS_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/XML/Resources/GUIBinder.h"

namespace Useless {

USELESS_RESOURCES_API Resource* CreateGUIBinder( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_gui_binder( "binders::gui-binder", CreateGUIBinder );

typedef ResourceQuery< SPointer< GUIBinder > > GUIBinderResource;

};//Useless

#endif//__INCLUDED_CREATE_BINDERS_H__
