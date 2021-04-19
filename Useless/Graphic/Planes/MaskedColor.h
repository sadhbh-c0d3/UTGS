#ifndef __INCLUDED__USELESS__MASKED_COLOR_H__
#define __INCLUDED__USELESS__MASKED_COLOR_H__

#include "Useless/Graphic/Planes/ImageBase.h"
#include "Useless/Graphic/FileIO/ImageLoader.h"

namespace Useless {

/*! MaskedColor is IGraphics which is painted 
 */
class USELESS_API MaskedColor : public ImageBase
{
public:
    MaskedColor();
    MaskedColor( const char *alpha_file, NormalPixel color = 0L );
    MaskedColor( const MaskedColor&, const Rect& );
    virtual ~MaskedColor();

    virtual SPointer<IGraphics> Copy()                 const { return new MaskedColor(*this); }
    virtual SPointer<IGraphics> Copy(const Rect &rect) const { return new MaskedColor(*this, rect); }

    void        SetColor( NormalPixel color ) { _color=color; }
    NormalPixel GetColor() const              { return _color; }

    void InvertMask( bool t ) { _inverted=t; }

    void SetAlphaFactors( int alpha_0, int alpha_1 ) { _alpha_0=alpha_0; _alpha_1=alpha_1; }
    int  GetAlphaFactor ( int n=1 ) const            { return ((n)? _alpha_1 : _alpha_0); }

    void SetAngle( int degrees ) { _angle=degrees; }
    int  GetAngle() const        { return _angle; }

    virtual void  BlitTo( int x, int y, OGraphics& output);
    virtual void  MultiBlitTo( OGraphics&, const PointList&, const RectList& );

    virtual bool  IsSolid()      const { return false; }
    virtual bool  IsColorKeyed() const { return false; }
    virtual void  SetColorKey(Types::ULONG c) { assert(0&&"MaskedColor::SetColorKey(): Operation not allowed"); }
    virtual void  SetNoColorKey() { assert(0&&"MaskedColor::SetNoColorKey(): Operation not allowed"); }

protected:
    virtual void Load();
    NormalPixel                         _color;
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
REGISTER_COPIER(Useless,IGraphics,MaskedColor);
REGISTER_COPIER(Useless,ImageBase,MaskedColor);

#endif//__INCLUDED__USELESS__MASKED_COLOR_H__
