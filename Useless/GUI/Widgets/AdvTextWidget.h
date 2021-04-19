#ifndef __INCLUDED_USELESS_ADV_TEXT_WIDGET_H__
#define __INCLUDED_USELESS_ADV_TEXT_WIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Text/FontPainter.h"
#include "Useless/GUI/Widgets/FontWidget.h"

namespace Useless {

class USELESS_API AdvTextBase : virtual public FontWidget, private FontPainter
{
public:
    enum { LEFT=0, RIGHT=1, CENTER=2 };

    AdvTextBase(const Font &font, const Text &text="...", int line_spacing=0, int alignment=0);
    virtual ~AdvTextBase();

    virtual void  Repaint( const WidgetPainter &painter );
    virtual void  Resize ( int w, int h);

            void  SetText( const Text &text );
            Text  GetText() const { return FontPainter::GetText(); }
            void  Clear() { SetText(""); }

     virtual void  SetFont( const Font &font );
     virtual const Font& GetFont() const { return FontPainter::GetFont(); }


      AdvTextBase& operator<<(const Text &text);

    DEFINE_CYCLIC_VISITABLE(Widget);

    int  GetLineSpacing() const { return _line_spacing; }

    typedef FontPainter::Status Status;
    typedef FontPainter::Lines  Lines;
    typedef FontPainter::Words  Words;
    typedef FontPainter::Carrige Carrige;
    
    FontPainter::GetStatus;
    FontPainter::GetLines;
    FontPainter::GetWords;

    Carrige QueryPosition( const Pos &pos );

    FontWidget::GetWidth;
    FontWidget::GetHeight;

private:
    int _line_spacing;
    int _alignment;
};

};//namespace Useless

//-- Additional typedefs -- see FontChanger for details
#include "Useless/GUI/Widgets/FontChanger.h"
#include "Useless/GUI/Logic/HiliteLogic.h"
namespace Useless {
    typedef PaintedWidget< AdvTextBase > AdvTextWidget;
    typedef MultiFont< AdvTextBase >::Widget AdvTextAndFont;
    typedef ActiveMultiFont< AdvTextBase, HiliteLogic >::Widget HLAdvText;
    typedef ActivatedWidget< AdvTextBase > ActiveText;
};//namespace Useless

#endif//__INCLUDED_USELESS_ADV_TEXT_WIDGET_H__
