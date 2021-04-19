#ifndef __INCLUDED_Painter_IMAGE_SKIN_H__
#define __INCLUDED_Painter_IMAGE_SKIN_H__

#include "Useless/GUI/Skins/Skin.h"

#include "Useless/Util/CtorWrapper.h"
#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Util/CopyingFactory.h"
#include "Useless/Util/VirtualValue.h"

namespace Useless {

/*! \ingroup Skins
 *  Uses Image as a skin.
 */
class USELESS_API ImageSkin : public Skin
{
public:
    typedef CtorWrapper< ImageBase, Image, const char * > RefImage;

    ImageSkin() {}
    ImageSkin( const RefImage &image );
    virtual ~ImageSkin() {}

    virtual  void  Paint    ( const Painter& painter, const Pos& = Pos() );
    virtual   int  GetWidth ()    const       { return _size.x;}
    virtual   int  GetHeight()    const       { return _size.y;}
    virtual   Pos  GetDirection() const       { return Pos(1,1); }
    virtual  void  SetSize  ( const Dim2i& d) { _size=d; }

    virtual bool         IsColorKeyed() const { return _sp_image->HasColorKey(); }
    virtual NormalPixel GetColorKey () const { return _sp_image->GetColorKey();  }
    virtual float       GetFrameRate() const { return _sp_image->GetFrameRate();}
    virtual bool        IsSolid()      const { return _sp_image->IsSolid();}

    ImageBase & GetImageBase() { return *_sp_image; }

private:
    Dim2i                   _size;
    VirtualValue<ImageBase> _sp_image;
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,ImageSkin);

#endif//__INCLUDED_Painter_IMAGE_SKIN_H__
