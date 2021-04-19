#ifndef __INCLUDED_USELESS_O_GFX_BUFFER__
#define __INCLUDED_USELESS_O_GFX_BUFFER__

/*
**  $Id: OGfxBuffer.h,v 1.6 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      OGfxBuffer
**
**  PURPOSE
**      Output graphics buffer
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Graphic/Transfer/OGfxFilter.h"
#include "Useless/Graphic/Transfer/IGfxReader.h"
#include "Useless/Util/Storage/MemBlock.h"

namespace Useless {
/*! \ingroup ImgIO
 *  Acts like a buffer for Image transfer.
 */

class USELESS_API OGfxBuffer : public OGfxFilter
{
public:
    virtual void SetSize( int w, int h, int d );
    virtual void WritePalette( const Types::UBYTE *rgb, int pal_size=256 );
    virtual void Write( const Types::UBYTE *s, int x, int y, int w, int h, int pitch );

    IGfxReader CreateReader() const 
    {
        return IGfxReader(_data.GetConstPtrUB(), _palette.GetConstPtrUB(), _width, _height, _bytes_per_pixel, _palette.GetSize() ); 
    }

    const Types::UBYTE * GetPtr(int *pitch=0) const 
    {
        if(pitch) { *pitch = _width*_bytes_per_pixel; }
        return _data.GetConstPtrUB(); 
    }


private:
    MemBlock _data;
    MemBlock _palette;
    int      _width;
    int      _height;
    int      _bytes_per_pixel;
};


}; // namespace Useless

#endif __INCLUDED_USELESS_O_GFX_BUFFER__
