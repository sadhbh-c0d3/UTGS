#ifndef __INCLUDED__USELESS__ALPHA_FONT_H__
#define __INCLUDED__USELESS__ALPHA_FONT_H__

#include "Useless/GUI/Skins/FacedFont.h"

namespace Useless {

class USELESS_API AlphaFont : public FacedFont
{
public:
    AlphaFont( const char *alpha_image, const char *font_desc );
    AlphaFont( const FacedFont &f );
    virtual ~AlphaFont();

    virtual void  Paint( const Painter&, const Text&, const Pos &p=Pos() );

    virtual void  SetStyle( int );
    virtual void  SetColor( NormalPixel );
    virtual void  SetOpacity( float );
	virtual void  SetBackColor( NormalPixel );
    virtual void  SetBackOpacity( float );
	virtual float GetOpacity() const;
	virtual float GetBackOpacity() const;
	virtual NormalPixel GetColor() const;
	virtual NormalPixel GetBackColor() const;

private:
    float       _opacity;
    float       _back_opacity;
    NormalPixel _back_color;
    
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,FontSkin,AlphaFont);
REGISTER_COPIER(Useless,FacedFont,AlphaFont);

#endif//__INCLUDED__USELESS__ALPHA_FONT_H__
