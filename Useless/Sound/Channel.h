#ifndef __INCLUDED__USELESS__CHANNEL_H__
#define __INCLUDED__USELESS__CHANNEL_H__

#include "Useless/Sound/Sample.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

class USELESS_API Channel
{
public:
    Channel() {}
    Channel( const SampleData &sample_data, bool loop_play=false, bool play=true );
    ~Channel();

    Channel& SetVolume(int);
    Channel& SetPan   (int);
    Channel& SetRate  (int);
    Channel& Play(bool loop=false);
    Channel& Stop();
    bool IsPlaying() const;
    bool IsValid() const { return !!_played_sample_clone; }

    //-- use with care - no reference counting
    SampleData* GetPtr() { return &*_played_sample_clone; }

private:
    SPointer< SampleData > _played_sample_clone;
};

};//namespace Useless
#endif//__INCLUDED__USELESS__CHANNEL_H__
