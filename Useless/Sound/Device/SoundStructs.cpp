#include "UselessPch.h"


#include "SoundStructs.h"

namespace Useless {

namespace Snd {

Format::Format()
{
    length = 0;
    sample_rate = 0;
    channels = 0;
    bits = 0;
}

Properties::Properties()
{
    volume_ctl = 0;
    pan_ctl = 0;
    pitch_ctl = 0;
    sfx_ctl = 0;
    space_ctl = 0;

    global_focus = 0;
    primary = 0;
    sndcard_memory = 0;
}

Properties::Properties( const Format &f ) : Format(f)
{
    volume_ctl = 0;
    pan_ctl = 0;
    pitch_ctl = 0;
    sfx_ctl = 0;
    space_ctl = 0;

    global_focus = 0;
    primary = 0;
    sndcard_memory = 0;
}

bool Properties::operator ==( const Properties &p ) const
{
    return 
    ( length == p.length ) &&
    ( sample_rate == p.sample_rate ) &&
    ( channels == p.channels ) &&
    ( bits == p.bits) &&
    
    ( volume_ctl == p.volume_ctl) &&
    ( pan_ctl == p.pan_ctl) &&
    ( pitch_ctl == p.pitch_ctl) &&
    ( sfx_ctl == p.sfx_ctl) &&
    ( space_ctl == p.space_ctl) &&

    ( global_focus == p.global_focus) &&
    ( primary == p.primary) &&
    ( sndcard_memory == p.sndcard_memory );

}

bool Properties::operator !=( const Properties &p ) const
{
    return ! ( this->operator==(p) );
}


PlayFX::PlayFX()
{
    play_rate = 0;
    volume = MAX_VOLUME+1;
    pan = MAX_PAN+1;
}

};//namespace Snd

};//namespace Useless
