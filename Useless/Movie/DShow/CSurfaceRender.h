#ifndef __INCLUDED_USELESS_CSURFACE_RENDER_H__
#define __INCLUDED_USELESS_CSURFACE_RENDER_H__

#include "Useless/UselessConfig.h"
#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Util/Shares.h"

#include "strmbase/streams.h"

namespace Useless{

struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_SurfaceRenderer;

class USELESS_API CSurfaceRender : public CBaseVideoRenderer
{
public:
    CSurfaceRender();
    ~CSurfaceRender();

    virtual HRESULT CheckMediaType(const CMediaType *media_type);
    virtual HRESULT SetMediaType(const CMediaType *media_type);
    virtual HRESULT DoRenderSample(IMediaSample *media_sample);
    
    virtual HRESULT OnStartStreaming(void);
    virtual HRESULT OnStopStreaming(void);

private:
    Share<Surface> _surface;

    int _width;
    int _height;
    double _fps;
    HRESULT _hr;
    CAMEvent _eventSample;
    CCritSec _mutex;

    friend class DSMedia;
    
public:
    static int _prefered_bpp;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_CSURFACE_RENDER_H__
