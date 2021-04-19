#ifndef __INCLUDED__USELESS_IMAGE_BASE_H__
#define __INCLUDED__USELESS_IMAGE_BASE_H__

#include "Useless/Graphic/Device/Surface.h"
#include "Useless/Graphic/Planes/IGraphics.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Util/Shares.h"

namespace Useless {
/*! \ingroup GfxPlanes
 *  Base for all surface based IGraphics
 *  Cannot be instantiated alone
 */
class USELESS_API ImageBase : public IGraphics
{
public:
    ImageBase   ();
    ImageBase   ( const ImageBase &image, const Rect &rect );
    
    ImageBase&
    operator =  ( const ImageBase &image );
    
    virtual ~ImageBase();

    virtual void
    Cooperate ( const Surface& surface );
    
    virtual const Surface *
    GetSurface      ()  const;
    
    virtual const Rect &
    GetRect         ()  const { return _rectangle; }

    virtual bool
    HasColorKey     ()  const { return _properties.has_color_key; }
    
    virtual NormalPixel
    GetColorKey     ()  const { return _properties.color_key; }
    
    virtual void // Set transparent color
    SetColorKey     ( NormalPixel color );
    virtual void // Disable transparent color
    SetNoColorKey   ();
    
    /*! Destination dimensions 
     *  are set by default to _rectangle size */
    virtual int 
    GetDestWidth    ()  const { return _width; }     
    virtual int
    GetDestHeight   ()  const { return _height;}     

    void  SetSurface( Surface *psurface );

    /*! Set destination image dimentions
     @{*/
    void  SetDestWidth ( int width  ) { _width =width; }
    void  SetDestHeight( int height ) { _height=height;}
    /*@}*/

    /*! Use this method to set better memory useage.
     *  Big images may reside in system memory, cause
     *  there might be more space for small images in video ram.
     *  Method sets memory type and returns true if image was big. */
    bool CheckIfBig( int maxPixels = 256*256);
    
    Share<Surface> GetSharedSurface() { return _sh_surface; }

protected:
    Share<Surface>      _sh_surface;
    Surf::Properties    _properties;
    Rect                _rectangle;
    int                 _width;
    int                 _height;

    virtual void Load() = 0;
};
//--------------------------------------------------------------------------

inline void ImageBase::SetSurface( Surface *psurface )
{
    _sh_surface.SetTarget(psurface);
    if ( psurface )
    {   Load();
    }
}

inline const Surface* ImageBase::GetSurface() const 
{
    return _sh_surface.GetTarget(); 
}

};//namespace Useless

#endif//__INCLUDED__USELESS_IMAGE_BASE_H__
