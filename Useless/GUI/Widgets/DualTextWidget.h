#ifndef __INLCUDED__USELESS__DUAL_TEXT_WIDGET_H__
#define __INLCUDED__USELESS__DUAL_TEXT_WIDGET_H__

#include "Useless/GUI/Widgets/FontWidget.h"

namespace Useless {

class USELESS_API BaseDualText : virtual public FontWidget
{
public:
    BaseDualText( int width, const Font &font,
                    const Text &left="", const Text &right="", int dots_shift_down=2 );
    virtual ~BaseDualText();

    virtual void Repaint( const WidgetPainter &painter );
    virtual void Resize( int w, int h );

    virtual void  SetFont( const Font &font );
    virtual const Font& GetFont() const { return _font; }

    void  SetDualText( const Text &left, const Text &right );
    const Text& GetLeftText () const { return _left_text; }
    const Text& GetRightText() const { return _right_text; }

    void SetDotCode( int code );
    int  GetDotCode() const { return _dot_code; }

    DEFINE_CYCLIC_VISITABLE(Widget);

private:
    Text    _left_text;
    Text    _dots;
    Text    _right_text;
    Font    _font;
    int     _state;
    int     _dot_code;
    int     _left_width;
    int     _right_width;
    int     _dots_shift;

protected:
    void GenereateDots();
    int  GetH() const { return (_font.GetHeight() + ((_dots_shift>0)? _dots_shift : 0)); }
};

};//namespace Useless

//-- Additional typedefs -- see FontChanger for details
#include "Useless/GUI/Widgets/FontChanger.h"
#include "Useless/GUI/Logic/HiliteLogic.h"
namespace Useless {
    typedef PaintedWidget< BaseDualText > DualTextWidget;
    typedef MultiFont< BaseDualText >::Widget DualTextAndFont;
    typedef ActiveMultiFont< BaseDualText, HiliteLogic >::Widget HLDualText;
};//namespace Useless

#endif//__INLCUDED__USELESS__DUAL_TEXT_WIDGET_H__
