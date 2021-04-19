#include "UselessPch.h"
#include <algorithm>
#include "Useless/Graphic/detail/SurfaceStructs.h"

namespace Useless {

//////////////////////////////////////////////////////////////
//
//  Common Section
//
Surf::Format::Format()
{
    std::memset(this, 0, sizeof(*this) );
}

Surf::Properties::Properties()
{
    std::memset(this, 0, sizeof(*this) );
    color = true;
}


Surf::BlitFX::BlitFX()
{
    std::memset(this, 0, sizeof(*this) );
    alpha_factors[0] = 0;
    alpha_factors[1] = 255;
}

bool Surf::BlitFX::operator !() const
{
    return !src_color_key &&
           !dest_color_key &&
           !src_alpha &&
           !dest_alpha &&
           !flood_fill &&
           !rotation &&
           !stretch;
}

#ifdef WIN32
/////////////////////////////////////////////////////////////
//
// DirectDraw Section
//
Surf::Properties::Properties(const DDSURFACEDESC2 &desc)
{
    width  = desc.dwWidth;
    height = desc.dwHeight;
    num_surfaces = desc.dwBackBufferCount;
    has_color_key= false;
    color_key    = 0L;
    system_memory= ( desc.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY   )? true : false;
    video_memory = ( desc.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY    )? true : false;
    renderable   = ( desc.ddsCaps.dwCaps & DDSCAPS_3DDEVICE       )? true : false;
    zbuffer      = ( desc.ddsCaps.dwCaps & DDSCAPS_ZBUFFER        )? true : false;
    texture      = ( desc.ddsCaps.dwCaps & DDSCAPS_TEXTURE        )? true : false;
    alpha        = ( desc.ddsCaps.dwCaps & DDSCAPS_ALPHA          )? true : false;
    primary      = ( desc.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE )? true : false;
    pixelformat  = ImageFormat::Recognize
                    (
                        desc.ddpfPixelFormat.dwRBitMask, 
                        desc.ddpfPixelFormat.dwGBitMask,
                        desc.ddpfPixelFormat.dwBBitMask,
                        desc.ddpfPixelFormat.dwRGBBitCount
                    );
}

Surf::Properties::operator DDSURFACEDESC2() const
{
    DDSURFACEDESC2 desc;
    ZeroMemory(&desc, sizeof(desc) );

    desc.dwSize         = sizeof( DDSURFACEDESC2 );
    desc.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; 
    desc.dwWidth  = width;
    desc.dwHeight = height;
    if (num_surfaces>0)
    {
        desc.dwBackBufferCount = num_surfaces;
        desc.dwFlags |= DDSD_BACKBUFFERCOUNT;
    }
    if (system_memory) { desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY  ; }
    if (video_memory ) { desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY   ; }
    if (renderable   ) { desc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE      ; }
    if (primary      ) { desc.ddsCaps.dwCaps |= DDSCAPS_PRIMARYSURFACE; }
    if (texture      ) { desc.ddsCaps.dwCaps |= DDSCAPS_TEXTURE       ; }
    if (pixelformat!=ImageFormat::INVALID)
    {
        if (has_color_key) 
        { 
            desc.ddckCKSrcBlt.dwColorSpaceLowValue = ImageFormat::Encode( pixelformat, color_key );
            desc.ddckCKSrcBlt.dwColorSpaceHighValue = ImageFormat::Encode( pixelformat, color_key );
            desc.dwFlags |= DDSD_CKSRCBLT;
        }
        desc.dwFlags |=  DDSD_PIXELFORMAT;
        unsigned long mask[3];
        int           bpp;

        ImageFormat::GetColorMask( pixelformat, &mask[0], &mask[1], &mask[2] );
        bpp = 8 * ImageFormat::GetBpp ( pixelformat );

        ZeroMemory( &desc.ddpfPixelFormat, sizeof(DDPIXELFORMAT) );
        desc.ddpfPixelFormat.dwSize  = sizeof(DDPIXELFORMAT);
        desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
        desc.ddpfPixelFormat.dwRGBBitCount = bpp;
        desc.ddpfPixelFormat.dwRBitMask = mask[0];
        desc.ddpfPixelFormat.dwGBitMask = mask[1];
        desc.ddpfPixelFormat.dwBBitMask = mask[2];
    }

    return desc;
}
/////////////////////////////////////////////////////////////
#else


/////////////////////////////////////////////////////////////
//
// Place for other Surf::Properties implememtations
//
/////////////////////////////////////////////////////////////
#endif

#ifdef WIN32
/////////////////////////////////////////////////////////////
//
// DirectDraw Section
//
Surf::BlitFX::operator DDBLTFX() const
{
    DDBLTFX fx;
    ZeroMemory(&fx, sizeof(fx));
    fx.dwSize = sizeof(DDBLTFX);

    if ( rotation )
        fx.dwRotationAngle = angle;

    if (src_color_key)
    {
        fx.ddckSrcColorkey.dwColorSpaceLowValue = source.color_key;
        fx.ddckSrcColorkey.dwColorSpaceHighValue = source.color_key;
    }

    if (dest_color_key)
    {
        fx.ddckDestColorkey.dwColorSpaceLowValue = destination.color_key;
        fx.ddckDestColorkey.dwColorSpaceHighValue = destination.color_key;
    }

    if (flood_fill)
        fx.dwFillColor = flood_color;

    return fx;
}

int Surf::BlitFX::GetDDFlags() const
{
    int flags = 0;
    if (flood_fill)     flags |= DDBLT_COLORFILL;
    if (rotation)       flags |= DDBLT_ROTATIONANGLE;
    if (src_color_key)  flags |= DDBLT_KEYSRCOVERRIDE;
    if (dest_color_key) flags |= DDBLT_KEYDESTOVERRIDE;
    return flags;
}
//////////////////////////////////////////////////////////////
#else

/////////////////////////////////////////////////////////////
//
// Place for other Surf::BlitFX implememtations
//
/////////////////////////////////////////////////////////////
#endif


};//namespace Useless
