#include "UselessPch.h"

#include "Useless/Graphic/Device/Screen.h"
#include "DSSoundCard.h"
#include "DSSampleData.h"

#include "Useless/Error/Error.h"
#define DX_TRY(c,e) if ( c != 0 ) { throw e; }

namespace Useless {

INIT_SINGLETON( DirectSound, USELESS_API );

bool IsSoundAvailable()
{
    return !!DirectSound::Instance().GetDSound();
}

bool Is3DSoundPrefered()
{
    return !!DirectSound::Instance().GetListener();
}


__DirectSound::__DirectSound()
{
    if ( DirectSoundCreate8( 0, &_p_direct_sound, 0 ) != 0 )
    {
        assert("WARNING: Sound is not available");
        _p_direct_sound=NULL;
    }
}

__DirectSound::~__DirectSound()
{
    if (_p_direct_sound)
    {
        _p_direct_sound->Release();
        _p_direct_sound=NULL;
    }
}

DSSoundCard::DSSoundCard( const Screen &screen, bool use3DProcessing )
{
    assert( DirectSound::Instance().GetDSound() );

    DX_TRY( DirectSound::Instance().GetDSound()->SetCooperativeLevel( _hWnd=screen.GetHandle(), DSSCL_EXCLUSIVE ), 
            Error("Could not connect DSound to Screen") );

    DSBUFFERDESC bufferDesc;
    memset( &bufferDesc, 0, sizeof(bufferDesc));
    bufferDesc.dwSize = sizeof(bufferDesc);
    
    if ( use3DProcessing )
        { bufferDesc.dwFlags = DSBCAPS_CTRL3D + DSBCAPS_PRIMARYBUFFER; }
    else
        { bufferDesc.dwFlags = DSBCAPS_CTRLPAN + DSBCAPS_PRIMARYBUFFER; }
        
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.lpwfxFormat = 0;
    bufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    
    DX_TRY( DirectSound::Instance().GetDSound()->CreateSoundBuffer( &bufferDesc, &DirectSound::Instance().GetBuffer(), 0),
            Error("Could not create primary buffer") );
    
    if ( use3DProcessing )
        { DX_TRY( DirectSound::Instance().GetDSound()->QueryInterface( IID_IDirectSound3DListener8, (void**)&DirectSound::Instance().GetListener()),
            Error("Could not query liestener from primary buffer") ) }

}

DSSoundCard::~DSSoundCard()
{
    if ( DirectSound::Instance().GetDSound() )
    {
        DirectSound::Instance().GetDSound()->SetCooperativeLevel( _hWnd, 0 );
    }
}

Channel DSSoundCard::Play( Sample &sample, bool loop, bool pause )
{
    Validate(sample);
    
    return MultiChannel::Play( Channel( *sample.GetData(), loop, !pause ) );
}

void DSSoundCard::Validate( Sample &sample )
{
    SampleData *p_data = sample.GetData();

    Snd::Properties req_prop;
    sample.GetProperties(req_prop);

    if ( !p_data || typeid(p_data)!=typeid(DSSampleData) 
                 || req_prop!=p_data->GetProperties() )
    {
        sample.SetData( new DSSampleData(req_prop) );
    }
}

SampleData* DSSoundCard::CreateData( Snd::Properties &prop) const
{
    SampleData *data = new DSSampleData( prop);
    prop = data->GetProperties();
    return data;
}


}; //namespace Useless
