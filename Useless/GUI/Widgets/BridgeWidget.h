#ifndef __INCLUDED_USELESS_BRIDGE_WIDGET_H__
#define __INCLUDED_USELESS_BRIDGE_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/CompoundWidget.h"
#include "Useless/GUI/WidgetInterfaces.h"

namespace Useless {

/*! \ingroup Widgets
 *  Virtual widget, which acts like a bridge for any requests.
 */
class USELESS_API BridgeWidget : virtual public CompoundWidget, public IWBridge
{
public:
    BridgeWidget(int w=0, int h=0, Widget* widget=NULL);
    virtual ~BridgeWidget();

    /* Bridge overloads
    ----------------------------------------------------------------------------------------*/
    virtual void Resize( int w, int h );

    virtual BridgeWidget& Bridge() { return *this; }

    virtual void    AttachWidget( Widget *widget, int x=0, int y=0);
    virtual Widget* DetachWidget();
            Widget* GetAttached () const        { return _widget; }
            void    Hug         ();             //!< Synchronize size and position with child


    template< class T > 
        T* WAttach( T *w, int x=0, int y=0) { AttachWidget(w,x,y); return w; }


    /* Group methods
    ----------------------------------------------------------------------------------------*/
    virtual    int  ChildrenCount  ()                       const { return (!*_widget)? 0:1; }
    virtual   bool  HasChilds      ()                       const { return !!*_widget; }
    virtual   bool  IsChild        ( const Widget *widget)  const { return !!*_widget && _widget==widget; }

    virtual RefIter FirstChild() const { return new Iterator( _widget) ;}
    virtual RefIter LastChild () const { return new Iterator( _widget) ;}

    class Iterator : public Widget::Iterator
    {
    public:
                      Iterator( Widget *pw) { _widget=pw; }
        virtual      ~Iterator() {}
        virtual void  Next()     { _widget=NULL; }
        virtual void  Previous() { _widget=NULL; }
    };

    /*---------------------------------------------------------------------------------------*/
private:
    Widget *_widget;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_BRIDGE_WIDGET_H__
