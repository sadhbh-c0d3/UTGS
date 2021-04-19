#include "UselessPch.h"

#include "Useless/GUI/Widgets/AdvTextWidget.h"

namespace Useless {

AdvTextBase::AdvTextBase(const Font &font, const Text &text, int ls, int algn )
    :FontPainter(font,text,0), _line_spacing(ls), _alignment(algn)
{
}

AdvTextBase::~AdvTextBase()
{}

void AdvTextBase::Repaint( const WidgetPainter &painter )
{
    int width = FontPainter::GetWidth();
    switch(_alignment)
    {
        case LEFT:
        FontPainter::Paint( painter, Pos(0,0), _line_spacing, LEFT );
        break;

        case RIGHT:
        FontPainter::Paint( painter, Pos(width,0), _line_spacing, RIGHT );
        break;

        case CENTER:
        FontPainter::Paint( painter, Pos(width/2,0), _line_spacing, CENTER );
        break;
    }
}

void AdvTextBase::Resize( int w, int h )
{
    if ( FontPainter::GetWidth()!=w )
    {
        FontPainter::SetWidth(w);
        SetDimensions( w, FontPainter::GetHeight(_line_spacing) );
    }
}

void AdvTextBase::SetText(const Text &text)
{
    FontPainter::SetText(text);
    SetDimensions( FontPainter::GetWidth(), FontPainter::GetHeight(_line_spacing) );
}

void AdvTextBase::SetFont(const Font &font)
{
    FontPainter::SetFont(font);
    SetDimensions( FontPainter::GetWidth(), FontPainter::GetHeight(_line_spacing) );
}

AdvTextBase& AdvTextBase::operator<<(const Text &text)
{
    SetText( GetText() += text );
    return *this;
}

AdvTextBase::Carrige AdvTextBase::QueryPosition( const Pos &pos )
{
    int width = FontPainter::GetWidth();
    switch(_alignment)
    {
    case LEFT:
        return FontPainter::QueryPosition( pos - Pos(0,0), _line_spacing, LEFT );

    case RIGHT:
        return FontPainter::QueryPosition( pos - Pos(width,0), _line_spacing, RIGHT );

    case CENTER:
        return FontPainter::QueryPosition( pos - Pos(width/2,0), _line_spacing, CENTER );
    }
	AdvTextBase::Carrige car;
    car._letterInText = -1;
    car._letterInWord = -1;
    car._wordInText = -1;
    car._wordInRow = -1;
    car._row = -1;
	return car;
}


};//namespace Useless
