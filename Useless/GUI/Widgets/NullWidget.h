#ifndef __INCLUDED_USELESS_NULL_WIDGET_H__
#define __INCLUDED_USELESS_NULL_WIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/Util/Singleton.h"

namespace Useless {

/*! \ingroup Widgets
 *  Completely Empty Widget.
 */
class USELESS_API NullWidget : public Widget
{
public:
    NullWidget() : Widget(0) { Widget::_parent=this; }
    virtual ~NullWidget() {}

    virtual bool IsOver         (const Pos& pos) const         { return false; }
    virtual void Repaint        (const WidgetPainter &painter) {}
    virtual void Resize         (int w, int h) {}
    
    virtual void Fill           (const Rect& rect=Rect()) {}
   
    virtual bool InputIsEager   ()                     const { return false; }
    virtual void InputIncEager  (Widget *sender=NULL) {}
    virtual void InputDecEager  (Widget *sender=NULL) {}
    virtual void InputIntercept (Widget *sender=NULL) {}
    virtual void InputRelease   (Widget *sender=NULL) {}
    virtual bool HasFocus       ()                     const { return false; }
    virtual void FocusIntercept (Widget *sender=NULL) {}
    virtual void FocusRelease   (Widget *sender=NULL) {}
            void DoInputReset   (Widget *sender=NULL) {}
    virtual void SetPosition    ( const Pos &pos  ) {}
    virtual  Pos GetAbsolutePos ( const Pos  &pos ) const { return pos; }
    virtual  Pos GetRelativePos ( const Pos  &pos ) const { return pos; }
    virtual Rect GetAbsoluteRect( const Rect &rect) const { return rect; }
    virtual Rect GetRelativeRect( const Rect &rect) const { return rect; }

    virtual      void  SetEnvironment( const WidgetEnvironment *enviro) {}

    virtual bool Accept(const Request& request) { return false; }
    
    virtual bool operator !() const { return true; }

    static NullWidget* InstancePtr();
};

DECLARE_SINGLETON(NullWidget, NullWidgetInstance, USELESS_API);

inline NullWidget* NullWidget::InstancePtr() { return &NullWidgetInstance::Instance(); }


};//namespace Useless
#endif//__INCLUDED_USELESS_NULL_WIDGET_H__
