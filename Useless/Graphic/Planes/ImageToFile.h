#ifndef __INCLUDED__USELESS_IMAGE_TO_FILE_H__
#define __INCLUDED__USELESS_IMAGE_TO_FILE_H__

#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/detail/ImageTypes.h"
#include <string>

namespace Useless {

class USELESS_API ImageToFile
{
public:
    static void Store( const Surface     &surface, 
                       const std::string &name, 
                             int          type_tag=ImageTypes::TGA );
};


};//namespace Useless
#endif//__INCLUDED__USELESS_IMAGE_TO_FILE_H__
