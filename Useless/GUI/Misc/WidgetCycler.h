#ifndef __INCLUDED__USELESS_WIDGET_CYCLER_H__
#define __INCLUDED__USELESS_WIDGET_CYCLER_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/Util/Storage/StupidVector.h"

namespace Useless {

class USELESS_API WidgetCycler
{
public:
    WidgetCycler( Widget *root, WidgetCycler *parent_cycler=NULL );
    ~WidgetCycler();

    bool   CycleForward();
    bool   CycleBack();
    bool   Set(Widget*);

    Widget* Who()   const;
    int     Level() const;
    bool    IsValid() const;

private:
    Widget                *_root;
    WidgetCycler          *_parent_cycler;

    typedef std::pair< Widget*, Widget::RefIter > IteratorInsideWidget;
    typedef StupidVector< IteratorInsideWidget > CycleList;
    CycleList _iterator_stack;

    Widget*          GetContainer() const;
    Widget::RefIter  GetIterator () const;
    
    void  PushIterator( Widget::RefIter ref_it );
    bool  RestoreIterator();

/* Debuging information
------------------------------------*/
#ifdef NDEBUG
    void UpdateDebugInfo() const {}
#else
    int     _level;
    Widget *_widget;
    Widget *_container;

    void UpdateDebugInfo()
    {
        _level = Level();
        _widget = Who();
        _container = GetContainer();
    }
    bool  CycleForwardImpl();
    bool  CycleBackImpl();
    bool  SetImpl(Widget*);
#endif
};


/* Inlines - for easy debiging
--------------------------------------------------------------------------------*/

inline Widget* WidgetCycler::Who() const 
{
    Widget *ptr = GetIterator().Who();
    return ptr;
}
inline int WidgetCycler::Level() const 
{
    int level = _iterator_stack.Size()-1;
    return level;
}

inline Widget* WidgetCycler::GetContainer() const 
{
    Widget *ptr = _iterator_stack.Last().Value().first;
    return ptr;
}

inline Widget::RefIter  WidgetCycler::GetIterator() const 
{
    const Widget::RefIter &rit = _iterator_stack.Last().Value().second;
    return rit;
}

inline void WidgetCycler::PushIterator( Widget::RefIter ref_it )
{
    _iterator_stack.Last().Value().second = ref_it; 
}

};//namespace Useless
#endif//__INCLUDED__USELESS_WIDGET_CYCLER_H__
