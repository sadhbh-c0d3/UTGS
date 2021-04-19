#ifndef __INCLUDED_SURFACE_WRITER_H__
#define __INCLUDED_SURFACE_WRITER_H__

/*
**  $Id: SurfaceWriter.h,v 1.7 2003/01/20 21:53:40 koolas Exp $
**
**  NAME
**      SurfaceWriter 
**
**  PURPOSE
**      Write data directly onto Surface
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Transfer/OGfxFilter.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

class Surface;

/*! \ingroup ImgIO
 *  \ingroup gGfxAPI
 *  Writes incoming Image data onto Surface !!!! NOW ITS ONLY GENERIC-TRANSFER WRAPPER !!!!
 */
class USELESS_API SurfaceWriter : public Useless::OGfxFilter
{
public:    
    SurfaceWriter(Surface &dest);
    ~SurfaceWriter();
    virtual void Write(const Types::UBYTE *src, int x, int y, int w, int h, int pitch);
    virtual void WritePalette(const Types::UBYTE *ptr, int pal_size=256);
    virtual void SetSize(int w, int h, int bpp);

    void Flush();
    
    Surface& GetSurface() const { return _dest; }
private:
    Surface &_dest;
    SPointer< PixelTransfer > _transfer;
    int _w;
    int _h;
    int _source_pitch;
    int _bpp;
};

};// namespace Useless
#endif //__INCLUDED_DDRAW_SURFACE_WRITER_H__
