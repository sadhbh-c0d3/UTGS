#ifndef __INCLUDED_USELESS_IMAGE_SAVER___
#define __INCLUDED_USELESS_IMAGE_SAVER___

/*
**  $Id: ImageSaver.h,v 1.4 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      ImageSaver
**
**  PURPOSE
**      Abstract for saving images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/File/OFile.h"
#include "Useless/Graphic/Transfer/IGfxFilter.h"

namespace Useless {

/*! \ingroup ImgIO
 *  Abstract for saving images of many different types
 */
class USELESS_API ImageSaver
{
public:
    virtual ~ImageSaver() {}
    virtual void Save( OFile &OFile, IGfxFilter &filter ) {}
};

}; // namespace Useless ends

#endif __INCLUDED_USELESS_IMAGE_SAVER___
