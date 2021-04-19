#ifndef __INCLUDED_USELESS_CAUDIO_RENDER_H__
#define __INCLUDED_USELESS_CAUDIO_RENDER_H__

#include "Useless/UselessConfig.h"
#include "Useless/Sound/Device/SampleData.h"
#include "Useless/Sound/SampleStream.h"
#include "Useless/Util/Shares.h"

#include "strmbase/streams.h"

namespace Useless{

struct USELESS_API __declspec(uuid("{7E7B5575-911E-449c-81EA-FD3D2B1450D4}")) CLSID_AudioRenderer;

class USELESS_API CAudioRender : public CBaseRenderer
{
public:
    CAudioRender();
    ~CAudioRender();

    virtual HRESULT CheckMediaType(const CMediaType *media_type);
    virtual HRESULT SetMediaType(const CMediaType *media_type);
    virtual HRESULT DoRenderSample(IMediaSample *media_sample);
    virtual HRESULT OnStartStreaming(void);
    virtual HRESULT OnStopStreaming(void);

    void SetSampleData( SampleData *sd);

private:
    Share<SampleData> _sample_data;
    int _sample_rate;
    int _buffer_size;
    int _bits_per_sample;
    int _num_channels;
    bool _running;

    SampleStream      _stream;
    HRESULT _hr;

    friend class DSMedia;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_CAUDIO_RENDER_H__
