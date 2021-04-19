#ifndef __INCLUDED__USELESS_DSOUND_SOUND_CARD_H__
#define __INCLUDED__USELESS_DSOUND_SOUND_CARD_H__

#include "Useless/Sound/Device/SoundCard.h"
#include "Useless/Util/Singleton.h"

#include <mmsystem.h>
#include <dsound.h>


namespace Useless {

//USES
    class Screen;

class USELESS_API DSSoundCard : public SoundCard
{
public:
    DSSoundCard( const Screen &screen, bool use3DProcessing = false );
    virtual ~DSSoundCard();

    virtual Channel Play( Sample &sample, bool loop=false, bool pause=false );
    virtual void Validate( Sample &sample );

    virtual SampleData* CreateData( Snd::Properties &prop) const;

private:
    HWND _hWnd;
};

class USELESS_API __DirectSound
{
public:
    __DirectSound();
    ~__DirectSound();

    LPDIRECTSOUND8 GetDSound() { return _p_direct_sound; }
    LPDIRECTSOUNDBUFFER & GetBuffer() { return _p_buffer; }
    LPDIRECTSOUND3DLISTENER & GetListener() { return _p_listener; }
    
private:
    LPDIRECTSOUND8          _p_direct_sound;
    LPDIRECTSOUNDBUFFER     _p_buffer;
    LPDIRECTSOUND3DLISTENER _p_listener;
};

DECLARE_SINGLETON( __DirectSound, DirectSound, USELESS_API );

};//namespace Useless
#endif//__INCLUDED__USELESS_DSOUND_SOUND_CARD_H__
