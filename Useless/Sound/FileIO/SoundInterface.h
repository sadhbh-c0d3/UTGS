#ifndef __INCLUDED_COOL_SOUND_INTERFACE_H__
#define __INCLUDED_COOL_SOUND_INTERFACE_H__

#pragma warning(disable:4786) // disable identifier was trunicated ...

#include "Useless/Sound/detail/CoolTypes.h"
#include "Useless/Sound/Device/SoundStructs.h"

namespace Useless {

enum ChannelType { SINGLE=1, LEFT=2, RIGHT=4, INTERLEAVE=6 };

/*! Abstract class for transfering data to destination class
 */
class USELESS_API OSoundInterface
{
public:
    virtual ~OSoundInterface() {}

    virtual void SetMode( long all_samples, short bits, long frequency, ChannelType channel_info ) = 0;
    virtual void Write( ConstPtr from, long num_samples, long insertion_point, int pitch ) = 0;
};


/*! Abstract class for transfering data from source class
 */
class USELESS_API ISoundInterface
{
public:
    virtual ~ISoundInterface() {}

    virtual void GetMode( long all_samples, short bits, long frequency, ChannelType channel_info ) = 0;
    virtual void Read( Ptr into, long num_samples, long read_point, int pitch ) = 0;

};

//Uses
    class IFile;

/*! Abstract class for loading sound files
 */
class USELESS_API SoundLoader
{
public:
    virtual ~SoundLoader() {}

    virtual void Load( IFile &file, OSoundInterface &sound_dest ) = 0;
    static  bool Recognize( IFile &file, Snd::Format *format=0 ) { return false; };
};

//Uses
    class OFile;

/*! Abstract class for saving sound files
 */
class USELESS_API SoundSaver
{
public:
    virtual ~SoundSaver() {}

    virtual void Save( OFile &file, ISoundInterface &sound_source ) = 0;
};



/*----------------------------------------------------------------------------------------------*/
/* Temporarly W/A
 */
class USELESS_API SoundInterface : public ISoundInterface, public OSoundInterface
{
public:
    virtual ~SoundInterface() {}

    virtual ConstPtr Lock( long *all_samples, int *bits, long *frequency, bool *stereo ) = 0;
    virtual void     Unlock() = 0;

    virtual void GetMode( long all_samples, short bits, long frequency, ChannelType channel_info ) {}
    virtual void Read( Ptr into, long num_samples, long read_point, int pitch ) {}
};


};//namespace Useless
#endif//__INCLUDED_COOL_SOUND_INTERFACE_H__
