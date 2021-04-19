#ifndef __INCLUDED_USELESS_IMAGE_SAVER_REGISTRATOR_H__
#define __INCLUDED_USELESS_IMAGE_SAVER_REGISTRATOR_H__
#include "Useless/Graphic/FileIO/ImageFactory.h"
#include "Useless/Graphic/detail/ImageTypes.h"

namespace Useless {

class ImageSaverRegistrator
{
    public:
        ImageSaverRegistrator( int id, ImageSaver *(*creator)() )
        { 
            ImageFactory::RegisterSaver( id, creator );
        }
};

}; // namespace Useless
#endif __INCLUDED_USELESS_IMAGE_SAVER_REGISTRATOR_H__
