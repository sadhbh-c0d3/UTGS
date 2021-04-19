#include "UselessPch.h"

#include "ImageDotTracer.h"
#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/Pixel/PixelTypes.h"
#include "Useless/Graphic/Pixel/PixelPointer.h"
#include "Useless/Util/LockObj.h"
#include "Useless/Graphic/Planes/MemSurface.h"

namespace Useless {

int ImageDotTracer::_Dx = 0;
int ImageDotTracer::_Dy = 0;
int ImageDotTracer::_Dw = 0;
int ImageDotTracer::_Dh = 0;

typedef std::vector< std::pair<int, std::vector<int> > > __Markers;

template< class PixelType >

__Markers GetMarkers( const void *p_data, int pitch, const Rect &area, PixelType &background )
{
    int ix,iy,x,y, x0= area.GetX(), y0=area.GetY(), w=area.GetW(), h=area.GetH();
 
    PixelPointer<PixelType> p_pixel( (PixelType*)p_data, pitch, area );
    
    p_pixel.MoveNLines(y0).MoveNPixels(x0);

    __Markers markers;

    PixelType marker_color = *p_pixel;
    background = *(p_pixel(1,0));

    bool old_font = *(p_pixel(0,1))==marker_color;

    for ( y=0, iy=0; y!= ( (old_font)? 1:h ); ++y )
    {
        if ( *p_pixel == marker_color )
        {
            markers.push_back( std::make_pair(y+y0, std::vector<int>() ) );

            PixelPointer<PixelType> p_pixel2(p_pixel);
            for ( x=0, ix=0; x!=w; ++x )
            {
                if ( *p_pixel2 == marker_color ) 
                { 
                    markers[iy].second.push_back(x+x0);
                    ++ix;
                }

                p_pixel2.NextPixel();
            }
            ++iy;
        }
        p_pixel.NextLine();
    }

    assert( !markers.empty() );
    if ( y==1 || (*markers.rbegin()).first != y0+y-1 ) 
    {
        markers.push_back( std::make_pair(h+y0, std::vector<int>() ) );
        markers[iy].second.push_back(x0); 
    }

    return markers;
}

ImageDotTracer::ImageDotTracer( const RefImage &ref_image )
    : ImageDicer( ref_image ), _max_width(0), _max_height(0)
{
    InitTracer();
}

void ImageDotTracer::InitTracer()
{
    typedef Useless::LockObj<LockableSurface,true> SurfLock;
    Surf::Ptr srf_ptr;
    
    IGraphics &ref_image = GetImage(SOURCE);
    const Surface *s = ref_image.GetSurface();

    if (!s) // then use default memory surface
    {
        ref_image.Cooperate( MemSurface() );
        s = ref_image.GetSurface();
    }

    int w = s->GetWidth();
    int h = s->GetHeight();

    SurfLock guard_surface(
        static_cast<LockableSurface&>(const_cast<Surface&>(*s)), Surface::COLOR, &srf_ptr );

    Rect area = ref_image.GetRect();


    __Markers markers;
    NormalPixel nckey;
    void *p_data = srf_ptr.ptr;
    int pitch = srf_ptr.pitch;
    int bpp = srf_ptr.format;

    if (bpp == 32) 
    { 
        PixelType8888 color_key;
        markers = GetMarkers( p_data, pitch, area, color_key ); 
        nckey = color_key.Get();
    }
    else if (bpp == 24) 
    { 
        PixelType888 color_key;
        markers = GetMarkers( p_data, pitch, area, color_key ); 
        nckey = color_key.Get();
    }
    else if (bpp == 16) 
    { 
        PixelType565 color_key;
        markers = GetMarkers( p_data, pitch, area, color_key ); 
        nckey = color_key.Get();
    }
    else if (bpp == 8 )
    {
        unsigned char color_key;
        markers = GetMarkers( p_data, pitch, area, color_key ); 
    }

    {int i,j,n,m,x,y,w,h;

        for ( j=0, m=markers.size(); j<m-1; ++j )
        {
            y = markers[j].first+1;
            h = markers[j+1].first -  y;
            if (_max_height<h+_Dh) { _max_height = h+_Dh; }

            for ( i=0, n=markers[j].second.size(); i<n-1; ++i )
            {
                x = markers[j].second[i]+1;
                w = markers[j].second[i+1] - x;
                if (_max_width<w) { _max_width = w; }

                ImageDicer::Dice( Rect(x+_Dx,y+_Dy,w+_Dw,h+_Dh) );
            }
        }
    }
    if ( typeid(GetImage(SOURCE))==typeid(Image) ) 
    {
        SetColorKey(nckey.u); 
    }
}

};//namespace Useless
