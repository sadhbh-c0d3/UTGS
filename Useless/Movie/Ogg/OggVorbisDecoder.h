#ifndef __INCLUDED__DECODER_H__
#define __INCLUDED__DECODER_H__

#include "Useless/UselessConfig.h"
#include "Useless/File/IFile.h"
#include "Useless/Sound/Device/SoundCard.h"
#include "Useless/Sound/SampleStream.h"
#include "Useless/Functional/DelayedExecutor.h"

#include <vorbis/codec.h>

#ifdef NDEBUG
#   pragma comment (lib,"vorbis.lib")
#   pragma comment (lib,"ogg.lib")
#else
#   pragma comment (lib,"vorbis_d.lib")
#   pragma comment (lib,"ogg_d.lib")
#endif

namespace Useless {

/*! Decode Ogg Vorbis stream
*/
class USELESS_API OggVorbisDecoder
{
public:
    OggVorbisDecoder( IFile &input_file );
    OggVorbisDecoder( IFile &input_file, SoundCard &sound_card );
    ~OggVorbisDecoder();

    Snd::Properties GetProperties () const;
    void Cooperate ( SoundCard &sound_card );
    void UseChannel( Channel &channel );

    void Play();
    void Stop();

   
    bool EoS() const { return eostream; }

    void Prepare();
    void UpdateBlock();
    bool UpdateSample();
    size_t BytesWritten() const { return _written_bytes; }

    Channel& GetChannel() { return _channel; }

private:
    IFile        &_input_file;
    Channel       _channel;
    SampleStream  _output_stream;
    size_t        _written_bytes;
    
    void Init();    

    ogg_sync_state   oy; /* sync and verify incoming physical bitstream */
    ogg_stream_state os; /* take physical pages, weld into a logical
		      stream of packets */
    ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
    ogg_packet       op; /* one raw packet of data for decode */

    vorbis_info      vi; /* struct that stores all the static vorbis bitstream
		      settings */
    vorbis_comment   vc; /* struct that stores all the bitstream user comments */
    vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
    vorbis_block     vb; /* local working space for packet->PCM decode */

    char *buffer;
    int  bytes;

    ogg_int16_t convbuffer[4096]; /* take 8k out of the data segment, not the stack */
    int convsize;

    bool  eos;
    bool  eostream;
    bool  continue_inner_loop;
    bool  continue_inner_inner_loop;

    bool InnerInnerLoop();
    bool InnerLoop();
    void CleanUp();

    DelayedExecutor::TaskID _auto_update_task;
};


};//namespace Useless
#endif//__INCLUDED__DECODER_H__
