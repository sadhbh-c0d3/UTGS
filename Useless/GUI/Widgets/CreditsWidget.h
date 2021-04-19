#ifndef __INCLUDED_USELESS_CREDITS_WIDGET_H__
#define __INCLUDED_USELESS_CREDITS_WIDGET_H__

#include "Useless/GUI/Misc/Paylist.h"
#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/System/Timer.h"

namespace Useless {

class USELESS_API CreditsWidget : virtual public Widget, public Paylist
{
public:
    /*! xml_file - credits script
        margins  - % of widget width
    */
    CreditsWidget( const char *xml_file, int pixel_per_second=50, int margins=5 );
    CreditsWidget( XMLIterator i, int pixel_per_second=50, int margins=5 );
    ~CreditsWidget();

    virtual void Repaint( const WidgetPainter &painter );
    virtual void Resize (int w, int h);
    virtual void Setup();

    void Restart();
    void Advance();
    bool Finished() const;

private:
    Timer  _time;
    double _advance;  
    int    _margins;
    int    _pixel_speed;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_CREDITS_WIDGET_H__
