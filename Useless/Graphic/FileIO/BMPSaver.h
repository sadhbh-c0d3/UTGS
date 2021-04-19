#ifndef __INCLUDED_USELESS_BMP_SAVER___
#define __INCLUDED_USELESS_BMP_SAVER___

/*
**  $Id: BMPSaver.h,v 1.3 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      BMPSaver
**
**  PURPOSE
**      Save BMP images
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/File/OFile.h"
#include "Useless/Graphic/Transfer/IGfxFilter.h"
#include "Useless/Graphic/FileIO/ImageSaver.h"
#include "Useless/Graphic/detail/ImageSaverRegistrator.h"

namespace Useless {
/*! \ingroup ImgIO
 *  Saves BMP images
 */
class USELESS_API BMPSaver : public ImageSaver
{
public:
    virtual void Save( OFile &OFile, IGfxFilter &filter );

    static ImageSaver * Create();
};

// Register BMP loader
static ImageSaverRegistrator _BMP_saver_registrator( ImageTypes::BMP, &BMPSaver::Create );

}; // namespace Useless ends

#endif __INCLUDED_USELESS_BMP_SAVER___
