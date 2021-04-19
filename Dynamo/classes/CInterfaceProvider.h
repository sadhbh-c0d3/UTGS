#ifndef __INCLUDED_DYNAMO_CINTERFACE_PROVIDER_H__
#define __INCLUDED_DYNAMO_CINTERFACE_PROVIDER_H__

#include "Dynamo/classes/CInterface.h"
#pragma comment (lib,"Dynamo.lib")

namespace Dynamo {

    struct CInterfaceProvider : CInterface, virtual IInterfaceProvider
    {
        IGraphix * ProvideIGraphix();

        IAudio * ProvideIAudio();

        IGui * ProvideIGui();

        IGameServer * ProvideIGameServer();

        IProfileManager * ProvideIProfileManager();

        IPlayerProfile * ProvideIPlayerProfile();

        IGameInfo * ProvideIGameInfo();
    
        IXml *ProvideIXml( std::string xmlId, std::string fileName, bool useEncryption );

        IModule *ProvideIModule( std::string xmlId, IXmlProgram *prog, IXmlScope *pScope );
    
        IXmlProgram *ProvideIXmlProgram( XMLProgram::IFiniteStateMachine *pFSM );
    };

};//Dynamo

#endif//__INCLUDED_DYNAMO_CINTERFACE_PROVIDER_H__
