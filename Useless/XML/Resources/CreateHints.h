#ifndef __INCLUDED_USELESS_CREATE_HINTS_H__
#define __INCLUDED_USELESS_CREATE_HINTS_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/GUI/Display/Hints.h"

namespace Useless {

/* Create methods
---------------------------------------------------------------------------------------------*/
USELESS_RESOURCES_API Resource* CreateHints( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_hints( "system::hints", CreateHints );

/* Query methods
---------------------------------------------------------------------------------------------*/
inline Hints& QueryHints( const std::string &id )
{
    SPointer<Hints> *p_hints;
    Resources::Instance().Query("system", id, p_hints);
    return **p_hints;
}

/* Helper methods
---------------------------------------------------------------------------------------------*/
USELESS_RESOURCES_API void SetHint( Widget *hint_owner, XMLAttributes attributes );

};//namespace Useless
#endif//__INCLUDED_USELESS_CREATE_SYSTEM_H__
