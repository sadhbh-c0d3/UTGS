#include "UselessPch.h"

#include "SampleWriter.h"
//#include "Useless/Sound/SampleType.h" - for fast convertion

namespace Useless {

SampleWriter::SampleWriter( SampleData &sample_data ): _sample_data(sample_data)
{
    _properties = _sample_data.GetProperties();
}

SampleWriter::~SampleWriter()
{}

void SampleWriter::Write( ConstPtr from, long num_samples, long insertion_point, int pitch )
{
    assert(!_need_convertion); // Convertion is unsupported this time

    int bytes_per_channel= (_properties.bits)/8 ;
    int bytes_per_sample = bytes_per_channel * _properties.channels;
    int bytes_to_read = bytes_per_channel * ((_channel_type==INTERLEAVE)? 2 : 1);
    int bytes_pitch = bytes_per_channel * ((_channel_type==SINGLE)? 1 : 2);

    char *p; int size; const char *q = (const char *)from;

    _sample_data.Lock( (void**)&p, &size );

    p += insertion_point*bytes_per_sample + ( (_channel_type==RIGHT)? bytes_per_channel : 0 );
   
    switch(bytes_to_read)
    {
    case 1:
        while( num_samples-- )
        {
            *((char*)p) = *((char*)q);
            q += pitch;
            p += bytes_pitch;
        }
        break;

    case 2:
        while( num_samples-- )
        {
            *((short*)p) = *((short*)q);
            q += pitch;
            p += bytes_pitch;
        }
        break;

    case 4:
        while( num_samples-- )
        {
            *((long*)p) = *((long*)q);
            q += pitch;
            p += bytes_pitch;
        }
        break;
    }

    _sample_data.Unlock();
}

void SampleWriter::SetMode( long all_samples, short bits, long frequency, ChannelType channel_info )
{
    int channels = (channel_info==SINGLE)? 1 : 2;
    _channel_type = channel_info;

    _need_convertion = !(all_samples==_properties.length) ||
                       !(bits==_properties.bits)          ||
                       !(frequency==_properties.sample_rate) ||
                       !(channels==_properties.channels );
}

};//namespace Useless
