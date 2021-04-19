#include "UselessPch.h"

#include "Useless/Graphic/Planes/BufferedImage.h"
#include "Useless/Graphic/Device/Screen.h"

#include "Useless/Graphic/Transfer/SurfaceWriter.h"
#include "Useless/Graphic/FileIO/ImageFactory.h"

#include "Useless/Graphic/Transfer/SurfaceReader.h"

#include "Useless/ErrorConfig.h"
#define   IMAGE_WARNING(e) APP_WARNING(e, "BufferedImage")


using namespace Useless;

BufferedImage::BufferedImage()
{}

BufferedImage::BufferedImage( const char *fname ): Image(fname)
{
}

BufferedImage::BufferedImage( const char *fname, unsigned long c): Image(fname,c)
{
}

BufferedImage::BufferedImage(const BufferedImage& image, const Useless::Rect &rect)
        :Image(image,rect)
{
}

BufferedImage::BufferedImage(const BufferedImage& image, const Useless::Rect &rect, unsigned long c)
        :Image(image,rect,c)
{
}

BufferedImage &BufferedImage::operator=(const BufferedImage &image)
{
    Image::operator=(image); return *this;
}

BufferedImage::~BufferedImage()
{}

void BufferedImage::Load()
{
    Surf::Properties props;
    _sh_surface->GetProperties(&props);

    if (!_image_buffer || props.pixelformat!=_format )
    {
        Image::Load();

        int w,h,d;

        _image_buffer = new OGfxBuffer;
        {
            SurfaceReader reader(*_sh_surface);
            reader.GetSize(&w,&h,&d);
            _image_buffer->SetSize(w,h,d);
        }
        Surf::Ptr srf_ptr;
        static_cast<LockableSurface&>(*_sh_surface).Lock( Surface::COLOR, &srf_ptr);
        _image_buffer->Write( (Types::UBYTE*)srf_ptr.ptr, 0,0, w,h, srf_ptr.pitch);
        static_cast<LockableSurface&>(*_sh_surface).Unlock();
    }
    else
    {
        SurfaceWriter writer(*_sh_surface);
        IGfxReader reader = _image_buffer->CreateReader();
        int w,h,d,p;
        reader.GetSize(&w,&h,&d);
        writer.SetSize(w,h,d);
        const unsigned char * pdata = _image_buffer->GetPtr(&p);
        writer.Write( pdata, 0,0, w,h, p);
    }
    _loaded = true;
}
