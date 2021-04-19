#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSounds.h"
#include "Useless/XML/Resources/CreateSystem.h"

#include "Useless/Sound/SampleFile.h"
#include "Useless/Sound/Device/DSound/DSSoundCard.h"

namespace Useless {

Resource* CreateSoundCard( XMLIterator i, XMLCreatorEnv *env )
{
    Screen & screen = QueryScreen("screen");

    SPointer< SoundCard > soundCard = new DSSoundCard( screen );

    return new AnyResource< SPointer< SoundCard > >( soundCard );
}


Resource* CreateSample( XMLIterator i, XMLCreatorEnv *env )
{
    std::string fileName = get_attribute( (*i)._attributes, "src" );

    SPointer< Sample > sample( new SampleFile( fileName ));

    return new AnyResource< SPointer< Sample > >( sample );
}

};//Useless
