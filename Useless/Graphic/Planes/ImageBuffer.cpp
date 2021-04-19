#include "UselessPch.h"


#include "Useless/Graphic/Planes/ImageBuffer.h"
#include "Useless/Graphic/Device/Screen.h"
#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/Transfer/SurfaceWriter.h"
#include "Useless/Util/Storage/MemBlock.h"


#include "Useless/ErrorConfig.h"
#define   IMAGEBUF_WARNING(e) APP_WARNING(e, "ImageBuffer")
#define     IMAGEBUF_ERROR(e) APP_WARNING(e, "ImageBuffer")

namespace Useless {

//-- Zero poroperties
ImageBuffer::ImageBuffer()
{
    _properties.width  = _width = 0;
    _properties.height = _height = 0;
    _properties.renderable = true;
}

//-- Initialize ImageBuffer structure ( Surface will not initialize )
ImageBuffer::ImageBuffer(int w, int h)
{
    _properties.width  = w;
    _properties.height = h;
    _properties.renderable = true;
    _rectangle = Rect(0, 0, _properties.width, _properties.height);
    _width = _rectangle.GetW(); _height = _rectangle.GetH();
}

//-- Create copy of ImageBuffer structure
ImageBuffer::ImageBuffer(const ImageBuffer &source, const Rect &rect): ImageBase( source, rect )
{
}

//-- Create ImageBuffer, copy pixels from Surface
ImageBuffer::ImageBuffer( Surface *pSurface, const Rect &rect)
{
    Surf::Properties props;
    pSurface->GetProperties( &props );
    props.width = rect.GetW();
    props.height = rect.GetH();
    props.renderable = false;
    props.texture = true;
    // Only 24-bit COLOR is copied
    int pixelFormat = 24;
    int bytesPerPel = 3;
    _sh_surface.SetTarget( pSurface->CreateSurface( props ));
    MemBlock block( props.width * props.height * 3 );
    // Download pixels from source
    SPointer< PixelTransfer > pT = pSurface->CreateReader( Surface::COLOR );
    pT->SetDestination( pSurface->GetWidth(), pSurface->GetHeight(), props.width * bytesPerPel, pixelFormat, 0, 0);
    pT->Transform();
    pT->Store( block.GetPtr(), rect );
    // Upload pixels to surface
    pT = _sh_surface->CreateWriter( Surface::COLOR );
    pT->SetSource( props.width, props.height, props.width * bytesPerPel, pixelFormat, 0, 0);
    pT->Fetch( block.GetPtr(), rect );
    pT->Transform();
    // Synchronize properties
    _sh_surface->GetProperties( &_properties );
    _rectangle = Rect(0, 0, _properties.width, _properties.height);
    _width = _rectangle.GetW();
    _height = _rectangle.GetH();
}

//-- Create ImageBuffer from other shared Surface
ImageBuffer::ImageBuffer( Share<Surface> sh_surface, const Rect &rect ) 
{
    _rectangle=rect;
    _sh_surface=sh_surface;
    _sh_surface->GetProperties( &_properties );
    if(!rect) _rectangle = Rect(0, 0, _properties.width, _properties.height);

    _width = _rectangle.GetW(); _height = _rectangle.GetH();
}

ImageBuffer::~ImageBuffer()
{}

//-- Normal assignment operator
ImageBuffer &ImageBuffer::operator=(const ImageBuffer &image)
{
    ImageBase::operator=(image);
    return *this;
}

SPointer<IGraphics> ImageBuffer::QueryIGraphics( const Rect &cut )
{
    return new ImageBuffer( _sh_surface.GetTarget(), cut );
}

//-- Use delivered Screen as target for this iImageBuffer ( initializes Surface )
void ImageBuffer::Create( const Screen& screen )
{
    Cooperate( *screen.GetSurface() );
}

//-- Resize ImageBuffer ( Surface is being created asynchronious )
void ImageBuffer::Resize(int w, int h)
{
    assert( _rectangle==Rect( 0, 0, _properties.width, _properties.height) && "ImageBuffer::Resize(): Only master ImageBuffer can be resized" );
    _sh_surface.SetTarget(NULL);
    _rectangle.w = _properties.width  = w;
    _rectangle.h = _properties.height = h;

    _width = _rectangle.GetW(); _height = _rectangle.GetH();
}

//-- Clear rectangular area ( if area=0 full ImageBuffer area )
void ImageBuffer::Clear( NormalPixel color, const Rect &area )
{
    if ( !GetSurface() ) //-- Impossibile operation
        IMAGEBUF_ERROR( Error("ImageBuffer::Clear(): No surface was initialized") );

    if ( !area )
        _sh_surface->Clear( color.u, GetRect() );
    else
        _sh_surface->Clear( color.u, area & GetRect() );
}

//-- Setup Clipper from multiple rectangles
void ImageBuffer::SetClipper( const RectList &rects )
{
    if ( !GetSurface() )
        IMAGEBUF_ERROR( Error("ImageBuffer::SetClipper(): No screen, use ImageBuffer::Create() first") );
    _sh_surface->SetClipper( rects );
}

//-- Get Clipper object
RectList ImageBuffer::GetClipper( const Rect &crop ) const
{
    if ( !GetSurface() )
        IMAGEBUF_ERROR( Error("ImageBuffer::SetClipper(): No screen, use ImageBuffer::Create() first") );

    return _sh_surface->GetClipper( crop );
}

};// namespace Useless

