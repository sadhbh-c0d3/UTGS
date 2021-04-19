#ifndef __INCLUDED__USELESS_SAMPLE_WRITER_H__
#define __INCLUDED__USELESS_SAMPLE_WRITER_H__

#include "Useless/Sound/Sample.h"
#include "Useless/Sound/FileIO/SoundInterface.h"

namespace Useless {

class USELESS_API SampleWriter : public OSoundInterface
{
public:
    SampleWriter( SampleData &sample_data );
    virtual ~SampleWriter();

    virtual void SetMode( long all_samples, short bits, long frequency, ChannelType channel_info );
    virtual void Write( ConstPtr from, long num_samples, long insertion_point, int pitch );

private:
    SampleData      &_sample_data;
    Snd::Properties  _properties;
    bool             _need_convertion;
    ChannelType      _channel_type;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_SAMPLE_WRITER_H__
