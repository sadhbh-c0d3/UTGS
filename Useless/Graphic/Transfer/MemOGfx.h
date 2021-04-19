#ifndef __INCLUDED_USELESS_MEM_OGFX_H__
#define __INCLUDED_USELESS_MEM_OGFX_H__

/*
**  $Id: MemOGfx.h,v 1.3 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      MemOGfx
**
**  PURPOSE
**      Output graphics to memory
**
**  NOTE
**      This class is unsafe, so it is your responsibility to pass enough memory.
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Graphic/Transfer/OGfxFilter.h"

namespace Useless {
/*! \ingroup ImgIO
 *  Memory image transfer.
 */

class USELESS_API MemOGfx : public OGfxFilter
{
public:
    MemOGfx( Types::UBYTE *output_memory, int pitch ): _output_memory(output_memory), _pitch(pitch), _palette_output(0) {}
    virtual ~MemOGfx() {}

    void SetPaletteMemory( Types::UBYTE *palette_output) { _palette_output=palette_output; }

    virtual void SetSize( int w, int h, int d );
    virtual void WritePalette( const Types::UBYTE *rgb, int pal_size=256 );
    virtual void Write( const Types::UBYTE *s, int x, int y, int w, int h, int pitch );

private:
    Types::UBYTE *_output_memory;
    Types::UBYTE *_palette_output;
             int  _pitch;
             int  _w;
             int  _h;
             int  _pxs;
};


} // namespace Useless

#endif __INCLUDED_USELESS_MEM_OGFX_H__
