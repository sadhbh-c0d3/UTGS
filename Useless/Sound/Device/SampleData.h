#ifndef __INCLUDED__USELESS__SAMPLE_DATA_H__
#define __INCLUDED__USELESS__SAMPLE_DATA_H__

#include "Useless/Sound/Device/SoundStructs.h"

namespace Useless {

/*! Holds sample data
 */
class USELESS_API SampleData
{
public:
    /* SampleData( const Snd::Properties& ); */
    virtual ~SampleData() {}
    
    virtual SampleData* Clone() const = 0;
    virtual SampleData* CreateData( const Snd::Properties& ) const = 0;

    virtual Snd::Properties GetProperties() const = 0;

    virtual bool IsValid() = 0;
    virtual bool Lock( void **pdata, int *size ) = 0;
    virtual void Unlock() = 0;

    virtual void SetRate( long rate ) = 0;
    virtual long GetRate() const = 0;

    virtual void SetPosition( long position ) = 0;
    virtual long GetPlayPosition() const = 0;
    virtual long GetWritePosition() const = 0;

    virtual void SetFX( const Snd::PlayFX& ) = 0;
    virtual void Play(bool loop=false) = 0;
    virtual void Stop() = 0;
    virtual bool IsPlaying() const = 0;
};


};//namespace Useless
#endif//__INCLUDED__USELESS__SAMPLE_DATA_H__
