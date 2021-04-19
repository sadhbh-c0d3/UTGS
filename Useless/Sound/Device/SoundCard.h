#ifndef __INCLUDED__USELESS_SOUND_CARD_H__
#define __INCLUDED__USELESS_SOUND_CARD_H__

#include "Useless/Sound/Channel.h"
#include "Useless/Sound/MultiChannel.h"

namespace Useless {

class USELESS_API SoundCard : public MultiChannel
{
public:
    virtual ~SoundCard() {}

    virtual Channel Play( Sample &sample, bool loop=false, bool pause=false ) = 0;
    virtual void Validate( Sample &sample ) = 0;
    
    virtual SampleData* CreateData( Snd::Properties &prop) const = 0;
};

USELESS_API bool IsSoundAvailable();
USELESS_API bool Is3DSoundPrefered();

};//namespace Useless
#endif//__INCLUDED__USELESS_SOUND_CARD_H__
