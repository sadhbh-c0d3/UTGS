#ifndef __INCLUDED_COOL_WAVEHEADER_H__
#define __INCLUDED_COOL_WAVEHEADER_H__

#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"

namespace Useless {

class WaveHeader
{
public:
        static int Size() { return 44; }
        void      Load( IFile &ifile)
        {
                                   ifile.Read( riff_string, 4);
                data_length      = ifile.ReadLongLE();
                                   ifile.Read( format_string, 8);
                block_length     = ifile.ReadLongLE();
                format_tag       = ifile.ReadWordLE();
                channels         = ifile.ReadWordLE();
                frequency        = ifile.ReadLongLE();
                sample_per_sec   = ifile.ReadLongLE();
                bytes_per_sample = ifile.ReadWordLE();
                bits_per_sample  = ifile.ReadWordLE();
                                   ifile.Read( data_string, 4);
                data_size        = ifile.ReadLongLE();
        }
        void      Save( OFile &ofile)
        {
                ofile.Write( &riff_string, 4);
                ofile.WriteLongLE(data_length);
                ofile.Write( &format_string, 8);
                ofile.WriteLongLE(block_length);
                ofile.WriteWordLE(format_tag);
                ofile.WriteWordLE(channels);
                ofile.WriteLongLE(frequency);
                ofile.WriteLongLE(sample_per_sec);
                ofile.WriteWordLE(bytes_per_sample);
                ofile.WriteWordLE(bits_per_sample);
                ofile.Write( &data_string, 4);
                ofile.WriteLongLE(data_size);
        }

        Byte      riff_string[4];
        UDWord    data_length;
        Byte      format_string[8];
        UDWord    block_length;
        UWord     format_tag;
        UWord     channels;
        UDWord    frequency;
        UDWord    sample_per_sec;
        UWord     bytes_per_sample;
        UWord     bits_per_sample;
        Byte      data_string[4];
        UDWord    data_size;
};


};//namespace Useless
#endif//__INCLUDED_COOL_WAVEHEADER_H__
