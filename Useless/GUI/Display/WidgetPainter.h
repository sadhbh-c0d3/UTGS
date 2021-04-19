#ifndef __INCLUDED_USELESS_WIDGET_PAINTER_H__
#define __INCLUDED_USELESS_WIDGET_PAINTER_H__

#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"

namespace Useless {

//USES
    class DisplayMan;
    class Widget;

class USELESS_API WidgetPainter : public Painter
{
public:
    WidgetPainter();
    WidgetPainter(         DisplayMan&, const Pos& =Pos());
    WidgetPainter(const WidgetPainter&, const Pos& =Pos());
    WidgetPainter(const WidgetPainter&, Widget& );
    WidgetPainter& operator =( const WidgetPainter& );

    Widget* GetWidget() const { return _widget; }
    
    ImageBuffer GetImageRect( const Rect &area ) const;

    bool Intersect(const Rect &rc) { return Painter::Intersect(rc);}

    bool Intersect();
    void AllowFullArea();

private:
    DisplayMan *_display;
    Widget     *_widget;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_WIDGET_PAINTER_H__
