#include "UselessPch.h"

#include "CSurfaceRender.h"

#include "Useless/Graphic/Device/GL/GLTextureSurfaceAllocator.h"

namespace Useless {

int CSurfaceRender::_prefered_bpp = 32;

typedef std::map< Surface *, std::pair< IMediaSample *, CAMEvent *> > MovieSamples;

MovieSamples g_MovieSamples;
CCritSec     g_CritSec;

void UpdateMovieAudio();

void SetMovieSample( Surface *pSurface, IMediaSample *pSample, CAMEvent *pEvent )
{
    CAutoLock guard( &g_CritSec );
    g_MovieSamples[ pSurface ] = std::make_pair( pSample, pEvent );
    if ( 0 != pEvent )
    {
        pEvent->Reset();
    }
}

void UnsetMovieSample( Surface *pSurface )
{
    CAutoLock guard( &g_CritSec );
    MovieSamples::iterator found = g_MovieSamples.find( pSurface );
    if ( found != g_MovieSamples.end() )
    {
        CAMEvent *pEvent = (*found).second.second;
        g_MovieSamples.erase( found );
        pEvent->Set();
    }
}

////////////////////////////////
//
// OpenGL version of UpdateMovieSample
//
// 
void GLUpdateMovieSample( GLTextureSurfaceAllocator *pTexAlloc, GLTextureSurface *pSurface, IMediaSample *pSample )
{
    if ( !pSurface )
    {
        throw Error("GLUpdateMovieSamples: Non-GLTextureSurface.");
    }
    int width  = pSurface->GetWidth();
    int height = pSurface->GetHeight();
    BYTE *pData;
    pSample->GetPointer( &pData );
    GLImageTesselator &tesselator = pTexAlloc->GetTesselator( pSurface );
    tesselator.UpdateAllTextures( (char*)pData, 0, 0, width, height, CSurfaceRender::_prefered_bpp, width );
    tesselator.SetFlip( false, true );
    pTexAlloc->ReleaseTesselator( pSurface, tesselator );
}

void GLUpdateMovieSamples( GLTextureSurfaceAllocator *pTexAlloc )
{
    CAutoLock guard( &g_CritSec );
    pTexAlloc->MakeCurrent();
    for ( MovieSamples::iterator itMS = g_MovieSamples.begin(); itMS != g_MovieSamples.end(); ++itMS )
    {
        GLTextureSurface *pSurface  = dynamic_cast< GLTextureSurface * >( (*itMS).first );
        IMediaSample     *pSample   = (*itMS).second.first;
        CAMEvent         *pEvent    = (*itMS).second.second;
        if ( 0 != pSurface && 0 != pSample )
        {
            GLUpdateMovieSample( pTexAlloc, pSurface, pSample );
            pEvent->Set();
        }
    }
    UpdateMovieAudio();
}

////////////////////////////////
//
// Generic version of UpdateMovieSample
//
// 
void UpdateMovieSample( Surface *pSurface, IMediaSample *pSample )
{
    SPointer<PixelTransfer> _writer = pSurface->CreateWriter();
    int width  = pSurface->GetWidth();
    int height = pSurface->GetHeight();
    BYTE *pData;
    pSample->GetPointer(&pData);
    int bytes = (CSurfaceRender::_prefered_bpp+1)/8;
    pData += width * bytes * (height-1);
    _writer->SetSpeedHint(true);
    _writer->SetSourcePointer( pData );
    _writer->SetSource( width, height, - width * bytes, CSurfaceRender::_prefered_bpp, 0, 0);
    _writer->Transform();
}

void UpdateMovieSamples()
{
    CAutoLock guard( &g_CritSec );
    for ( MovieSamples::iterator itMS = g_MovieSamples.begin(); itMS != g_MovieSamples.end(); ++itMS )
    {
        Surface          *pSurface  = (*itMS).first;
        IMediaSample     *pSample   = (*itMS).second.first;
        CAMEvent         *pEvent    = (*itMS).second.second;
        if ( 0 != pSurface && 0 != pSample )
        {
            UpdateMovieSample( pSurface, pSample );
            pEvent->Set();
        }
    }
    UpdateMovieAudio();
}



////////////////////////////////
//
//  CSurfaceRender
//

CSurfaceRender::CSurfaceRender()
    : _width(0)
    , _height(0)
    , _fps(0)
    , _eventSample(TRUE)
    , CBaseVideoRenderer(__uuidof(CLSID_SurfaceRenderer), NAME("Surface Renderer"), NULL, &_hr)
{
     _hr = S_OK;
}

CSurfaceRender::~CSurfaceRender()
{
    CAutoLock guard( &_mutex );
    UnsetMovieSample( _surface.GetTarget() );
}

HRESULT CSurfaceRender::CheckMediaType(const CMediaType *media_type)
{
    VIDEOINFO *vid_info;
    if ( *media_type->FormatType() == FORMAT_VideoInfo)
    {
        vid_info = (VIDEOINFO*)media_type->Format();
        if ( IsEqualGUID( *media_type->Type(), MEDIATYPE_Video))
        {
            switch ( _prefered_bpp )
            {
            case 32:
                if (IsEqualGUID( *media_type->Subtype(), MEDIASUBTYPE_RGB32))
                    { return S_OK; }
                break;
            case 24:
                if (IsEqualGUID( *media_type->Subtype(), MEDIASUBTYPE_RGB24))
                    { return S_OK; }
                break;
            case 16:
                if (IsEqualGUID( *media_type->Subtype(), MEDIASUBTYPE_RGB565))
                    { return S_OK; }
                break;
            case 15:
                if (IsEqualGUID( *media_type->Subtype(), MEDIASUBTYPE_RGB555))
                    { return S_OK; }
                break;
            }
        }
    }
    return E_INVALIDARG;
}

HRESULT CSurfaceRender::SetMediaType(const CMediaType *media_type)
{
    VIDEOINFO *vid_info;
    vid_info = (VIDEOINFO*)media_type->Format();
    _width = vid_info->bmiHeader.biWidth;
    _height= vid_info->bmiHeader.biHeight;
    double frame_time_100ns = vid_info->AvgTimePerFrame;
    _fps = 10000000/frame_time_100ns;

    return S_OK;
}

HRESULT CSurfaceRender::DoRenderSample( IMediaSample *pSample )
{ 
    CAutoLock guard( &_mutex );
    SetMovieSample( _surface.GetTarget(), pSample, &_eventSample );
    _eventSample.Wait( 50 );
    SetMovieSample( _surface.GetTarget(), 0, &_eventSample );
    return S_OK;
}

HRESULT CSurfaceRender::OnStartStreaming(void)
{
    return 0;
}
    
HRESULT CSurfaceRender::OnStopStreaming(void)
{
    CAutoLock guard( &_mutex );
    UnsetMovieSample( _surface.GetTarget() );
    return 0;
}
    
};//namespace Useless
