#ifndef __INCLUDED_USELESS_TEXT_FIELD_WIDGET_H__
#define __INCLUDED_USELESS_TEXT_FIELD_WIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Widgets/FontWidget.h"

namespace Useless {

/*! \ingroup Widgets
 *  TextWidget paints a single line text with one font.
 */
class USELESS_API BaseTextWidget : virtual public FontWidget
{
public:
    BaseTextWidget(const Font &font, const Text &text="...");
    virtual ~BaseTextWidget();

    virtual void  Repaint( const WidgetPainter &painter );

            void  SetText(const Text &text);
            Text  GetText() const { return _text; }
            void  Clear();

            virtual void  SetFont( const Font &font );
            virtual const Font& GetFont() const { return _font; }

      BaseTextWidget& operator<<(const Text &text);

    DEFINE_CYCLIC_VISITABLE(Widget);

private:
    Font       _font;
    Text       _text;
};

};//namespace Useless

//-- Additional typedefs -- see FontChanger for details
#include "Useless/GUI/Widgets/FontChanger.h"
#include "Useless/GUI/Logic/HiliteLogic.h"
namespace Useless {
    typedef PaintedWidget< BaseTextWidget > TextWidget;
    typedef MultiFont< BaseTextWidget >::Widget TextAndFont;
    typedef ActiveMultiFont< BaseTextWidget, HiliteLogic >::Widget HLText;
};//namespace Useless

#endif//__INCLUDED_USELESS_TEXT_FIELD_WIDGET_H__
