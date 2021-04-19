#ifndef __INCLUDED_IMAGE_H__
#define __INCLUDED_IMAGE_H__

/*
**  $Id: Image.h,v 1.17 2003/01/22 18:58:13 koolas Exp $
**
**  NAME
**      Image
**
**  PURPOSE
**      Image read-only graphics for blits
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "Useless/Graphic/Planes/ImageBase.h"
#include "Useless/Graphic/FileIO/ImageLoader.h"
#include <string>

namespace Useless {
/*! \ingroup GfxPlanes
 *  Loads any image format from disk, and places it onto Surface
 */
//REQUIRED
    class Screen;

class USELESS_API Image : public ImageBase
{
public:
    Image();
    Image( const char *fname);
    Image( const char *fname, unsigned long color_key );
    Image( const Image& image, const Useless::Rect &rect );
    Image( const Image& image, const Useless::Rect &rect, unsigned long color_key );
    Image &operator=( const Image & );
    virtual ~Image();
    virtual SPointer<IGraphics> Copy()                 const { return new Image(*this); }
    virtual SPointer<IGraphics> Copy(const Rect &rect) const { return new Image(*this, rect); }

    void Prefetch( const Screen& screen );

protected:
    virtual void Load();

    std::string                         _name;
    Useless::SPointer< Useless::ImageLoader > _loader;
    bool                                _loaded;
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,IGraphics,Image);
REGISTER_COPIER(Useless,ImageBase,Image);

#endif //__INCLUDED_IMAGE_H__
