#ifndef __INCLUDED_BUFFERED_IMAGE_H__
#define __INCLUDED_BUFFERED_IMAGE_H__

/*
**  $Id: BufferedImage.h,v 1.8 2003/01/22 18:58:13 koolas Exp $
**
**  NAME
**      BufferedImage
**
**  PURPOSE
**      BufferedImage read-only graphics for blits
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Graphic/Transfer/OGfxBuffer.h"

namespace Useless {
/*! \ingroup GfxPlanes
 *  Loads any image format from disk, and places it onto Surface
 */

class USELESS_API BufferedImage : public Image
{
public:
    BufferedImage();
    BufferedImage( const char *fname);
    BufferedImage( const char *fname, unsigned long color_key );
    BufferedImage( const BufferedImage& image, const Useless::Rect &rect );
    BufferedImage( const BufferedImage& image, const Useless::Rect &rect, unsigned long color_key );
    BufferedImage &operator=( const BufferedImage & );

    virtual ~BufferedImage();
    virtual SPointer<IGraphics> Copy()                 const { return new BufferedImage(*this); }
    virtual SPointer<IGraphics> Copy(const Rect &rect) const { return new BufferedImage(*this, rect); }

protected:
    virtual void Load();

    SPointer<OGfxBuffer>                _image_buffer;
    int                                 _format;
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,IGraphics,BufferedImage);
REGISTER_COPIER(Useless,ImageBase,BufferedImage);
REGISTER_COPIER(Useless,Image,BufferedImage);

#endif //__INCLUDED_BUFFERED_IMAGE_H__
