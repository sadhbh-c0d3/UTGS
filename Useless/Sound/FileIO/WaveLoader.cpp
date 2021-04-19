#include "UselessPch.h"

#include "WaveLoader.h"
#include "Useless/Sound/detail/WaveHeader.h"
#include <assert.h>

namespace Useless {

    const int wave_transport_unit = 512;

    void WaveLoader::Load( IFile &file, OSoundInterface &sound_writer)
    {
        WaveHeader header;
        header.Load(file);
        assert( header.bits_per_sample == 16 || header.bits_per_sample == 8);
        int  bits = header.bits_per_sample;
        long freq = header.frequency;
        ChannelType channel_type = (header.channels==1)? SINGLE : INTERLEAVE;
        long all_samples  = header.data_size / ( header.channels*(header.bits_per_sample/8) );

        sound_writer.SetMode( all_samples, bits, freq, channel_type);

        Ptr memblock = new char[wave_transport_unit];

        try
        { 
            int k=0;
            for ( int i=0; i<(long)header.data_size; i+=wave_transport_unit)
            {
                int read_bytes  = file.ReadUpto( memblock, wave_transport_unit);
                int num_samples = header.bits_per_sample==16 ? read_bytes/2 : read_bytes;
                num_samples/= header.channels;
                int pitch = header.bytes_per_sample;
                if (k + num_samples > all_samples) { num_samples = all_samples - k; }
                sound_writer.Write( memblock, num_samples, k, pitch);
                k += num_samples;
            }
        }
        catch(...)
        {
            delete[] (char*)memblock;
            throw;
        }

        delete[] (char*)memblock;
    }

    bool WaveLoader::Recognize( IFile &file, Snd::Format *format )
    {
        WaveHeader header;
        header.Load(file);
        if (format)
        {
            format->sample_rate = header.frequency;
            format->bits = header.bits_per_sample;
            format->channels = header.channels;
            format->length = header.data_size / ( header.channels*(header.bits_per_sample/8) );
        }

        return ( header.bits_per_sample==8 && header.bytes_per_sample==header.channels ||
                header.bits_per_sample==16 && header.bytes_per_sample==2*header.channels ) &&
            ( header.channels==1 || header.channels==2 ) &&
            *((int*)header.riff_string) == *((int*)"RIFF") &&
            *((int*)header.data_string) == *((int*)"data");
    }

};//namespace Useless
