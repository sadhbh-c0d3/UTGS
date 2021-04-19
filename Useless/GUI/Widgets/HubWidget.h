#ifndef __INCLUDED_USELESS_HUB_WIDGET_H__
#define __INCLUDED_USELESS_HUB_WIDGET_H__

/*
 *    $Id: HubWidget.h,v 1.18 2003/01/20 22:00:31 koolas Exp $
 *
 *    NAME
 *        HubWidget
 *
 *    PURPOSE
 *        Container of Widgets
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#pragma warning(disable:4250) // inherit via dominance
#pragma warning(disable:4786)
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/GUI/Widgets/CompoundWidget.h"
#include "Useless/GUI/WidgetInterfaces.h"

namespace Useless {

///////////////////////////////////////////////////////
//  CLASS: HubWidget
//
/*! \ingroup Widgets
 *  Virtual widget, which acts like a hub for any requests.
 *  This widget may contain many other widgets.
 */
class USELESS_API HubWidget : virtual public CompoundWidget, public IWHub
{
public:
    HubWidget(int w=0, int h=0);
    virtual ~HubWidget();

    virtual HubWidget& Hub() { return *this; }
    DEFINE_CYCLIC_VISITABLE(Widget);

    /* Hub overloads
    ----------------------------------------------------------------------------------------*/
    bool Insert   ( Widget *widget,int x=0, int y=0, int z=50);
    bool Remove   ( Widget *widget );
    bool Retrieve ( Widget *widget );

    virtual void  Resize          ( int w, int h );

    virtual void  BringOnTop      (Widget *widget);
    virtual void  Lift            (int num_layers, Widget *widget);

            bool  IsOnTop         ( Widget *widget )  const;
             int  GetNearestZ     ()                  const;
          Widget* WhoIsOnTop      ()                  const;
             int  GetChildZIndex  (Widget *widget)    const;
            void  SetChildZIndex  (int z, Widget *widget);


    template< class T > 
        T* WInsert(T* w, int x=0, int y=0, int z=50) { return (Insert(w,x,y,z))? w : NULL; }


    /*---------------------------------------------------------------------------------------*/

private:
    typedef AssocVector< int, Widget* > ZWidgetList;

    ZWidgetList _z_indices;

    void ReleaseAll();


public:
    /* Group methods
    ----------------------------------------------------------------------------------------*/
    virtual    int  ChildrenCount  ()                       const { return   _z_indices.Size();  }
    virtual   bool  HasChilds      ()                       const { return  !_z_indices.Empty(); }
    virtual   bool  IsChild        ( const Widget *widget)  const { return !!_z_indices.FindID((Widget*)widget); }

    virtual RefIter  FirstChild() const { return new Iterator( ((ZWidgetList&)_z_indices).Begin() );}
    virtual RefIter  LastChild () const { return new Iterator( ((ZWidgetList&)_z_indices).End()-1 );}

    class Iterator : public Widget::Iterator
    {
    public:
                      Iterator( const ZWidgetList::Iterator &iterator): _iterator(iterator) { UpdatePtr(); }
        virtual      ~Iterator() {}
        virtual void  Next()     { ++_iterator; UpdatePtr(); }
        virtual void  Previous() { --_iterator; UpdatePtr(); }

    private:
        void UpdatePtr() { _widget = (!_iterator)? NULL : *_iterator; }

        ZWidgetList::Iterator _iterator;
    };
};

};// namespace Useless

#endif// __INCLUDED_USELESS_HUB_WIDGET_H__
