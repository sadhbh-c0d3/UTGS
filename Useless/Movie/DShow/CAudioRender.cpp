#include "UselessPch.h"

#include "CAudioRender.h"
#include "Useless/Util/Math.h"

#include <set>

namespace Useless {


typedef std::set< SampleData * > MoviesAudio;
MoviesAudio g_MoviesAudio;
CCritSec    g_CritSecAudio;

void PutMovieAudio( SampleData *sample )
{
    CAutoLock guard( &g_CritSecAudio );
    g_MoviesAudio.insert( sample );
}

void RemoveMovieAudio( SampleData *sample )
{
    CAutoLock guard( &g_CritSecAudio );
    MoviesAudio::iterator found = g_MoviesAudio.find( sample );
    if ( found != g_MoviesAudio.end() )
    {
        g_MoviesAudio.erase( found );
    }
    sample->Stop();
}

void UpdateMovieAudio()
{
    CAutoLock guard( &g_CritSecAudio );
    for ( MoviesAudio::iterator itMA = g_MoviesAudio.begin(); itMA != g_MoviesAudio.end(); ++itMA )
    {
        (*itMA)->Play( true );
    }
    g_MoviesAudio.clear();
}
    
CAudioRender::CAudioRender()
    :_sample_rate(0),
     _buffer_size(0),
     _bits_per_sample(0),
     _num_channels(0),
     CBaseRenderer(__uuidof(CLSID_AudioRenderer),
                                   NAME("Audio Renderer"), NULL, &_hr)
{
     _hr = S_OK;
}

CAudioRender::~CAudioRender()
{
    RemoveMovieAudio( _sample_data.GetTarget() );
}

HRESULT CAudioRender::CheckMediaType(const CMediaType *media_type)
{
    WAVEFORMATEX *wav_fmt;
    if ( *media_type->FormatType() == FORMAT_WaveFormatEx)
    {
        wav_fmt = (WAVEFORMATEX*)media_type->Format();
        if ( IsEqualGUID( *media_type->Type(), MEDIATYPE_Audio) &&
             IsEqualGUID( *media_type->Subtype(), MEDIASUBTYPE_PCM))
        {
            return S_OK;
        }
    }
    return E_INVALIDARG;
}

HRESULT CAudioRender::SetMediaType(const CMediaType *media_type)
{
    WAVEFORMATEX *wav_fmt;
    wav_fmt = (WAVEFORMATEX*)media_type->Format();
    _sample_rate = wav_fmt->nSamplesPerSec;
    _bits_per_sample = wav_fmt->wBitsPerSample;
    _num_channels = wav_fmt->nChannels;

    _buffer_size = 4 * media_type->lSampleSize;

    return S_OK;
}

HRESULT CAudioRender::DoRenderSample(IMediaSample *media_sample)
{
    try {
        if ( !!_sample_data)
        {
            BYTE *data;
            int size = media_sample->GetActualDataLength();
            media_sample->GetPointer( &data);
            while (!_stream.Ready( 2 * size ))
            {
                Sleep(5);
            }
            _stream.Write( data, size );
            if ( !_sample_data->IsPlaying() )
            {
                PutMovieAudio( _sample_data.GetTarget() );
            }
            return S_OK;
        }
    } catch(...) {}
    
    return E_FAIL;
}

HRESULT CAudioRender::OnStartStreaming(void)
{
    _stream = SampleStream( _sample_data.GetTarget() );
    return S_OK;
}

HRESULT CAudioRender::OnStopStreaming(void)
{
    _sample_data->Stop();
    RemoveMovieAudio( _sample_data.GetTarget() );
    return S_OK;
}

void CAudioRender::SetSampleData( SampleData *sd)
{
    _sample_data.SetTarget( sd);
    
    void *data;
    int size;
    _sample_data->Lock( &data, &size);
    memset( data, 0L, size);
    _sample_data->Unlock();
}
    
};//namespace Useless
