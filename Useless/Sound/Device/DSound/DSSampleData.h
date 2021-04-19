#ifndef __INCLUDED__USELESS__DSOUND_SAMPLE_DATA_H__
#define __INCLUDED__USELESS__DSOUND_SAMPLE_DATA_H__

#include "Useless/Sound/Device/SampleData.h"
#include "Useless/System/w32/COMObject.h"
#include "DSStructs.h"
#include <dsound.h>

namespace Useless {

/*! Holds sample data with DirectSound SoundBuffer
 */
class USELESS_API DSSampleData : public SampleData
{
public:
    DSSampleData();
    DSSampleData( const Snd::Properties& );
    virtual ~DSSampleData();
    
    virtual SampleData* Clone() const;
    virtual SampleData* CreateData( const Snd::Properties& ) const;

    virtual Snd::Properties GetProperties() const;

    virtual bool IsValid();
    virtual bool Lock( void **pdata, int *size );
    virtual void Unlock();

    virtual void SetRate( long rate );
    virtual long GetRate() const;

    virtual void SetPosition( long position );
    virtual long GetPlayPosition() const;
    virtual long GetWritePosition() const;

    virtual void SetFX( const Snd::PlayFX& );
    virtual void Play(bool loop=false);
    virtual void Stop();
    virtual bool IsPlaying() const;

    /* DirectX part
    ---------------------------------------------------------------*/
    LPDIRECTSOUNDBUFFER GetDSBuffer    () const { return _p_sound_buffer; }
    const DSBUFFERDESC& GetDSBufferDesc() const { return _buffer_desc.GetDesc(); }
    COMObject<IDirectSound3DBuffer> Get3DBuffer  () const;


private:
    LPDIRECTSOUNDBUFFER _p_sound_buffer;
    Snd::DSBufferDesc   _buffer_desc;
    Snd::Properties     _properties;
    void*               _lock_ptr;

    friend class DSSoundCard;
};


};//namespace Useless
#endif//__INCLUDED__USELESS__DSOUND_SAMPLE_DATA_H__
