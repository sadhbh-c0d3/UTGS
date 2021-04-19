#ifndef __INCLUDED__USELESS__MEMORY_SURFACE_H__
#define __INCLUDED__USELESS__MEMORY_SURFACE_H__

#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/detail/SurfaceStructs.h"
#include "Useless/Graphic/RectList.h"

namespace Useless {

/*! Simplified memory surface
 */
class USELESS_API MemorySurface //simplified
{
public:
    MemorySurface();
    MemorySurface( const Surf::Properties &properties );

    void Lock( void **ptr, int *pitch, int *bpp );
    void Unlock();

    void LockAlpha( void **ptr, int *pitch );
    void UnlockAlpha();

    /* x,y : destination positon
     * dest: destination memory ptr
     * s_rc: source crop rectangle
    */
    void Blit( int x, int y, const Surf::Ptr &dest, Rect s_rc ) const;
    void ColorFill( int x, int y, const Surf::Ptr &dest, Rect s_rc,
                    NormalPixel color ) const;

    /* d_rc : destination area
     * dest : destination memory ptr
     * s_rc : source crop rectangle
     * angle: rotation angle (CCW)
     * pivot: rotation pivot
    */
    void RotoBlit( Rect d_rc, const Surf::Ptr &dest, Rect s_rc,
                   double angle, Pos pivot) const;
    void RotoColorFill( Rect d_rc, const Surf::Ptr &dest, Rect s_rc,
                        NormalPixel color, double angle, Pos pivot ) const;

    void InvertAlpha( bool t=true ) { _invert_alpha=t; }

    void SetFactors( int alpha_0=0, int alpha_1=255 );

protected:
    int  _width;
    int  _height;
    int  _color_bits; //< phisycal number of bits per pixel (not ImageFormat)
    bool _invert_alpha;
    int  _alpha_0;
    int  _alpha_1;

    MemBlock _color_plane;
    MemBlock _alpha_plane;
};
//----------------------------------------------------------


inline void MemorySurface::Lock( void **ptr, int *pitch, int *bpp )
{
    *ptr = _color_plane.GetPtr();
    *pitch = _width * _color_bits/8;
    *bpp = _color_bits;
}

inline void MemorySurface::Unlock()
{
}

inline void MemorySurface::LockAlpha( void **ptr, int *pitch )
{
    *ptr = _alpha_plane.GetPtr();
    *pitch = _width;
}

inline void MemorySurface::UnlockAlpha()
{
}


}; //Useless
#endif//__INCLUDED__USELESS__MEMORY_SURFACE_H__
