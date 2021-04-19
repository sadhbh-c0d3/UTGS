#ifndef __INCLUDED_USELESS_IMAGE_LOADER_REGISTRATOR_H__
#define __INCLUDED_USELESS_IMAGE_LOADER_REGISTRATOR_H__
#include "Useless/Graphic/FileIO/ImageFactory.h"
#include "Useless/Graphic/detail/ImageTypes.h"

namespace Useless {

class ImageLoaderRegistrator
{
    public:
        ImageLoaderRegistrator( int id, ImageLoader *(*creator)(), bool (*recognizer)( IFile &,int*,int*,int*) )
        { 
            ImageFactory::Register( id, creator, recognizer );
        }
};

}; // namespace Useless
#endif __INCLUDED_USELESS_IMAGE_LOADER_REGISTRATOR_H__
