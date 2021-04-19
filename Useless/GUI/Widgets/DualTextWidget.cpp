#include "UselessPch.h"

#include "DualTextWidget.h"

#include "Useless/ErrorConfig.h"

namespace Useless {

BaseDualText::BaseDualText( int width, const Font &font, const Text &left, const Text &right, int shift )
    : _left_text(left), _right_text(right), _font(font), _dot_code('.'), _dots_shift(shift)
{
    _state = 1;
    GenereateDots();
    SetDimensions( width, GetH() );
}

BaseDualText::~BaseDualText()
{
}

void BaseDualText::Repaint( const WidgetPainter &painter )
{
    Pos dots_pos = Pos(_left_width, _dots_shift );
    Pos right_pos = Pos( GetWidth()-_right_width, 0 );
    _font.Paint( painter, _left_text );
    _font.Paint( painter, _dots, dots_pos );
    _font.Paint( painter, _right_text, right_pos );
}

void BaseDualText::Resize( int w, int h )
{
    if ( h && h!=GetHeight() )
    HUGE_LOG( Error("Resize(%i,%i) height was ommited",w,h), "BaseDualText" );
    Rect r = ( GetBoundingRect()-GetPosition() ) | Rect(0,0,w,h);
    SetDimensions( w, GetH() );
    GenereateDots();
}

void BaseDualText::SetFont( const Font &font )
{
    _font = font;
    SetDimensions( GetWidth(), GetH() );
    GenereateDots();
}

void BaseDualText::SetDualText( const Text &left, const Text &right ) 
{
    _left_text=left;
    _right_text=right;
    SetDimensions( GetWidth(), GetH() );
    GenereateDots();
}

void BaseDualText::SetDotCode( int code ) 
{
    _dot_code = code;
    SetDimensions( GetWidth(), GetH() );
    GenereateDots();
}


void BaseDualText::GenereateDots()
{
    _dots = Text();

    _left_width = _font.GetWidth( _left_text );
    _right_width = _font.GetWidth( _right_text );

    int d = GetWidth() - (_left_width+_right_width);
    int w = GetFont().GetWidth(_dot_code);
    int s = GetFont().GetKerning('.','.');
   
    if ( (w+s>0) && (d+s>0) )
    {
        int n = ( d+s )/( w+s );

        for ( int i=0; i!=n; ++i )
        {
            _dots += _dot_code;
        }
    }
}


};//namespace Useless
