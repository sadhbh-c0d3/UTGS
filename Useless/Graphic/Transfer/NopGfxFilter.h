#ifndef __INCLUDED_USELESS_NOP_GFX_FILTER__
#define __INCLUDED_USELESS_NOP_GFX_FILTER__

/*
**  $Id: NopGfxFilter.h,v 1.3 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      NopGfxFilter
**
**  PURPOSE
**      NOP IO graphics filter
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Graphic/Transfer/OGfxFilter.h"
#include "Useless/Graphic/Transfer/IGfxFilter.h"

namespace Useless {

class USELESS_API NopGfxFilter : public OGfxFilter, public IGfxFilter
{
public:
    virtual void SetSize( int w, int h, int d ) {}
    virtual void WritePalette( const Types::UBYTE *rgb, int pal_size=256 ) {}
    virtual void Write( const Types::UBYTE *s, int x, int y, int w, int h, int pitch ) {}

    virtual void GetSize( int *w, int *h, int *d) {}
    virtual void ReadPalette(const Types::UBYTE **rgb, int *pal_size) {}
    virtual void Read( Types::UBYTE *s, int x, int y, int w, int h, int pitch ) {}
};

}; // namespace Useless

#endif __INCLUDED_USELESS_NOP_GFX_FILTER__
