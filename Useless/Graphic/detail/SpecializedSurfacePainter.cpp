#include "UselessPch.h"

#pragma warning (disable:4244) //-- conversion from 'float' to 'int', possible loss of data
#include "Useless/Graphic/Device/Surface.h"
#include "SpecializedSurfacePainter.h"
#include "Useless/Graphic/Pixel/PixelTypes.h"

namespace Useless {

template< class PixelType >
void __CreateSpecializedSurfacePainter
    ( LockableSurface *p_surface, void *ptr, int pitch, PixelType&, SurfacePainter *&painter )
{
    painter = new SpecializedSurfacePainter< PixelType >
        ( p_surface, ptr, pitch, p_surface->GetClipper().GetOptimized() );
}

SafePtr< SurfacePainter > CreateSurfacePainter( LockableSurface *p_surface )
{
    if (!p_surface)
    {
        assert(0);
        APP_WARNING( Error("No Surface to paint on !!!"), "Creator< SurfacePainter >");
    }

    Surf::Ptr surf_ptr;

    p_surface->Lock( Surface::COLOR, &surf_ptr );
    if (!surf_ptr.ptr) { return 0; }

    SurfacePainter *painter = 0;

    __PIXEL_SWITCH( surf_ptr.format,
        __CreateSpecializedSurfacePainter( p_surface, surf_ptr.ptr, surf_ptr.pitch, pix, painter), pix );

    if (!painter)
    {
        APP_WARNING( Error("Format %i is not supported", surf_ptr.format), "Creator< SurfacePainter >");
    }

    return painter;
}

};//namespace Useless
