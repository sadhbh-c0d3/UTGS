#ifndef __INCLUDED__USELESS_SKIN_MANAGED_WIDGET_H__
#define __INCLUDED__USELESS_SKIN_MANAGED_WIDGET_H__

#include "Useless/GUI/Skins/SkinMan.h"
#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

/*! This widget uses external SkinMan, so if you'd like to use it
 *  REMEMBER: 1) You MUST NOT delete your SkinMan!
 *            2) You MUST delete this widget before removing SkinMan!
 */
class USELESS_API SkinManagedWidget : public Widget
{
public:
    SkinManagedWidget( int skin_no, SkinMan &skin_manager )
                    :_skin_manager(skin_manager), _skin_no(skin_no) 
    {
        SetDimensions( _skin_manager.GetWidth(_skin_no), _skin_manager.GetHeight(_skin_no) );
    }

    virtual ~SkinManagedWidget() {}

    virtual void Repaint(const WidgetPainter &painter) { _skin_manager.Paint(_skin_no, painter); }

private:
    SkinMan &_skin_manager;
    int      _skin_no;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_SKIN_MANAGED_WIDGET_H__
