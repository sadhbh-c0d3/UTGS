#ifndef __INCLUDED_USELESS_TGA_SAVER___
#define __INCLUDED_USELESS_TGA_SAVER___

/*
**  $Id: TGASaver.h,v 1.3 2002/09/12 14:27:37 koolas Exp $
**
**  NAME
**      TGASaver
**
**  PURPOSE
**      Save TGA images
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
 *  Saves TGA images
 */

class USELESS_API TGASaver : public ImageSaver
{
public:
    virtual void Save( OFile &OFile, IGfxFilter &filter );

    static ImageSaver * Create();
};

// Register TGA loader
static ImageSaverRegistrator _TGA_saver_registrator( ImageTypes::TGA, &TGASaver::Create );

}; // namespace Useless ends

#endif __INCLUDED_USELESS_TGA_SAVER___
