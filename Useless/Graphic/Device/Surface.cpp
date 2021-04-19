#include "UselessPch.h"

#include "Useless/Graphic/Device/Surface.h"

// Software pixel transfering
#include "Useless/Graphic/Transfer/GenericTransfer.h"

// Software surface painting
#include "Useless/Graphic/detail/SpecializedSurfacePainter.h"

namespace Useless {

Surface::Surface()  {}
Surface::~Surface() {}

void Surface::MultiBlit
(
    const Surface      &surf,
    const PointList    &p,
    const RectList     &r,
    const Surf::BlitFX &fx
)
{
    int ir, rsize = r.size();
    int ip, psize = p.size();
    for( ir=0, ip=0; ir!=rsize && ip!=psize; ++ir, ++ip )
    {
        if (!r[ir]) continue;

        Pos ps = p[ip]; Rect rc = r[ir];
        Blit( ps.x, ps.y , surf, rc, fx );
    }
}

//-----------------------------------------------------------------
struct SurfaceReadLock : public GenericTransfer
{
    SurfaceReadLock( LockableSurface *p, int channel_format )
        :_p(p), _channel_format(channel_format)
    {
        Surf::Ptr srf_ptr;
        _p->Lock( _channel_format, &srf_ptr );
        GenericTransfer::Initialize( srf_ptr.ptr, 0);
        GenericTransfer::SetSource( p->GetWidth(),
                                    p->GetHeight(),
                                    srf_ptr.pitch,
                                    srf_ptr.format );
    }

    virtual ~SurfaceReadLock()
    {
        _p->Unlock(_channel_format);
    }

    LockableSurface *_p;
    int              _channel_format;
};


struct SurfaceWriteLock : public GenericTransfer
{
    SurfaceWriteLock( LockableSurface *p, int channel_format )
        :_p(p), _channel_format(channel_format)
    {
        Surf::Ptr srf_ptr;
        _p->Lock( _channel_format, &srf_ptr );
        GenericTransfer::Initialize( 0, srf_ptr.ptr);
        GenericTransfer::SetDestination( p->GetWidth(),
                                         p->GetHeight(),
                                         srf_ptr.pitch,
                                         srf_ptr.format );
    }

    virtual ~SurfaceWriteLock()
    {
        _p->Unlock(_channel_format);
    }

    LockableSurface *_p;
    int              _channel_format;
};

SPointer<PixelTransfer> LockableSurface::CreateReader( int channel_format ) const
{
    return new SurfaceReadLock( const_cast<LockableSurface*>(this), channel_format);
}

SPointer<PixelTransfer> LockableSurface::CreateWriter( int channel_format )
{
    return new SurfaceWriteLock( this, channel_format);
}

SPointer<SurfacePainter> LockableSurface::CreatePainter()
{
    SurfacePainter *painter = CreateSurfacePainter( this );
    return painter;
}

};// namespace Useless
