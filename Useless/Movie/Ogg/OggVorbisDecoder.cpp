/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2001             *
 * by the XIPHOPHORUS Company http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

function: simple example decoder
last mod: $Id: OggVorbisDecoder.cpp,v 1.17 2005/03/10 14:18:05 koolas Exp $

 ********************************************************************/

/* Takes a vorbis bitstream from stdin and writes raw stereo PCM to
   stdout.  Decodes simple and chained OggVorbis files from beginning
   to end.  Vorbisfile.a is somewhat more complex than the code below.  */

/* Note that this is POSIX, not ANSI code */

#include "UselessPch.h"


#include <math.h>

#include "OggVorbisDecoder.h"
#include "Useless/Sound/SampleStream.h"
#include "Useless/Sound/BasicSample.h"
#include "Useless/ErrorConfig.h"

#define OGG_ERR(e) APP_ERROR(e,"Ogg_Vorbis");
#define OGG_WRN(e) APP_WARNING(e,"Ogg_Vorbis");
#define OGG_LOG(e) SMALL_LOG(e,"Ogg_Vorbis");

namespace Useless {

    OggVorbisDecoder::~OggVorbisDecoder()
    {
        CleanUp();

        /* OK, clean up the framer */
        ogg_sync_clear(&oy);

        OGG_LOG("Done.\n");
    }

    OggVorbisDecoder::OggVorbisDecoder( IFile &_input_file )
        :_input_file(_input_file), convsize(4096), eos(0), eostream(0), continue_inner_loop(0), continue_inner_inner_loop(0)
        {
            Init();
        }

    OggVorbisDecoder::OggVorbisDecoder( IFile &_input_file, SoundCard &sound_card )
        :_input_file(_input_file), convsize(4096), eos(0), eostream(0), continue_inner_loop(0), continue_inner_inner_loop(0)
        {
            Init();
            Cooperate( sound_card );
        }

    void
        OggVorbisDecoder::Init()
        {
            _written_bytes = 0;

            /********** Decode setup ************/
            ogg_sync_init(&oy); /* Now we can read pages */

            while(1)
            { /* we repeat if the bitstream is chained */
                eos=0;
                int i;

                /* grab some data at the head of the stream.  We want the first page
                   (which is guaranteed to be small and only contain the Vorbis
                   stream initial header) We need the first page to get the stream
                   serialno. */

                /* submit a 4k block to libvorbis' Ogg layer */
                buffer=ogg_sync_buffer(&oy,4096);
                bytes=_input_file.ReadUpto(buffer,4096);
                ogg_sync_wrote(&oy,bytes);

                /* Get the first page. */
                if(ogg_sync_pageout(&oy,&og)!=1)
                {
                    /* have we simply run out of data?  If so, we're done. */
                    if(bytes<4096)
                    { break; }

                    /* error case.  Must not be Vorbis data */
                    OGG_ERR( Error("Input does not appear to be an Ogg bitstream.\n") );
                }

                /* Get the serial number and set up the rest of decode. */
                /* serialno first; use it to set up a logical stream */
                ogg_stream_init(&os,ogg_page_serialno(&og));

                /* extract the initial header from the first page and verify that the
                   Ogg bitstream is in fact Vorbis data */

                /* I handle the initial header first instead of just having the code
                   read all three Vorbis headers at once because reading the initial
                   header is an easy way to identify a Vorbis bitstream and it's
                   useful to see that functionality seperated out. */

                vorbis_info_init(&vi);
                vorbis_comment_init(&vc);
                if(ogg_stream_pagein(&os,&og)<0)
                { 
                    /* error; stream version mismatch perhaps */
                    OGG_ERR( Error("Error reading first page of Ogg bitstream data.\n") );
                }

                if(ogg_stream_packetout(&os,&op)!=1)
                { 
                    /* no page? must not be vorbis */
                    OGG_ERR( Error("Error reading initial header packet.\n") );
                }

                if(vorbis_synthesis_headerin(&vi,&vc,&op)<0)
                { 
                    /* error case; not a vorbis header */
                    OGG_ERR( Error("This Ogg bitstream does not contain Vorbis ") );
                }

                /* At this point, we're sure we're Vorbis.  We've set up the logical
                   (Ogg) bitstream decoder.  Get the comment and codebook headers and
                   set up the Vorbis decoder */

                /* The next two packets in order are the comment and codebook headers.
                   They're likely large and may span multiple pages.  Thus we reead
                   and submit data until we get our two pacakets, watching that no
                   pages are missing.  If a page is missing, error out; losing a
                   header page is the only place where missing data is fatal. */

                i=0;
                while(i<2)
                {
                    while(i<2)
                    {
                        int result=ogg_sync_pageout(&oy,&og);
                        if(result==0)
                        { break; }/* Need more data */
                        /* Don't complain about missing or corrupt data yet.  We'll
                           catch it at the packet output phase */
                        if(result==1)
                        {
                            ogg_stream_pagein(&os,&og); /* we can ignore any errors here
                                                           as they'll also become apparent
                                                           at packetout */
                            while(i<2)
                            {
                                result=ogg_stream_packetout(&os,&op);
                                if(result==0)
                                { break; }
                                if(result<0)
                                {
                                    /* Uh oh; data at some point was corrupted or missing!
                                       We can't tolerate that in a header.  Die. */
                                    OGG_ERR( Error("Corrupt secondary header.  Exiting.\n") );
                                }
                                vorbis_synthesis_headerin(&vi,&vc,&op);
                                i++;
                            }
                        }
                    }
                    /* no harm in not checking before adding more */
                    buffer=ogg_sync_buffer(&oy,4096);
                    bytes=_input_file.ReadUpto(buffer,1*4096);
                    if(bytes==0 && i<2)
                    {
                        OGG_ERR( Error("End of file before finding all Vorbis headers!\n") );
                    }
                    ogg_sync_wrote(&oy,bytes);
                }

                /* Throw the comments plus a few lines about the bitstream we're
                   decoding */
                {
                    char **ptr=vc.user_comments;
                    while(*ptr)
                    {
                        fprintf(stderr,"%s\n",*ptr);
                        ++ptr;
                    }
                    OGG_LOG( Error("\nBitstream is %d channel, %ldHz\n",vi.channels,vi.rate) );
                    OGG_LOG( Error("Encoded by: %s\n\n",vc.vendor) );
                }

                convsize=4096/vi.channels;

                /* OK, got and parsed all three headers. Initialize the Vorbis
                   packet->PCM decoder. */
                vorbis_synthesis_init(&vd,&vi); /* central decode state */
                vorbis_block_init(&vd,&vb);     /* local state for most of the decode
                                                   so multiple block decodes can
                                                   proceed in parallel.  We could init
                                                   multiple vorbis_block structures
                                                   for vd here */
                break;
                //return true;// tell that stream i ready
            };
            //return false;// tell that stream has finished
        };

