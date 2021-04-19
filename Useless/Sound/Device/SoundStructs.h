#ifndef __INCLUDED__USELESS__SOUND_STRUCTS_H__
#define __INCLUDED__USELESS__SOUND_STRUCTS_H__

namespace Useless {
namespace Snd {

struct USELESS_API Format
{
    Format();

    long length;      //number of samples
    long sample_rate; //44100, 22050 etc...
    char channels;    //1-mono or 2-stereo
    char bits;        //8 or 16
};

struct USELESS_API Properties : public Format
{
    Properties();
    Properties( const Format &f );

    bool volume_ctl;
    bool pan_ctl;
    bool pitch_ctl;
    bool sfx_ctl;
    bool space_ctl;

    bool global_focus;
    bool primary;
    bool sndcard_memory;

    bool operator ==( const Properties& ) const;
    bool operator !=( const Properties& ) const;
};

enum { MIN_VOLUME=-100,  MAX_VOLUME=0 };
enum { MIN_PAN   = -40,  MAX_PAN  =40 };

struct USELESS_API PlayFX
{
    PlayFX();

    int play_rate;
    int volume;
    int pan;
};

};//namespace Snd
};//namespace Useless
#endif//__INCLUDED__USELESS__SOUND_STRUCTS_H__
