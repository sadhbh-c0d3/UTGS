#include "UselessPch.h"

#ifndef WIN32
#   include <typeinfo>
#endif

#include "Useless/Graphic/Planes/ImageBase.h"

#include "Useless/ErrorConfig.h"
#define   IMAGE_WARNING(e) APP_WARNING(e, "ImageBase")
#define   IMAGE_ERROR(e) APP_ERROR(e, "ImageBase")

namespace Useless {

ImageBase::ImageBase(): _width(0), _height(0)
{}

ImageBase::ImageBase( const ImageBase &image, const Rect &rect )
       :_sh_surface(image._sh_surface), _properties(image._properties), _rectangle(rect)
{
    const Rect &r = image.GetRect(); _rectangle.SetPos( _rectangle.GetPos() + r.GetPos() );
    if( !(_rectangle &= r) )
    {
        IMAGE_WARNING( Useless::Error("No area: R=(%i,%i,%i,%i) R'=(%i,%i,%i,%i)", 
              r.GetX(), r.GetY(), r.GetW(), r.GetH(),
              rect.GetX(), rect.GetY(), rect.GetW(), rect.GetH() ) );
    }

    _width = _rectangle.GetW(); _height = _rectangle.GetH();
}

ImageBase& ImageBase::operator=( const ImageBase &image )
{
    _sh_surface=image._sh_surface; 
    _properties=image._properties;
    _rectangle =image._rectangle;
    _width = _rectangle.GetW(); _height = _rectangle.GetH();
    return *this;
}

ImageBase::~ImageBase()
{}

void ImageBase::Cooperate( const Surface& surface )
{
    Surf::Properties actual;
    if ( !!_sh_surface ) { _sh_surface->GetProperties(&actual); }

    //  Check if surface is allocated, valid and conforms to descriptor

    // debug variables
    bool height_is_wrong = false;
    bool width_is_wrong = false;
    bool not_valid_blit_source = false;
    bool no_surface = false;
    
    while( true )
    {
        if ( !!_sh_surface )
        {
            if ( surface.IsValidBlitSource( *_sh_surface ) )
            {
                if ( actual.width == _properties.width )
                {
                    if ( actual.height == _properties.height )
                    {
                        //  Set color-key if this is what descriptors says
                        if ( _properties.has_color_key )
                        {
                            if ( !_sh_surface->IsColorKeyed() ) 
                            {
                                _sh_surface->SetColorKey( _properties.color_key );
                            }
                        }
                        // This plane has associated correct surface
                        return;
                    }
                    else {
                        height_is_wrong = true;
                        break; //< a place for debug breakpoint
                    }
                }
                else {
                    width_is_wrong = true;
                    break; //< a place for debug breakpoint
                }
            }
            else {
                not_valid_blit_source = true;
                break; //< a place for debug breakpoint
            }
        }
        else {
            no_surface = true;
            break; //< a place for debug breakpoint
        }
    }

    if ( no_surface || not_valid_blit_source || width_is_wrong || height_is_wrong )
    {
        //  This creates new Surface
        Surface *p_new = surface.CreateSurface( _properties );

        if ( !p_new ) // then try default pixelformat
        {
            //  Set 'pixelformat' member to be 'out' type value
            _properties.pixelformat = ImageFormat::INVALID;
            p_new = surface.CreateSurface( _properties );

            if ( !p_new ) // then we can't do anything more
            {
                IMAGE_ERROR( Error("Could not cooperate with surface") ); 
            }
        }

        //  This releases previous Surface and assigns new one
        SetSurface( p_new );

        //  Make sure there is a clippper setup
        _sh_surface->SetClipper( Rect( 0, 0, _properties.width, _properties.height) );

        //  Set color-key if this is what descriptors says
        if ( _properties.has_color_key ) 
        {
            _sh_surface->SetColorKey(_properties.color_key); 
        }
    }
}

void ImageBase::SetColorKey( NormalPixel color )
{
    _properties.color_key = color.u;
    _properties.has_color_key = true;
}

void ImageBase::SetNoColorKey()
{
    _properties.has_color_key = false;
}

bool ImageBase::CheckIfBig( int maxPixels)
{
    if ( _properties.width * _properties.height > maxPixels )
    {
        _properties.system_memory = true;
        return true;
    }
    return false;
}

};//namespace Useless