    Snd::Properties
        OggVorbisDecoder::GetProperties () const
        {
            Snd::Properties properties;
            properties.sample_rate = vi.rate;
            properties.bits = 16;
            properties.channels = vi.channels;
            properties.length = vi.rate*10;// buffer for 10 seconds
            properties.pan_ctl = true;
            properties.pitch_ctl = true;
            properties.volume_ctl = true;
            return properties;
        }

    void
        OggVorbisDecoder::Cooperate( SoundCard &sound_card )
        {
            BasicSample sample( GetProperties() );
            Channel channel = sound_card.Play( sample, true, true ); // make sample redy for playback
            UseChannel( channel );
        }

    void
        OggVorbisDecoder::UseChannel( Channel &channel )
        {
            _channel = channel;
            _output_stream = SampleStream( _channel.GetPtr());// create stream interface for sample

            // W/A - clear sound buffer
            char *p; int s;
            _channel.GetPtr()->Lock( (void**)&p, &s);
            for( int i=0; i!=s; ++i, ++p ) { *p = 0; }
            _channel.GetPtr()->Unlock();
        }

    /* Update sample
       ------------------------------------------------------------------------*/
    bool OggVorbisDecoder::UpdateSample()
    {
        /* The rest is just a straight decode loop until end of stream */
        while(!eos)
        {
            while(!eos)
            {
                if ( continue_inner_inner_loop )
                {
                    continue_inner_inner_loop = InnerInnerLoop();
                    if ( !continue_inner_inner_loop )
                        continue_inner_loop = 1;
                }
                else if ( continue_inner_loop )
                {
                    continue_inner_loop = InnerLoop();
                }
                else
                {
                    int result=ogg_sync_pageout(&oy,&og);
                    if(result==0)
                    { break; }/* need more data */
                    if(result<0)
                    { /* missing or corrupt data at this page position */
                        OGG_WRN( Error("Corrupt or missing data in bitstream; continuing...\n") );
                    }
                    else
                    {
                        ogg_stream_pagein(&os,&og); /* can safely ignore errors at
                                                       this point */
                        if( continue_inner_loop = InnerLoop() ) { return true; }
                        if(ogg_page_eos(&og))eos=1;
                    }
                }
            }
            if(!eos)
            {
                buffer=ogg_sync_buffer(&oy,4096);
                bytes=_input_file.ReadUpto(buffer,1*4096);
                ogg_sync_wrote(&oy,bytes);
                if(bytes==0)eos=1;
            }
            if(!eos) { return true; }
        }
        return false;
    }

