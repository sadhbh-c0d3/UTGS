#ifndef __INCLUDED__USELESS_AUDIO_H__
#define __INCLUDED__USELESS_AUDIO_H__

#include "Useless/UselessConfig.h"

namespace Useless {

/*! Abstract for playing audio paths
 *  Use it to play long sound tracks with compression
 */
class USELESS_API Audio
{
public:
    virtual ~Audio() {}

    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual bool IsPlaying() const = 0;

    virtual void   SetTime    (double) = 0;
    virtual double GetTime    () const = 0;
    virtual double GetDuration() const = 0;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_AUDIO_H__
