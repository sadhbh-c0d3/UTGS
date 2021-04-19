#ifndef __INCLUDED_COOL_WAVE_LOADER_H__
#define  __INCLUDED_COOL_WAVE_LOADER_H__

#include "Useless/File/IFile.h"
#include "Useless/Sound/FileIO/SoundInterface.h"
#include "Useless/Sound/detail/SoundCreatorRegistrator.h"

namespace Useless {

class USELESS_API WaveLoader : public SoundLoader
{
public:
    virtual void Load( IFile &file, OSoundInterface &sound_writer);

    static bool Recognize( IFile &file, Snd::Format *format=NULL );
    static SoundLoader* Create() { return new WaveLoader; }
};

static RegisterSoundLoader __register_wave_loader( WAVE, &WaveLoader::Recognize, &WaveLoader::Create );

};//namespace Useless
#endif//__INCLUDED_COOL_WAVE_LOADER_H__
