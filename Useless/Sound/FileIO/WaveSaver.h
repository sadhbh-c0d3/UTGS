#ifndef __INCLUDED_COOL_WAVE_SAVER_H__
#define  __INCLUDED_COOL_WAVE_SAVER_H__

#include "Useless/File/OFile.h"
#include "Useless/Sound/FileIO/SoundInterface.h"

namespace Useless {

class USELESS_API WaveSaver
{
public:
      void Save( OFile &file, SoundInterface &sound_writer);
};

//static RegisterSoundSaver __register_wave_saver( WAVE, &WaveSaver::Create );


};//namespace Useless
#endif//__INCLUDED_COOL_WAVE_SAVER_H__
