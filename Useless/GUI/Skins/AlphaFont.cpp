#include "UselessPch.h"

#include "AlphaFont.h"
#include "Useless/Graphic/Planes/MaskedColor.h"

namespace Useless {

AlphaFont::AlphaFont( const char *alpha_image, const char *font_desc )
    :FacedFont( MaskedColor(alpha_image), font_desc ),
    _opacity(1.0), _back_opacity(0.0), _back_color(0xFFFFFF)
{
    _color = 0xFFFFFF;
}

AlphaFont::AlphaFont( const FacedFont &f )
    :FacedFont( f ), _opacity(1.0), _back_opacity(0.0), _back_color(0xFFFFFF)
{}

AlphaFont::~AlphaFont()
{}

void AlphaFont::Paint( const Painter &painter, const Text &text, const Pos &pos )
{
    PreformatedText preformated = text;
    PreformatedText::ConstIterator subtext_i = preformated.ConstBegin();
    Pos carret = pos;

    MaskedColor &mask = (MaskedColor &)( (*_faces).GetImage( ImageDicer::SOURCE ) );

    for (; !!subtext_i; ++subtext_i )
    {
        const UniCode *p = &*preformated.begin() + (*subtext_i).start;
        const UniCode *q = &*preformated.begin() + (*subtext_i).end;

        ApplyMarkup( *subtext_i );

        if ( p==q ) { continue; }

        PointList points;
        RectList rects;
        ThinkQuick( p, q, points, rects, carret );
        
        if ( _back_opacity > 0.0 )
        {
            mask.InvertMask(true);
            mask.SetAlphaFactors( 0, 255.0*_back_opacity );
            mask.SetColor( _back_color );
            painter.ProjectImages( mask, points, rects );
        }
        if ( _opacity > 0.0 )
        {
            mask.InvertMask(false);
            mask.SetAlphaFactors( 0, 255.0*_opacity );
            mask.SetColor( _color );
            painter.ProjectImages( mask, points, rects );
        }
    }
}

void AlphaFont::SetStyle( int style_flags )
{
    if (!(_style & INVERT) && (style_flags & INVERT) ||
         (_style & INVERT) &&!(style_flags & INVERT))
    {
        std::swap( _color, _back_color );
        std::swap( _opacity, _back_opacity );
    }
    FacedFont::SetStyle( style_flags & ~INVERT );

    _style = style_flags;
}

void AlphaFont::SetColor( NormalPixel color )
{
    if (_style & INVERT) _back_color = color;
    else _color = color;
}

void AlphaFont::SetOpacity( float opacity )
{
    if (_style & INVERT) _back_opacity = opacity;
    else _opacity = opacity;
}

void AlphaFont::SetBackColor( NormalPixel color )
{
    if (_style & INVERT) _color = color;
    else _back_color = color;
}

void AlphaFont::SetBackOpacity( float opacity )
{
    if (_style & INVERT) _opacity = opacity;
    else _back_opacity = opacity;
}

float AlphaFont::GetOpacity() const
{
    return (_style & INVERT)? _back_opacity : _opacity;
}

float AlphaFont::GetBackOpacity() const
{
    return (_style & INVERT)? _opacity : _back_opacity;
}

NormalPixel AlphaFont::GetColor() const
{
    return (_style & INVERT)? _back_color : _color;
}

NormalPixel AlphaFont::GetBackColor() const
{
    return (_style & INVERT)? _color : _back_color;
}



};//namespace Useless
