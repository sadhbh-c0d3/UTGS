#ifndef __INCLUDED__USELESS_SOUND_CREATOR_REGISTRATOR_H__
#define __INCLUDED__USELESS_SOUND_CREATOR_REGISTRATOR_H__

#include "Useless/Sound/FileIO/SoundFactory.h"

namespace Useless {

class RegisterSoundLoader
{
public:
    RegisterSoundLoader( int st_id,
                         bool (*recognize)(IFile&,Snd::Format*),
                         SoundLoader* (*create_loader)())
    {
        SoundFactory::Instance().Register( st_id, create_loader );
        SoundFactory::Instance().Register( st_id, recognize );
    }
};


class RegisterSoundSaver
{
public:
    RegisterSoundSaver( int st_id, SoundSaver* (*create_saver)())
    {
        SoundFactory::Instance().Register( st_id, create_saver );
    }
};

};//namespace Useless
#endif//__INCLUDED__USELESS_SOUND_CREATOR_REGISTRATOR_H__
