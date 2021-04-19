#ifndef __INCLUDED_USELESS_I_GFX_READER__
#define __INCLUDED_USELESS_I_GFX_READER__

/*
**  $Id: IGfxReader.h,v 1.4 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      IGfxReader
**
**  PURPOSE
**      Input graphics from buffer
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Graphic/Transfer/IGfxFilter.h"
#include "Useless/Util/Storage/MemBlock.h"

namespace Useless {
/*! \ingroup ImgIO
 *  Gives IGraphics interface to memory block
 */

class USELESS_API IGfxReader : public IGfxFilter
{
public:
    IGfxReader( const Types::UBYTE *d, const Types::UBYTE *p, int w, int h, int b, int s)
        : _data(d), _palette(p), _width(w), _height(h), _bytes_per_pixel(b), _pal_size(s) {}
    virtual void GetSize( int *w, int *h, int *d );
    virtual void ReadPalette( const Types::UBYTE **rgb, int *pal_size );
    virtual void Read( Types::UBYTE *s, int x, int y, int w, int h, int pitch );

private:
    const Types::UBYTE *_data;
    const Types::UBYTE *_palette;
    int _width;
    int _height;
    int _bytes_per_pixel;
    int _pal_size;
};


}; // namespace Useless

#endif __INCLUDED_USELESS_I_GFX_READER__
