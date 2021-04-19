#include "UselessPch.h"

#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Graphic/Device/Screen.h"

#include "Useless/Graphic/Transfer/SurfaceWriter.h"
#include "Useless/Graphic/FileIO/ImageFactory.h"

#include "Useless/ErrorConfig.h"
#include "Useless/Error/ErrorMessage.h"
#define   IMAGE_WARNING(e) APP_WARNING(e, "Image")

#include "Useless/File/VIFS.h"

using namespace Useless;

Image::Image(): _loader(NULL), _loaded(false)
{}

Image::Image( const char *fname ): _name(fname), _loaded(false)
{
    SPointer<IFile> file = IFS::Instance().OpenFile(_name);
    _loader = ImageFactory::Create(*file,&_properties.width, &_properties.height, (int*)&_properties.pixelformat);
    _rectangle = Useless::Rect(0, 0, _properties.width, _properties.height);
    _width = _rectangle.GetW(); _height = _rectangle.GetH();
}

Image::Image( const char *fname, unsigned long c): _name(fname), _loaded(false)
{
    SPointer<IFile> file = IFS::Instance().OpenFile(_name);
    _loader = ImageFactory::Create(*file,&_properties.width, &_properties.height, (int*)&_properties.pixelformat);
    _rectangle = Useless::Rect(0, 0, _properties.width, _properties.height);
    _properties.pixelformat = ImageFormat::INVALID;
    _properties.has_color_key = true;
    _properties.color_key = c;
    _width = _rectangle.GetW(); _height = _rectangle.GetH();
}

Image::Image(const Image& image, const Useless::Rect &rect)
        :ImageBase(image, rect), _name(image._name), _loader(image._loader), _loaded(image._loaded)
{}

Image::Image(const Image& image, const Useless::Rect &rect, unsigned long c)
        :ImageBase(image, rect), _name(image._name), _loader(image._loader), _loaded(image._loaded)
{
    _properties.has_color_key = true;
    _properties.color_key = c;
}

Image &Image::operator=(const Image &image)
{
    ImageBase::operator=(image);
    _name  =image._name;
    _loader=image._loader;
    _loaded=image._loaded;
    return *this;
}

Image::~Image()
{}

void Image::Prefetch( const Screen& screen )
{
    Cooperate( *screen.GetSurface() );
}
    
void Image::Load()
{
    SPointer<IFile> file = IFS::Instance().OpenFile(_name);

    SPointer<PixelTransfer> spt = _sh_surface->CreateWriter( Surface::COLOR );
    _loader->Load( *file, *spt );
    spt->Transform();

    _loaded = true;
    C_message( Image, 0, ("Loaded Pixels of Image:", _name.c_str() ));
}
