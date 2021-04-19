#include "UselessPch.h"

#include "DSSampleData.h"
#include "DSSoundCard.h"

#include "Useless/Error/Error.h"
#define DX_TRY(c,e) if ( c != 0 ) { /*throw e;*/ APP_WARNING(e,"DSound"); }

namespace Useless {
    
COMObject<IDirectSound3DBuffer> DSSampleData::Get3DBuffer  () const
{
    LPDIRECTSOUND3DBUFFER b;
    _p_sound_buffer->QueryInterface( IID_IDirectSound3DBuffer, (void**)&b);
    return b;
}

DSSampleData::DSSampleData(): _p_sound_buffer(0)
{}

DSSampleData::DSSampleData( const Snd::Properties &props )
{
    _buffer_desc = props;
    LPDIRECTSOUND8 p_snd = DirectSound::Instance().GetDSound();

    DX_TRY( p_snd->CreateSoundBuffer( &_buffer_desc.GetDesc(), &_p_sound_buffer, 0 ), 
            Error("Could not create sound buffer") );

    _properties = props;
}

DSSampleData::~DSSampleData()
{
    if (_p_sound_buffer) _p_sound_buffer->Release(); _p_sound_buffer=NULL;
}

SampleData* DSSampleData::Clone() const
{
    DSSampleData *p_new = new DSSampleData();

    LPDIRECTSOUND8 p_snd = DirectSound::Instance().GetDSound();

    DX_TRY( p_snd->DuplicateSoundBuffer( _p_sound_buffer, &p_new->_p_sound_buffer ),
            Error("Could not duplicate sound buffer") );

    p_new->_buffer_desc = _buffer_desc;
    p_new->_properties = _properties;

    return p_new;
}

SampleData* DSSampleData::CreateData( const Snd::Properties &props ) const
{
    return new DSSampleData(props);
}

Snd::Properties DSSampleData::GetProperties() const
{
    return _properties;
}

bool DSSampleData::IsValid()
{
    assert(_p_sound_buffer);

    unsigned long status;
    DX_TRY( _p_sound_buffer->GetStatus(&status), Error("Cannot Stop sound") );

    return (status & DSBSTATUS_BUFFERLOST)? 1:0;
}

bool DSSampleData::Lock( void **p_pdata, int *p_size )
{
    assert(_p_sound_buffer);

    void *pdata[2];
    unsigned long bytes[2];

    try {
    DX_TRY( _p_sound_buffer->Lock(0,0,&pdata[0],&bytes[0],&pdata[1],&bytes[1], DSBLOCK_ENTIREBUFFER ),0 );
    } catch(...) { return 0; }

    assert( !pdata[1] && !bytes[1] && bytes[0]==_properties.length*_properties.channels*(_properties.bits/8) );
    *p_pdata = _lock_ptr = pdata[0];
    *p_size  = bytes[0];
    return true;
}

void DSSampleData::Unlock()
{
    assert(_p_sound_buffer);

    void *pdata[2] = { _lock_ptr, 0 };
    unsigned long bytes[2] = { _properties.length*(_properties.bits/8), 0 };
    
    DX_TRY( _p_sound_buffer->Unlock(pdata[0],bytes[0],pdata[1],bytes[1] ), 
        Error("Could not unlock sound buffer") );
}

void DSSampleData::SetRate( long rate )
{
    assert(_p_sound_buffer);

    DX_TRY( _p_sound_buffer->SetFrequency(rate), Error("Cannot SetRate()") );
}

long DSSampleData::GetRate() const
{
    assert(_p_sound_buffer);

    unsigned long rate;
    DX_TRY( _p_sound_buffer->GetFrequency(&rate), Error("Cannot GetRate()") );

    return rate;
}

void DSSampleData::SetPosition( long position )
{
    assert(_p_sound_buffer);
    long pos = position * (_properties.bits * _properties.channels)/8;
    DX_TRY( _p_sound_buffer->SetCurrentPosition( pos ), Error("Cannot SetPosition()") );
}

long DSSampleData::GetPlayPosition() const
{
    assert(_p_sound_buffer);

    unsigned long pos;
    DX_TRY( _p_sound_buffer->GetCurrentPosition( &pos, NULL ), Error("Cannot GetPosition()") );

    return (pos*8)/(_properties.bits*_properties.channels);//convert bytes to samples
}

long DSSampleData::GetWritePosition() const
{
    assert(_p_sound_buffer);

    unsigned long pos;
    DX_TRY( _p_sound_buffer->GetCurrentPosition( NULL, &pos ), Error("Cannot GetPosition()") );

    return (pos*8)/(_properties.bits*_properties.channels);//convert bytes to samples
}

void DSSampleData::SetFX( const Snd::PlayFX &fx )
{
    assert(_p_sound_buffer);
    using namespace Snd;

    if ( fx.volume< MAX_VOLUME+1 )
    {
        int r = DSBVOLUME_MAX - DSBVOLUME_MIN;
        int w = MAX_VOLUME - MIN_VOLUME;
        int x = fx.volume - MIN_VOLUME;
        int y = (x * r)/w + DSBVOLUME_MIN;
        _p_sound_buffer->SetVolume(y);
    }
    if ( fx.pan<MAX_PAN+1 )
    {
        int r = DSBPAN_RIGHT - DSBPAN_LEFT;
        int w = MAX_PAN - MIN_PAN;
        int x = fx.pan - MIN_PAN;
        int y = (x * r)/w + DSBPAN_LEFT;
        _p_sound_buffer->SetPan(y);
    }
    if ( fx.play_rate )
    {
        long y = fx.play_rate;
        _p_sound_buffer->SetFrequency(y);
    }
}

void DSSampleData::Play(bool loop)
{
    assert(_p_sound_buffer);

    DX_TRY( _p_sound_buffer->Play(0,0,(loop)? DSBPLAY_LOOPING : 0 ), Error("Cannot Play sound") );
}

void DSSampleData::Stop()
{
    assert(_p_sound_buffer);

    DX_TRY( _p_sound_buffer->Stop(), Error("Cannot Stop sound") );
}

bool DSSampleData::IsPlaying() const
{
    assert(_p_sound_buffer);

    unsigned long status;
    DX_TRY( _p_sound_buffer->GetStatus(&status), Error("Cannot Stop sound") );

    return (status & DSBSTATUS_PLAYING)? 1:0;
}

};//namespace Useless
