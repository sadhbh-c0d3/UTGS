#ifndef __INCLUDED_USELESS_PNG_LOADER___
#define __INCLUDED_USELESS_PNG_LOADER___

/*
**  $Id: PNGLoader.h,v 1.5 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      PNGLoader
**
**  PURPOSE
**      Load PNG images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Graphic/FileIO/ImageLoader.h"
#include "Useless/Graphic/detail/ImageLoaderRegistrator.h"

namespace Useless {

/*! \ingroup ImgIO
 *  Loads PNG files.
 */
class USELESS_API PNGLoader : public ImageLoader
{
public:
    virtual void Load ( IFile &ifile, OGfxFilter &filter );
    virtual void Load( IFile &ifile, PixelTransfer &transfer );
    virtual ~PNGLoader() {}

    static bool Recognize( IFile &ifile, int *w,int *h,int *bpp );
    static ImageLoader * Create();
};


static ImageLoaderRegistrator _PNG_registrator( ImageTypes::PNG, &PNGLoader::Create, &PNGLoader::Recognize );

}; // namespace Useless ends

#endif __INCLUDED_USELESS_PNG_LOADER___
