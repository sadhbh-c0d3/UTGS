#ifndef __INLCUDED__USELESS__FONT_SKIN_H__
#define __INLCUDED__USELESS__FONT_SKIN_H__

#include "Useless/GUI/Text/Text.h"
#include "Useless/Graphic/Planes/Painter.h"

namespace Useless {

class USELESS_API FontSkin
{
public:
    virtual ~FontSkin() {}
    /*! Vector of letter positions */
	typedef StupidVector<int> Letters;
    /*! Font styles */
    enum { NORMAL=0, BOLD=1, ITALIC=2, UNDERLINE=4, INVERT=8 };

    /*! Paint text at given position using painter */
    virtual void  Paint( const Painter&, const Text&, const Pos &p=Pos() ) = 0;

    /*! Setup font parameters using preformated text */
    virtual void  Preformat( const Text &) = 0;

    /*! Set text color */
    virtual void  SetColor( NormalPixel ) = 0;
    /*! Set text opacity */
    virtual void  SetOpacity( float ) = 0;
    /*! Set backdrop color */
	virtual void  SetBackColor( NormalPixel ) = 0;
    /*! Set backdrop opacity */
    virtual void  SetBackOpacity( float ) = 0;
    /*! Get text opacity */
	virtual float GetOpacity() const = 0;
    /*! Get backdrop opacity */
	virtual float GetBackOpacity() const = 0;
    /*! Get text color */
	virtual NormalPixel GetColor() const = 0;
    /*! Get backdrop color */
	virtual NormalPixel GetBackColor() const = 0;

    /*! Set font style (bold,italic,underline,inverted) */
    virtual void SetStyle( int ) = 0;
    /*! Set character spacing */
    virtual void SetKerning( int k ) = 0;
    /*! Set distance between text lines */
	virtual void SetSpacing( int s ) = 0;
    /*! Get actual font style */
	virtual int  GetStyle()   const = 0;
    /*! Get actual character spacing */
    virtual int  GetKerning() const = 0;
    /*! Get actual line spacing */
	virtual int  GetSpacing() const = 0;
    /*! Get font height */
	virtual int  GetHeight()  const = 0;
    /*! Get character width */
	virtual int  GetWidth ( UniCode ) const = 0;
    /*! Get preformated text width */
	virtual int  GetWidth ( const Text& ) const = 0;
    /*! Get preformated text character positions */
	virtual Letters GetStats( const Text& ) const = 0;

    virtual bool IsColorKeyed() const { return false; }
    virtual NormalPixel GetColorKey() const { return false; }
};

};//namespace Useless
#endif//__INLCUDED__USELESS__FONT_SKIN_H__
