#ifndef __INCLUDED_USELESS_FRAME_WIDGET_H__
#define __INCLUDED_USELESS_FRAME_WIDGET_H__

/*
 *    $Id: FrameWidget.h,v 1.10 2002/09/12 14:27:36 koolas Exp $
 *
 *    NAME
 *        FrameWidget
 *
 *    PURPOSE
 *        Frame with FrameWidget look
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Skins/FrameSkin.h"

namespace Useless {

//USES
    class FrameStyle;

/*! \ingroup Widgets
 *  Frame like Widget. It may contain many other widgets.
 *  Acts like a hub for any requests.
 */
class USELESS_API FrameWidget : public HubWidget
{
public:
    FrameWidget(int w, int h, const Skin &skin);
    FrameWidget(const FrameStyle &style);
    virtual ~FrameWidget();

    virtual void Resize(int w, int h);
    virtual void RepaintAction(const WidgetPainter &painter);
    virtual Rect GetClientRect( const Widget *sender=NULL ) const;

private:
    bool            _need_init;
    SPointer<Skin>  _sp_skin;

    int             _frame_left;
    int             _frame_top;
    int             _frame_right;
    int             _frame_bottom;
};


/*! \class FrameStyle
 *  designed for FrameWidget
 -------------------------------------------------------------*/
class FrameStyle// : public BorderStyle
{
public:
    FrameStyle(       int  w,
                      int  h,
                const Skin &skin,
                      int  frame_left=0,
                      int  frame_top=0,
                      int  frame_right=0,
                      int  frame_bottom=0
              ): 
                 
                 //BorderStyle(w, h, skin),
                 w(w), h(h), skin(skin),
                 frame_left(frame_left),
                 frame_top(frame_top),
                 frame_right(frame_right),
                 frame_bottom(frame_bottom)
                 {}
  
    int w;
    int h;
    const Skin &skin;
    int frame_left;
    int frame_top;
    int frame_right;
    int frame_bottom;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_FRAME_WIDGET_H__