    void OggVorbisDecoder::CleanUp()
    {
        _auto_update_task.Free();

        /* clean up this logical bitstream; before exit we see if we're
           followed by another [chained] */

        ogg_stream_clear(&os);

        /* ogg_page and ogg_packet structs always point to storage in
           libvorbis.  They're never freed or manipulated directly */

        vorbis_block_clear(&vb);
        vorbis_dsp_clear(&vd);
        vorbis_comment_clear(&vc);
        vorbis_info_clear(&vi);  /* must be called last */
    }

    bool OggVorbisDecoder::InnerLoop()
    {
        int result, i;
        while(1)
        {
            result=ogg_stream_packetout(&os,&op);

            if(result==0)
            { break; }/* need more data */
            if(result<0)
            { /* missing or corrupt data at this page position */
                /* no reason to complain; already complained above */
            }else{
                /* we have a packet.  Decode it */
                if(vorbis_synthesis(&vb,&op)==0) /* test for success! */
                    vorbis_synthesis_blockin(&vd,&vb);

                if ( continue_inner_inner_loop = InnerInnerLoop() )
                {
                    return false;
                }                
            }
            return true;
        }
        return false;
    }
    
    bool OggVorbisDecoder::InnerInnerLoop()
    {
        float **pcm;
        int samples;
        /* 
         **pcm is a multichannel float vector.  In stereo, for
         example, pcm[0] is left, and pcm[1] is right.  samples is
         the size of each channel.  Convert the float values
         (-1.<=range<=1.) to whatever PCM format and write it out */

        while((samples=vorbis_synthesis_pcmout(&vd,&pcm))>0)
        {
            int j;
            int clipflag=0;
            int bout=(samples<convsize?samples:convsize);

            /* convert floats to 16 bit signed ints (host order) and
               interleave */
            for(int i=0;i<vi.channels;i++)
            {
                ogg_int16_t *ptr=convbuffer+i;
                float  *mono=pcm[i];
                for(j=0;j<bout;j++)
                {
#if 1
                    int val=mono[j]*32767.f;
#else /* optional dither */
                    int val=mono[j]*32767.f+drand48()-0.5f;
#endif
                    /* might as well guard against clipping */
                    if(val>32767)
                    {
                        val=32767;
                        clipflag=1;
                    }
                    if(val<-32768)
                    {
                        val=-32768;
                        clipflag=1;
                    }
                    *ptr=val;
                    ptr+=vi.channels;
                }
            }

            if(clipflag)
                fprintf(stderr,"Clipping in frame %ld\n",(long)(vd.sequence));

            size_t bytes_to_write = 2*vi.channels*bout;
            size_t free = _output_stream.Free();
            if ( free > bytes_to_write + 4 )
            {
                _output_stream.Write( convbuffer, bytes_to_write );
                _written_bytes += bytes_to_write;

                vorbis_synthesis_read(&vd,bout); /* tell libvorbis how
                                                    many samples we
                                                    actually consumed */
            }
            else
            {
                vorbis_synthesis_read(&vd,0); /* tell libvorbis there was no space for samples */
                return true;
            }
        }
        return false;
    }

    void OggVorbisDecoder::Play()
    {
        Prepare();

        GetChannel().Play(true);

        DelayedExecutor::TaskInfo timed;
        timed.p_func = PackFunctor( VoidCall(this, &OggVorbisDecoder::UpdateBlock) );

        timed.delay = 0.5;
        timed.repeats = DelayedExecutor::ALLWAYS;
        _auto_update_task = OnTimer::Instance().Schedule(timed);
        eostream = false;

        UpdateBlock();
    }

    void OggVorbisDecoder::Prepare()
    {
        assert( !GetChannel().IsPlaying() );

        UpdateSample();
        while ( InnerLoop() ) {}
    }

    void OggVorbisDecoder::Stop()
    {
        if ( GetChannel().IsValid() )
        { GetChannel().Stop(); }
    }
    
    void OggVorbisDecoder::UpdateBlock()
    {
        if ( !eos )
        {
            Snd::Properties sndProp = GetChannel().GetPtr()->GetProperties();
            size_t full_space = (sndProp.length * sndProp.channels * (sndProp.bits/8));
            int min_space = full_space / 2;
            if ( !eos )
            {
                while( (_output_stream.Free() > min_space) && UpdateSample())
                {}
            }
            if ( eos ) //-- stream has finished
            {
                char *ptr;
                int   size;
                int   pos = _output_stream.GetPos();
                SampleData *sampleData = GetChannel().GetPtr();
                sampleData->Lock( (void**)&ptr, &size );
                while ( pos < size ) // fill rest of buffer with zeros
                {
                    ptr[pos++] = 0;
                }
                sampleData->Unlock();
                GetChannel().Play(false);
            }
        }
        else
        {
            if ( !GetChannel().IsPlaying() ||
                    _output_stream.Played() >= _output_stream.GetPos() )
            {
                GetChannel().Stop();
                CleanUp();
                eostream = true;
            }
        }
    }

};//namespace Useless
