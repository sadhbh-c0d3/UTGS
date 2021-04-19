#include "UselessPch.h"

#include "MemSurface.h"
#include "Useless/System/TypeInfo.h"
#include "Useless/Graphic/Transfer/GenericTransfer.h"

namespace Useless {

MemSurface::MemSurface()
{
}

MemSurface::MemSurface( Surf::Properties &properties )
    :MemorySurface( properties )
{
}

MemSurface::~MemSurface()
{}

Surface* MemSurface::CreateSurface( Surf::Properties &properties ) const
{
    if ( properties.pixelformat == ImageFormat::INVALID || true == properties.alpha )
    {
        properties.pixelformat = ImageFormat::Format(32);
    }
    
    if ( properties.width > 0 && properties.height > 0 )
    {   
        return new MemSurface( properties );
    }
    else
    {
        return NULL;
    }
}

bool MemSurface::Lock( int channel_format, Surf::Ptr *srf_ptr )
{
    switch ( channel_format )
    {
    case COLOR:
        MemorySurface::Lock( &srf_ptr->ptr, &srf_ptr->pitch, &srf_ptr->format );
        if (!srf_ptr->ptr){ MemorySurface::LockAlpha( &srf_ptr->ptr, &srf_ptr->pitch ); srf_ptr->format = 8; }
        return !!srf_ptr->ptr;
    case ALPHA:
        MemorySurface::LockAlpha( &srf_ptr->ptr, &srf_ptr->pitch );
		srf_ptr->format = ImageFormat::SINGLE8;
        return !!srf_ptr->ptr;
    }
    return false;
}

void MemSurface::Unlock( int channel_format )
{
    switch ( channel_format )
    {
    case COLOR: MemorySurface::Unlock(); break;
    case ALPHA: MemorySurface::UnlockAlpha(); break;
    }
}


void MemSurface::Blit( int x, int y,
                   const Surface &src,
                   const Useless::Rect &src_rect,
                   const Surf::BlitFX &fx
                  )
{
    if ( typeid(src) == typeid(MemSurface) )
    {
        assert(0);
    }
    else
    {   throw Error("MemorySurface::Blit(): Incompatibile surface types");
    }
}

void MemSurface::Clear( unsigned long color,
                    const Useless::Rect &drc
                  )
{
    assert(0);
}

void MemSurface::GetProperties( Surf::Properties *prop ) const
{
    prop->width = _width;
    prop->height = _height;
    prop->pixelformat = ImageFormat::Format( _color_bits );
    prop->alpha = _alpha_plane.GetSize()>0;
    prop->color = _color_plane.GetSize()>0;
}

bool MemSurface::SetProperties( const Surf::Properties &prop )
{
    if ( prop.width == _width &&
         prop.height == _height &&
         prop.pixelformat == ImageFormat::Format( _color_bits ) &&
         prop.alpha && _alpha_plane.GetSize()>0 &&
         prop.color && _color_plane.GetSize()>0 )
    {
        return true;
    }
    assert(0);
    return false;
}

bool MemSurface::SetClipper( const RectList &region )
{
    //assert(0); cannot assert since it causes Image crashes
    return false;
}

RectList MemSurface::GetClipper( const Rect &crop ) const
{
    assert(0);
    return RectList();
}

void MemSurface::SetConstantAlpha( int alpha )
{
    assert(0);
}

int  MemSurface::GetConstantAlpha() const
{
    return 255;
}

bool MemSurface::operator==( const Surface& srf )
{
    if ( typeid(srf)==typeid(const MemSurface) )
    {
        const MemSurface &s = (const MemSurface&)srf;
        return (
            s._width==_width && s._height==_height && s._color_bits==_color_bits &&
            s._alpha_plane.GetSize()==_alpha_plane.GetSize()
            );
    }
    return false;
}

bool MemSurface::operator!=( const Surface& s )
{
    return !(*this==s);
}

};//namespace Useless
