#include "UselessPch.h"

#include "WaveSaver.h"
#include "Useless/Sound/detail/WaveHeader.h"
#include <assert.h>
#include <string>

namespace Useless {

const int wave_transport_unit = 512;

void WaveSaver::Save( OFile &file, SoundInterface &sound_surface)
{
        WaveHeader header;
        long all_samples, frequency;
        int bits;
        bool stereo;
        ConstPtr data = sound_surface.Lock( &all_samples, &bits, &frequency, &stereo );

        memcpy( header.riff_string, "RIFF", 4);
        memcpy( header.format_string, "WAVEfmt ", 8);
        memcpy( header.data_string, "data", 4);
        header.bits_per_sample  = bits;
        header.frequency        = frequency;
        header.sample_per_sec   = frequency;
        header.channels         = 1+stereo;
        header.bytes_per_sample = (bits/8)*(1+stereo);
        header.block_length     = 16;
        header.data_size        = all_samples*header.bytes_per_sample;
        header.data_length      = header.data_size+36;
        header.format_tag       = 1;
        header.Save(file);

        long remaining = header.data_size;

                for ( int i=0; i<(long)header.data_size; i+=wave_transport_unit)
                {
                    long size = wave_transport_unit < remaining ? wave_transport_unit : remaining;
                    remaining -= file.WriteUpto( (ConstPChar)data + i, size);
                }

        sound_surface.Unlock();
}

};//namespace Useless
