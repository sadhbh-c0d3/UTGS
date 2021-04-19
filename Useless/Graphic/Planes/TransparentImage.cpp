#include "UselessPch.h"

#include "Useless/Graphic/Planes/TransparentImage.h"
#include "Useless/Graphic/Planes/OGraphics.h"

#include "Useless/File/VIFS.h"
#include "Useless/Graphic/FileIO/ImageFactory.h"

#include "Useless/ErrorConfig.h"
#include "Useless/Error/ErrorMessage.h"
#define   TRIMAGE_ERROR(e)   APP_ERROR(e, "TransparentImage")
#define   TRIMAGE_WARNING(e) APP_WARNING(e, "TransparentImage")

namespace Useless {

TransparentImage::TransparentImage()
    :_alpha_loader(NULL), _alpha_loaded(false), _inverted(0), _alpha_0(0), _alpha_1(255), _angle(0.0)
{}

TransparentImage::TransparentImage( const char *color_plane, const char *alpha_plane )
    :Image(color_plane), _alpha_file(alpha_plane), _alpha_loaded(false),
     _inverted(0), _alpha_0(0), _alpha_1(255), _angle(0.0)
{
    SPointer<IFile> file = IFS::Instance().OpenFile(_alpha_file);
    int width, height, bpp;
    _alpha_loader = ImageFactory::Create(*file,&width, &height, &bpp);
    /*if (bpp!=8) 
    { 
        TRIMAGE_ERROR(Error("Alpha channel must be 8 bpp")); 
    }*/
    if (width!=_properties.width || height!=_properties.height )
    {
        TRIMAGE_ERROR(Error("Dimmentions of image( %i %i) and alpha(%i %i) channel must be the same",
        _properties.width, _properties.height, width, height) );
    }
    _properties.alpha = true;
}

TransparentImage::TransparentImage( const TransparentImage &image, const Rect &rect ):
            Image(image, rect),
    _alpha_file  (image._alpha_file),
    _alpha_loader(image._alpha_loader),
    _alpha_loaded(image._alpha_loaded),
    _inverted(image._inverted),
    _alpha_0(image._alpha_0), _alpha_1(image._alpha_1),
    _angle(image._angle)
{
}

TransparentImage::~TransparentImage()
{
}
 
void TransparentImage::BlitTo( int x, int y, OGraphics& output)
{
    Surface &dest = *output.GetSurface();
    Cooperate( dest );

    Surf::BlitFX fx;
    fx.invert_src_alpha = _inverted;
    fx.alpha_factors[0] = _alpha_0;
    fx.alpha_factors[1] = _alpha_1;
    fx.rotation = !!_angle;
    fx.angle = _angle;

    fx.width = GetDestWidth();
    fx.height = GetDestHeight();
    fx.stretch = (fx.width != GetWidth()) || (fx.height != GetHeight());

    dest.Blit( x, y, *GetSurface(), GetRect(), fx );
}

void TransparentImage::MultiBlitTo( OGraphics &output, const PointList &pnts, const RectList &rcs )
{
    Surface &dest = *output.GetSurface();
    Cooperate( dest );
    const Surface &source = *GetSurface();

    Surf::BlitFX fx;
    fx.invert_src_alpha = _inverted;
    fx.alpha_factors[0] = _alpha_0;
    fx.alpha_factors[1] = _alpha_1;
    fx.rotation = !!_angle;
    fx.angle = _angle;

    dest.MultiBlit( source, pnts, rcs, fx );
}
 
    
void TransparentImage::Load()
{
    // Since there were problems with Renderware locking surface just after unlocking
    // I have copied here a piece of code from Image::Load() method
    // Image::Load();

    // Load color (RGB) plane
    SPointer<IFile> color_file = IFS::Instance().OpenFile(_name);
    SPointer<PixelTransfer> trans_color = _sh_surface->CreateWriter( Surface::COLOR );
    _loader->Load( *color_file, *trans_color );
    trans_color->Transform();

    // Load alpha plane or channel (PixelTransfer must handle both)
    SPointer<IFile> alpha_file = IFS::Instance().OpenFile(_alpha_file);
    SPointer<PixelTransfer> trans_alpha = _sh_surface->CreateWriter( Surface::ALPHA );
    _alpha_loader->Load( *alpha_file, *trans_alpha );
    trans_alpha->Transform();

    C_message( TransparentImage, 0, ("Loaded Pixels of TransparentImage:", _name.c_str() ));
}

};//namespace Useless
