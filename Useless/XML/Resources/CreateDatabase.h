#ifndef __INCLIDED_USELESS_XML_DATABASE_H__
#define __INCLIDED_USELESS_XML_DATABASE_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/GUI/Widgets/DataConnector.h"

namespace Useless {

USELESS_RESOURCES_API Resource* CreateDbTable( XMLIterator i, XMLCreatorEnv *env);
USELESS_RESOURCES_API Resource* UpdateDbTable( XMLIterator i, XMLCreatorEnv *env);

static RegisterXMLCreator reg_create_database_table( "database::table", &CreateDbTable );
static RegisterXMLCreator reg_update_database_table( "database::update", &UpdateDbTable );

};//Useless
#endif//__INCLIDED_USELESS_XML_DATABASE_H__
