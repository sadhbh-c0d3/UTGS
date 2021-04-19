#ifndef __INCLUDED_USELESS_CTIVE_AREA_WIDGET_H__
#define __INCLUDED_USELESS_CTIVE_AREA_WIDGET_H__

#include "Useless/GUI/Widgets/ActiveWidget.h"

namespace Useless {

class USELESS_API ActiveAreaWidget : public ActiveWidget
{
public:
    /////////////////////////////////////////////////////////
    //  Reload this Signal if you want to paint anything
    mutable Signal_1<WidgetPainter>  OnPaint;       // Paiter

    ActiveAreaWidget( int w, int h) { SetDimensions(w,h); }
    virtual ~ActiveAreaWidget() {}

    virtual void Repaint( const WidgetPainter& painter ) { OnPaint.Send_1(painter); }
    virtual void Resize ( int w, int h) { SetDimensions(w,h); }

};

};//namespace Useless

#endif//__INCLUDED_USELESS_CTIVE_AREA_WIDGET_H__
