#ifndef __INCLUDED_USELESS_TGA_LOADER___
#define __INCLUDED_USELESS_TGA_LOADER___

/*
**  $Id: TGALoader.h,v 1.5 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      TGALoader
**
**  PURPOSE
**      Load TGA images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Graphic/FileIO/ImageLoader.h"
#include "Useless/Graphic/detail/ImageLoaderRegistrator.h"

namespace Useless {

/*! \ingroup ImgIO
 *  Loads TGA files.
 */
class USELESS_API TGALoader : public ImageLoader
{
public:
    virtual void Load ( IFile &ifile, OGfxFilter &filter );
    virtual void Load( IFile &ifile, PixelTransfer &transfer );
    virtual ~TGALoader() {}

    static bool Recognize( IFile &ifile, int *w,int *h,int *bpp );
    static ImageLoader * Create();
};


static ImageLoaderRegistrator _TGA_registrator( ImageTypes::TGA, &TGALoader::Create, &TGALoader::Recognize );

}; // namespace Useless ends

#endif __INCLUDED_USELESS_TGA_LOADER___
