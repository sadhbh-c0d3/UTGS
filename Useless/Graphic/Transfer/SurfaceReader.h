#ifndef __INCLUDED__USELESS_SURFACE_READER_H__
#define __INCLUDED__USELESS_SURFACE_READER_H__

#include "Useless/Graphic/Transfer/IGfxFilter.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

class Surface;

class USELESS_API SurfaceReader : public IGfxFilter
{
public:
    SurfaceReader( const Surface &surface );
    virtual ~SurfaceReader();

    virtual void GetSize    ( int *width, int *height, int *depth );
    virtual void Read       ( Types::UBYTE *s, int x, int y, int w, int h, int pitch );

    // result is null palette
    virtual void ReadPalette( const Types::UBYTE **, int *);

    void SetFormat( int format );

    const Surface& GetSurface() const { return _source; }
private:
    const Surface &_source;
    SPointer< PixelTransfer > _transfer;
    int _w;
    int _h;
    int _dest_pitch;
    int _bpp;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_SURFACE_READER_H__
