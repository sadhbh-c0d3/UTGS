#ifndef __INCLUDED_USELESS_TRANSPARENT_IMAGE_H__
#define __INCLUDED_USELESS_TRANSPARENT_IMAGE_H__

#include "Useless/Graphic/Planes/Image.h"

namespace Useless {

/*! \ingroup GfxPlanes
 *  In addition to Image, this class loads any !!! 8bit !!! alpha image format from disk
 *  and places it onto alpha-Surface using Surface::LockAlpha()
 */
class USELESS_API TransparentImage : public Image
{
public:
                        TransparentImage();
                        TransparentImage( const char *color_plane, const char *alpha_plane );
                        TransparentImage( const TransparentImage &image, const Rect &rect );
    virtual            ~TransparentImage();
    virtual SPointer<IGraphics> Copy()                 const { return new TransparentImage(*this); }
    virtual SPointer<IGraphics> Copy(const Rect &rect) const { return new TransparentImage(*this, rect); }

    void SetAlphaFactors( int alpha_0, int alpha_1 ) { _alpha_0=alpha_0; _alpha_1=alpha_1; }
    int  GetAlphaFactor ( int n=1 ) const            { return ((n)? _alpha_1 : _alpha_0); }

    void SetAngle( int degrees ) { _angle=degrees; }
    int  GetAngle() const        { return _angle; }

    virtual void  BlitTo( int x, int y, OGraphics& output);
    virtual void  MultiBlitTo( OGraphics&, const PointList&, const RectList& );


    virtual bool        IsSolid()      const { return false; }
    virtual bool        HasColorKey()  const { return false; }
    virtual void        SetColorKey(Types::ULONG c) { assert(0&&"TransparentImage::SetColorKey(): Operation not allowed"); }
    virtual void        SetNoColorKey() { assert(0&&"TransparentImage::SetNoColorKey(): Operation not allowed"); }
    
protected:
    virtual void Load();
    std::string                         _alpha_file;
    Useless::SPointer< Useless::ImageLoader > _alpha_loader;
    bool                                _alpha_loaded;
    bool                                _inverted;
    int                                 _alpha_0;
    int                                 _alpha_1;
    int                                 _angle;
};

};//namespace Useless


#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,IGraphics,TransparentImage);
REGISTER_COPIER(Useless,ImageBase,TransparentImage);
REGISTER_COPIER(Useless,Image,TransparentImage);

#endif//__INCLUDED_USELESS_TRANSPARENT_IMAGE_H__
