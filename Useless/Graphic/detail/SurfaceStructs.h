#ifndef __INCLUDED_USELESS_SURFACE_STRUCS__H__
#define __INCLUDED_USELESS_SURFACE_STRUCS__H__

#ifdef WIN32
#   include <ddraw.h>
#endif

#include "Useless/Graphic/detail/ImageDesc.h"

namespace Useless {

class Surface;

struct Surf {

/*! Helpful surface pointer
 */
struct Ptr
{
    Ptr(): ptr(0), pitch(0), format(0) {}
    Ptr( void *ptr, int pitch, int format ): ptr(ptr), pitch(pitch), format(format) {}

    void *ptr;
    int   pitch;
    int   format;
};


/*! Simple description of image: WIDTH x HEIGHT x DEPTH
 */
class USELESS_API Format
{
public:
    Format();

    int                 width;          
    int                 height;
    ImageFormat::Format pixelformat;
};

/*! \class Properties
 *  \ingroup gGfxAPI
 *  Class only intended to use with any kind of Surface.
 *
 *  Used to set parameters of newly created surface,
 *  or to obtain current surface parameters.
 */
class USELESS_API Properties : public Format
{
public:
    Properties();

    unsigned color:1,         // color plane
             alpha:1,         // alpha plane
             zbuffer:1,       // Z plane
             texture:1,       // to use as texture for objects
             renderable:1,    // to render images and object on it
             blit_source:1,   // to be used as blit source
             prefer_copy:1,   // create new surface and copy pixels, instead of using this as blit source
             use_textures:1,  // scatter to several textures
             video_memory:1,  // use video memory
             system_memory:1, // use system memory
             primary:1,       // is it primary surface (screen or window)
             has_color_key:1; // set color key

    unsigned color_key;     // color key value
    int      num_surfaces;  // length of backbuffer chain


#ifdef WIN32
    // This will enable casts between DDSURFACEDESC2 and Properties
    Properties( const DDSURFACEDESC2 &);
    operator DDSURFACEDESC2() const;
#endif
};

/*! \class BlitFX
 *  \ingroup gGfxAPI
 *  Class only intended to use with DDSurface
 *
 *  Used to setup blit technique
 */
class USELESS_API BlitFX
{
public:
    BlitFX();

    bool operator !()     const;

    unsigned src_color_key:1,
             dest_color_key:1,
             src_alpha:1,
             dest_alpha:1,
             invert_src_alpha:1,
             invert_dest_alpha:1,
             flood_fill:1,
             rotation:1,
             stretch:1;

    int     angle;
    int     width;
    int     height;
    int     flood_color;
    int     alpha_factors[2];

    union
    {
       unsigned    color_key;
       Surface    *alpha;
    } source, destination;


#ifdef WIN32
    // This will enable casts between DDBLTFX and BlitFX
    operator    DDBLTFX()    const;
          int   GetDDFlags() const;
#endif

};

};//struct Surf

};//namespace Useless;

#endif//__INCLUDED_USELESS_SURFACE_STRUCS__H__
