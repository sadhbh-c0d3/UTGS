#include "UselessPch.h"

#include "Useless/GUI/Widgets/TextWidget.h"

namespace Useless {

BaseTextWidget::BaseTextWidget(const Font& font, const Text &text): _font(font), _text(text)
{}

BaseTextWidget::~BaseTextWidget()
{}

void BaseTextWidget::Repaint( const WidgetPainter &painter )
{
    _font.Paint( painter, _text);
}
 

void BaseTextWidget::SetText(const Text &text)
{
    SetDimensions( _font.GetWidth(_text=text), _font.GetHeight() );
}

void BaseTextWidget::SetFont(const Font &font)
{
    _font = font;
    SetDimensions( _font.GetWidth(_text), _font.GetHeight() );
}

void BaseTextWidget::Clear()
{
    _text = Text();
    SetDimensions(0,0);
}

BaseTextWidget& BaseTextWidget::operator<<(const Text &text)
{
    SetDimensions( _font.GetWidth(_text+=text), _font.GetHeight() );
    return *this;
}


};//namespace Useless
