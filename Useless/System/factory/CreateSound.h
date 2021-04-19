#ifndef __INCLUDED__USELESS_CREATE_SOUND_H__
#define __INCLUDED__USELESS_CREATE_SOUND_H__

#include "Useless/PlatformConfig.h"
#include "Useless/Sound/Device/SoundCard.h"

#ifdef __USE_DIRECT_SOUND__
#   include "Useless/Sound/Device/DSound/DSSoundCard.h"
#   include "Useless/Sound/Device/DSound/DSSampleData.h"
#else
#   error "Only DirectSound was implemented"
#endif

#include "Useless/System/factory/CreateBase.h"

namespace Useless {

template<> class Creator< SoundCard >
{
public:
    typedef DSSoundCard type;// a place for API dependant changes

    static SoundCard* Create( Screen &s )
    {
        return new type(s);
    }

    static SoundCard* Create( Screen *s )
    {
        return new type(*s);
    }
};

template<> class Creator< SampleData >
{
public:
    typedef DSSampleData type;// a place for API dependant changes

    static SampleData* Create( const Snd::Properties &p )
    {
        return new type(p);
    }
};

};//namespace Useless
#endif//__INCLUDED__USELESS_CREATE_SOUND_H__
