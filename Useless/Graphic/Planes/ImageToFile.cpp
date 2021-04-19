#include "UselessPch.h"

#include "Useless/Graphic/Planes/ImageToFile.h"
#include "Useless/Graphic/FileIO/ImageFactory.h"
#include "Useless/Graphic/Transfer/SurfaceReader.h"
#include "Useless/File/StdOFile.h"
#include "Useless/Graphic/FileIO/TGASaver.h"

namespace Useless {
void ImageToFile::Store( const Surface     &surface,
                         const std::string &name, 
                               int          type_tag )
{
    SurfaceReader reader(surface);
    reader.SetFormat( ImageFormat::B8G8R8 );

    StdOFile file(name);
    ImageFactory::Create(type_tag)->Save( file, reader);
}

};//namespace Useless
