#ifndef __INCLUDED_USELESS_ICON_WIDGET_H__
#define __INCLUDED_USELESS_ICON_WIDGET_H__

#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/GUI/Logic/LogicWidget.h"

namespace Useless {

struct IconStyle;

/*! \ingroup Widgets
 *  Kind of LogicWidget that paints icon over skin.
 */
class USELESS_API IconWidget : virtual public LogicWidget
{
public:
    IconWidget(  const SkinMan &frames,
                 const SkinMan &icons,
                 int   distance=0 );

    IconWidget( const IconStyle &style );
    virtual ~IconWidget();


     virtual void  Repaint( const WidgetPainter &painter );
     virtual void  Resize(int w, int h);

     virtual  int  GetState () const;
     virtual bool  SetState (int state);
     virtual bool  HasState (int state) const;

/*-------------------------------------------------------------------*/
private:
     SkinSelector       _skins;
     SkinSelector       _icons;
              int       _distance;
};
/*-------------------------------------------------------------------*/


struct IconStyle
{
    IconStyle(  const SkinMan &frames,
                const SkinMan &icons,
                int   distance=0
               ): skins(skins), icons(icons), distance(distance) {}

    const SkinMan &skins;
    const SkinMan &icons;
              int  distance;
};

};//namespace Useless


#endif//__INCLUDED_USELESS_ICON_WIDGET_H__
