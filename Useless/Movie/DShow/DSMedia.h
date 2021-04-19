#ifndef __INCLUDED_USELESS_DS_MEDIA_H__
#define __INCLUDED_USELESS_DS_MEDIA_H__

#include "Useless/UselessConfig.h"
#include "Useless/System/w32/COMInitializer.h"
#include "Useless/System/w32/COMObject.h"
#include "Useless/Movie/DShow/CSurfaceRender.h"
#include "Useless/Movie/DShow/CAudioRender.h"
#include "Useless/Sound/Device/SoundCard.h"

#include "strmbase/streams.h"
#include "Useless/Movie/DShow/detail/asyncio.h"
#include "Useless/Movie/DShow/detail/asyncrdr.h"
#include "Useless/Movie/DShow/detail/memstrm.h"
#include "Useless/Movie/DShow/detail/filestrm.h"

#pragma comment (lib,"strmiids.lib")
#pragma comment (lib,"Quartz.lib") //-- for AMGetErrorText()

/*#ifdef NDEBUG
#   pragma comment (lib,"strmbase.lib")
#else
#   pragma comment (lib,"strmbasd.lib")
#endif*/

namespace Useless {

class USELESS_API DSMedia
{
public:
    enum MediaType { VIDEO=1, AUDIO=2 };
    DSMedia( const char *file_name, unsigned int media_type_flags );
    DSMedia( const std::string &fileName, const std::wstring &audioPinName );
    ~DSMedia();

    void Play();
    void Pause();
    void Stop();
    
    bool IsPlaying();
    bool IsPaused();
    
    double GetTime() const;
    double GetDuration() const;
    void   SetTime( double time);
    
    bool IsVideo() const { return !!_dest_filter; }
    int GetWidth() const { return _width;}
    int GetHeight() const{ return _height;}
    double GetFPS() const{ return _fps;}
    
    
    bool IsAudio() const { return !!_audio_filter;}
    int GetSampleRate() const { return _sample_rate;}
    int GetBufferSize() const { return _buffer_size;}
    int GetBits()       const { return _bits_per_sample;}
    int GetNumChannels() const{ return _num_channels;}

   
    void Cooperate( const Surface &surface);
    void Cooperate( const SoundCard &surface);

    Share<Surface>            _surface;
    Share<SampleData>         _sample_data;

private:
    COMObject<IGraphBuilder>  _graph;
    COMObject<IMediaControl>  _media_control;
    COMObject<IMediaPosition> _media_position;
    COMObject<IMediaEvent>    _media_event;
    COMObject<CMemReader>     _src_filter;
    COMObject<CSurfaceRender> _dest_filter;
    COMObject<CAudioRender>   _audio_filter;
    SPointer<CAsyncStream>    _stream;
    DSMedia(const DSMedia &);
    DSMedia& operator=(const DSMedia &);
    void Init( std::string fileName, unsigned int mediaTypeFlags, std::wstring videoPinName, std::wstring audioPinName );

    int _width;
    int _height;
    double _fps;

    int _sample_rate;
    int _buffer_size;
    int _bits_per_sample;
    int _num_channels;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_DS_MEDIA_H__
