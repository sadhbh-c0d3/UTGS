#include "UselessPch.h"

#ifndef USELESS_HAS_DSHOW
#   error  "USELESS_HAS_DSHOW is not defined. Please, exclude appropriate files from build."
#endif

#include "DSMedia.h"

#include "Useless/ErrorConfig.h"
#include "Useless/File/VIFS.h"
#include <assert.h>

#define ERROR_IF( cond, error) \
    if ( cond )\
    {\
		TCHAR buffer[256];\
        AMGetErrorText( cond, buffer, 256);\
        if ( cond==0x40242)\
        {\
            APP_WARNING( Error( "File \"%s\": %s: %s", file_name.c_str(), error, buffer), "DSMedia");\
        }\
        else {\
            APP_ERROR( Error( "%s: %s", error, buffer), "DSMedia");\
        }\
    }

namespace Useless {

DSMedia::DSMedia( const char *file_name_chars, unsigned int media_type_flags)
{
    Init( file_name_chars, media_type_flags, L"Stream 00", L"Stream 01");
}

DSMedia::DSMedia( const std::string &fileName, const std::wstring &audioPinName )
{
    Init( fileName, VIDEO + AUDIO, L"Stream 00", audioPinName );
}

void DSMedia::Init( std::string fileName, unsigned int mediaTypeFlags, std::wstring videoPinName, std::wstring audioPinName )
{
    long error_code=0, success_code=1;
    bool video = mediaTypeFlags & VIDEO;
    bool audio = mediaTypeFlags & AUDIO;

    const std::string &file_name( fileName );
    std::string lower_name( file_name );
    std::transform( lower_name.begin(), lower_name.end(), lower_name.begin(), tolower );

    // Detect media type by extention [IDIOTIC MS]
    CMediaType mt;
    mt.majortype = MEDIATYPE_Stream;
    int file_ext_pos = file_name.size()-4;
    
    if ( lower_name.rfind(".mpg", -1) == file_ext_pos )
    {
        mt.subtype = MEDIASUBTYPE_MPEG1System;
    }
    else if ( lower_name.rfind(".mpv", -1) == file_ext_pos )
    {
        mt.subtype = MEDIASUBTYPE_MPEG1Video;
    }
    else if ( lower_name.rfind(".avi", -1) == file_ext_pos )
    {
        mt.subtype = MEDIASUBTYPE_Avi;
    }
    else if ( lower_name.rfind(".mov", -1) == file_ext_pos )
    {
        mt.subtype = MEDIASUBTYPE_QTMovie;
    }
    else if ( lower_name.rfind(".mp3", -1) == file_ext_pos )
    {
        mt.subtype = MEDIASUBTYPE_MPEG1Audio;
    }
    else
    {
        APP_ERROR( Error("File \"%s\": File format unknown", file_name.c_str()), "DSMedia");
    }
    
    // CREATE FILTER GRAPH
    _graph.Create( CLSID_FilterGraph, CLSCTX_INPROC);
    ERROR_IF( !_graph, "Failed to create filter graph");
    
    // SOURCE
    SPointer<IFile> input_avi = IFS::Instance().OpenFile( file_name);
    _stream = new CFileStream( input_avi, 4);
    _src_filter.Assign( new CMemReader( _stream.get(), &mt, &error_code));
    
    error_code = _graph->AddFilter( _src_filter.get(), NULL);
    ERROR_IF( error_code, "Failed to add source filter to graph");
    
    COMObject<IPin> src_out;
    error_code = _src_filter->FindPin(L"Output", &src_out);
    ERROR_IF( error_code, "Could not find output pin");
    
    COMObject<IPin> audio_src;
    COMObject<IPin> video_src;

    if ( video && audio)
    {
        COMObject<IBaseFilter> splitter( CLSID_AviSplitter, CLSCTX_INPROC);
        COMObject<IEnumPins> it_pins;
        COMObject<IPin> pin;
        ULONG fetched = 1;
        
        error_code = _graph->AddFilter( splitter.get(), NULL);
        ERROR_IF( error_code, "Failed to add AVI splitter filter to graph");

        error_code = splitter->FindPin(L"input pin", &pin);
        ERROR_IF( error_code, "Could not find input pin (avi splitter)");
        
        error_code = _graph->Connect(src_out.get(), pin.get());
        ERROR_IF( error_code, "Could not connect pins (avi splitter to source)");
        
        pin.Release();

        error_code = splitter->FindPin( videoPinName.c_str(), &video_src);
        if ( error_code )
        {
            video = false;
            //C_xlogf( DSMedia, 1, false, ("Could not find video output pin (avi splitter)"));
        }
        
        error_code = splitter->FindPin( audioPinName.c_str(), &audio_src);
        if ( error_code )
        {
            audio = false;
            //C_xlogf( DSMedia, 1, false, ( "Could not find audio output pin (avi splitter)" ));
        }
    }
    else {
        audio_src = src_out;
        video_src = src_out;
    }
    
    // VIDEO OUT
    if ( video )
    {
        _dest_filter.Assign( new CSurfaceRender());
        _surface = _dest_filter->_surface;

        error_code = _graph->AddFilter( _dest_filter.get(), NULL);
        ERROR_IF( error_code, "Failed to add filter to graph for video-renderer");

        COMObject<IPin> dest_in;

        error_code = _dest_filter->FindPin(L"In", &dest_in);
        ERROR_IF( error_code, "Could not find input pin of video-renderer");

        error_code = _graph->Connect(video_src.get(), dest_in.get());
        ERROR_IF( error_code, "Could not connect pin of video-renderer");
        
        _width=_dest_filter->_width;
        _height=_dest_filter->_height;
        _fps=_dest_filter->_fps;
    }

    // AUDIO OUT
    if ( audio )
    {
        _audio_filter.Assign( new CAudioRender());
        _sample_data= _audio_filter->_sample_data;
        
        error_code = _graph->AddFilter( _audio_filter.get(), NULL);
        ERROR_IF( error_code, "Failed to add filter to graph for audio-renderer");
    
        COMObject<IPin> audio_in;

        error_code = _audio_filter->FindPin(L"In", &audio_in);
        ERROR_IF( error_code, "Could not find input pin of audio-renderer");

        error_code = _graph->Connect(audio_src.get(), audio_in.get());
        ERROR_IF( error_code, "Could not connect pin of audio-renderer");

        _sample_rate=_audio_filter->_sample_rate;
        _buffer_size=_audio_filter->_buffer_size;
        _bits_per_sample=_audio_filter->_bits_per_sample;
        _num_channels=_audio_filter->_num_channels;
    }
    

    // PLAYBACK CONTROL
    _media_control = COM_cast<IMediaControl>(_graph);
    ERROR_IF( !_media_control, "Failed to get media control");
    
    _media_position= COM_cast<IMediaPosition>(_graph);
    ERROR_IF( !_media_position, "Failed to get media position");
    
    _media_event = COM_cast<IMediaEvent>(_graph);
    ERROR_IF( !_media_event, "Failed to get media event");
}

DSMedia::~DSMedia()
{
    _media_control->Stop();
    
    long error_code = 0;
    std::string file_name("~DSMedia");
    COMObject<IPin> pin;

    if ( _dest_filter.get() )
    {
        error_code = _dest_filter->FindPin(L"In", &pin);
        ERROR_IF( error_code, "Could not find input pin of video-renderer");
        error_code = pin->Disconnect();
        ERROR_IF( error_code, "Could not disconnect pin of video-renderer");
        pin.Release();
        error_code = _graph->RemoveFilter( _dest_filter.get() );
        ERROR_IF( error_code, "Could not remove video-renderer");
        _dest_filter.Release();
    }
    if ( _audio_filter.get() )
    {
        error_code = _audio_filter->FindPin(L"In", &pin);
        ERROR_IF( error_code, "Could not find input pin of audio-renderer");
        error_code = pin->Disconnect();
        ERROR_IF( error_code, "Could not disconnect pin of audio-renderer");
        pin.Release();
        error_code = _graph->RemoveFilter( _audio_filter.get() );
        ERROR_IF( error_code, "Could not remove audio-renderer");
        _audio_filter.Release();
    }
    if ( _src_filter.get() )
    {
        error_code = _src_filter->FindPin(L"Output", &pin );
        ERROR_IF( error_code, "Could not find output pin of file-reader");
        error_code = pin->Disconnect();
        ERROR_IF( error_code, "Could not disconnect pin of file-reader");
        pin.Release();
        _graph->RemoveFilter( _src_filter.get() );
        ERROR_IF( error_code, "Could not remove file-reader");
        _src_filter.Release();
        _stream.Release();
    }
    
    try {
        _media_control.Release();
        _media_event.Release();
        _media_position.Release();
        _graph.Release();
    }
    catch(...) {
        C_xlogf( DSMedia, 2, false, ("IFilterGraph was not released succesfully"));
    }
}

void DSMedia::Play()
{
    _media_control->Run();
}

void DSMedia::Pause()
{
    _media_control->Pause();
}

void DSMedia::Stop()
{
    _media_control->Stop();
}

bool DSMedia::IsPlaying()
{
    OAFilterState state;
    _media_control->GetState( INFINITE, &state);
    return ( state==State_Running );
}

bool DSMedia::IsPaused()
{
    OAFilterState state;
    _media_control->GetState( INFINITE, &state);
    return ( state==State_Paused );
}

double DSMedia::GetTime() const
{
    REFTIME time;
    _media_position->get_CurrentPosition( &time);
    return time;
}

double DSMedia::GetDuration() const
{
    REFTIME time;
    _media_position->get_Duration( &time);
    return time;
}

void DSMedia::SetTime( double time)
{
    _media_position->put_CurrentPosition( time);
}


void DSMedia::Cooperate( const Surface &reference)
{
    if ( IsVideo() )
    {
        Surf::Properties prop;
        prop.width=_dest_filter->_width;
        prop.height=_dest_filter->_height;
        prop.pixelformat = ImageFormat::Format( CSurfaceRender::_prefered_bpp);
        _surface.SetTarget( reference.CreateSurface(prop));
    }
}   

void DSMedia::Cooperate( const SoundCard &reference)
{
    if( IsAudio() )
    {
        Snd::Properties prop;
        prop.sample_rate = _audio_filter->_sample_rate;
        prop.channels = _audio_filter->_num_channels;
        prop.bits = _audio_filter->_bits_per_sample;
        prop.length = _audio_filter->_buffer_size;
        prop.volume_ctl = true;
        prop.pan_ctl = true;
        prop.global_focus = true;
        _audio_filter->SetSampleData( reference.CreateData(prop));
    }
}   

    
};//namespace Useless
