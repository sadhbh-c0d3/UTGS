#ifndef __INCLUDED_USELESS_IMAGE_LOADER___
#define __INCLUDED_USELESS_IMAGE_LOADER___

/*
**  $Id: ImageLoader.h,v 1.5 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      ImageLoader
**
**  PURPOSE
**      Abstract for loading images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/File/IFile.h"
#include "Useless/Graphic/Transfer/OGfxFilter.h"
#include "Useless/Graphic/Transfer/PixelTransfer.h"

namespace Useless {

/*! \ingroup ImgIO
 *  Abstract for loading images of many different types
 */
class USELESS_API ImageLoader
{
public:
    virtual void Load( IFile &ifile, OGfxFilter &filter ) = 0;
    virtual void Load( IFile &ifile, PixelTransfer &transfer ) {}
    virtual ~ImageLoader() {}
};

}; // namespace Useless ends

#endif __INCLUDED_USELESS_IMAGE_LOADER___
