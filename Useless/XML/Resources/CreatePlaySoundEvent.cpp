#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSignalEvents.h"
#include "Useless/XML/Resources/CreateSounds.h"

namespace Useless {

struct PlaySoundSignalEvent : SignalEvent
{
    SoundCard *_card;
    Channel _ch;
    int _loudness;

    PlaySoundSignalEvent( SoundCard *card )
        : _card( card ), _loudness( 0 )
    {
    }

    virtual void Raise( Signal *s, const std::string &arg )
    {
        if ( arg.find("__volume:") == 0 )
        {
            _loudness = to_integer( arg.substr( std::string("__volume:").size(), -1 ));
        }
        else
        {
            SampleResource sample( "sounds", arg );

            if ( _ch.IsValid() )
            {
                _ch.Stop();
            }

            _ch = _card->Play( **sample );
            _ch.SetVolume( _loudness );
        }
    }
};


Resource* CreatePlaySoundEvent( XMLIterator i, XMLCreatorEnv *env )
{
    std::string cardName = get_attribute( (*i)._attributes, "soundcard" );

    SoundCardResource card( "system", cardName );

    PlaySoundSignalEvent *soundEvent = new PlaySoundSignalEvent( (*card).get() );

    SPointer< SignalEvent > event( soundEvent );

    return new AnyResource< SPointer< SignalEvent > >( event );
}


};//Useless
