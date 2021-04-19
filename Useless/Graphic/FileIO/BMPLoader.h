#ifndef __INCLUDED_USELESS_BMP_LOADER___
#define __INCLUDED_USELESS_BMP_LOADER___

/*
**  $Id: BMPLoader.h,v 1.5 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      BMPLoader
**
**  PURPOSE
**      Load BMP images
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
class USELESS_API BMPLoader : public ImageLoader
{
public:
    virtual void Load ( IFile &ifile, OGfxFilter &filter );
    virtual void Load( IFile &ifile, PixelTransfer &transfer );
    virtual ~BMPLoader() {}

    static bool Recognize( IFile &ifile, int *w,int *h,int *bpp );
    static ImageLoader * Create();
};

static ImageLoaderRegistrator _BMP_registrator( ImageTypes::BMP, &BMPLoader::Create, &BMPLoader::Recognize );

}; // namespace Useless ends

#endif __INCLUDED_USELESS_BMP_LOADER___
