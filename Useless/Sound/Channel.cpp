#include "UselessPch.h"


#include "Channel.h"

namespace Useless {

Channel::Channel( const SampleData &sample_data, bool loop_play, bool play )
{
    _played_sample_clone = sample_data.Clone();

    if (play) { Play(loop_play); }
}

Channel::~Channel()
{
    if ( _played_sample_clone.Unique() && IsPlaying() ) { Stop(); }
}

Channel& Channel::SetVolume( int x)
{
    Snd::PlayFX fx;
    fx.volume = x;

    _played_sample_clone->SetFX(fx);
    return *this;
}

Channel& Channel::SetPan( int x )
{
    Snd::PlayFX fx;
    fx.pan = x;

    _played_sample_clone->SetFX(fx);
    return *this;
}

Channel& Channel::SetRate( int x )
{
    Snd::PlayFX fx;
    fx.play_rate = x;

    _played_sample_clone->SetFX(fx);
    return *this;
}

Channel& Channel::Play( bool loop )
{
    _played_sample_clone->Play(loop);
    return *this;
}

Channel& Channel::Stop()
{
    _played_sample_clone->Stop();
    return *this;
}

bool Channel::IsPlaying() const
{
    return _played_sample_clone->IsPlaying();
}

};//namespace Useless


