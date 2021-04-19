#ifndef __INCLUDED__USELESS__FACED_FONT_H__
#define __INCLUDED__USELESS__FACED_FONT_H__

#pragma warning(disable:4786)
#include "Useless/GUI/Text/PreformatedText.h"
#include "Useless/GUI/Skins/ImageDotTracer.h"
#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/GUI/Skins/FontSkin.h"
#include <fstream>

namespace Useless {

class USELESS_API FacedFont : public FontSkin
{
public:
    typedef ImageDotTracer::RefImage RefImage;

    FacedFont( const RefImage &ref_image, const char *desc_file_name );
    virtual ~FacedFont();

    virtual void Paint( const Painter&, const Text&, const Pos &p=Pos() );
    virtual void Preformat( const Text &);

    virtual void        SetColor( NormalPixel );
    virtual NormalPixel GetColor() const { return _color; }


    typedef StupidVector<int> Letters;
    enum { NORMAL=0, BOLD=1, ITALIC=2, UNDERLINE=4, INVERT=8 };

    virtual void SetStyle( int );
    virtual void SetKerning( int k ) { _kerning=k; }
    virtual void SetSpacing( int s ) { _spacing=s; }

    virtual int  GetStyle()   const  { return _style; }
    virtual int  GetKerning() const  { return _kerning; }
    virtual int  GetSpacing() const  { return _spacing; }
    virtual int  GetHeight()  const  { return (*_faces).GetMaxHeight(); }

    virtual Letters GetStats( const Text& ) const;

    virtual int  GetWidth ( UniCode ) const;
    virtual int  GetWidth ( const Text& ) const;

protected:
    int         _kerning;		// pixels between characters
    int         _spacing;		// lines between text rows
    int         _style;			// combination of flags
    NormalPixel _color;			// actual color
    NormalPixel _prev_color;	// previous color
    int         _offset;		// an ImageDicer index where the first character of actual style resides

    typedef std::pair< NormalPixel, int > ColorOffset;
    typedef AssocVector< int, ColorOffset > Styles;

    SPointer< Styles > _styles; // style => color associationa
    SPointer< ImageDotTracer > _faces;

    void ApplyMarkup( const PreformatedText::Markup &mark );

    void Think( const PreformatedText &preformated,
            PointList &points,
            RectList &rects,
            const Pos &offset=Pos() );

    void ThinkQuick( const UniCode *begin, const UniCode *end,
            PointList &points,
            RectList &rects,
            Pos &carret );

public:
    // impossible operations: (do nothing)
    virtual void  SetOpacity( float ) {}
    virtual void  SetBackColor( NormalPixel ) {}
    virtual void  SetBackOpacity( float ) {}
    virtual float GetOpacity() const { return 1.0; }
    virtual float GetBackOpacity() const { return 0.0; }
    virtual NormalPixel GetBackColor() const { return 0L; }

public: // ???
    virtual bool IsColorKeyed() const { return (*_faces).IsColorKeyed(); }
    virtual NormalPixel GetColorKey() const { return (*_faces).GetColorKey(); }

};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,FontSkin,FacedFont);

#endif//__INCLUDED__USELESS__FACED_FONT_H__
