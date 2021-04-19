#ifndef __INCLUDED__USELESS_SOUND_FACTORY_H__
#define __INCLUDED__USELESS_SOUND_FACTORY_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'

#include <map>
#include "Useless/Sound/Device/SoundStructs.h"
#include "Useless/Sound/FileIO/SoundInterface.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Util/Singleton.h"
#include "Useless/Util/SingletonKiller.h"

namespace Useless {

enum SampleTypes 
{
    WAVE, IFF, SND 
};

class USELESS_API __SoundFactory
{
public:
    void Register( int st_id, bool         (*recognize)(IFile&,Snd::Format*) );
    void Register( int st_id, SoundLoader* (*create_loader)() );
    void Register( int st_id, SoundSaver*  (*create_saver)() );

    SPointer< SoundLoader > Create( IFile &, Snd::Format *format=NULL );
    SPointer< SoundSaver >  Create( int st_id );

private:
    struct SoundCreator
    {
        bool         (*recognize)(IFile&,Snd::Format*);
        SoundLoader* (*create_loader)();
        SoundSaver*  (*create_saver)();
    };

    typedef std::map< int, SoundCreator > SoundCreatorMap;

    SoundCreatorMap _sound_creators;
};

DECLARE_SINGLETON(__SoundFactory, SoundFactory, USELESS_API);

};//namespace Useless
#endif//__INCLUDED__USELESS_SOUND_FACTORY_H__
