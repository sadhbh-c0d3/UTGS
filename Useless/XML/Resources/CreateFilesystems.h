#ifndef __INCLUDED_USELESS_CREATE_FILESYSTEMS_H__
#define __INCLUDED_USELESS_CREATE_FILESYSTEMS_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/File/VIFS.h"

namespace Useless {

    /* Create methods
---------------------------------------------------------------------------------------------*/
USELESS_RESOURCES_API Resource* CreateMountDir( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_mount_dir( "filesystems::dir", CreateMountDir );

USELESS_RESOURCES_API Resource* CreateMountHtc( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_mount_htc( "filesystems::htc", CreateMountHtc );

USELESS_RESOURCES_API Resource* CreateMountScan( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_mount_scan( "filesystems::scan", CreateMountScan );

};

#endif//__INCLUDED_USELESS_CREATE_FILESYSTEMS_H__
