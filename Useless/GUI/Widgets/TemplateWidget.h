#ifndef __INCLUDED_USELESS_TEMPLATE_WIDGET_H__
#define __INCLUDED_USELESS_TEMPLATE_WIDGET_H__

// Do not uncomment the line below.
// If needed copy this pragma to file where you use this template. 
//#pragma warning(disable:4250) // inherit via dominance

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/HubWidget.h"


namespace Useless {

/*! \ingroup Widgets
 *  TemplateWidget allows you to combine two widget classes together.
 *  If you need more widget classes to combine you can cascade TemplateWidget's.
 *  Under msvc cascading does NOT work.
 */
template < class BasicWidgetType, class WidgetExpansionType >

class TemplateWidget : public BasicWidgetType, virtual public WidgetExpansionType
{
public:
    // There are some problems w/ default ctors when using DLL.
    // I assume there is no widget w/o parameters in ctor.
    //USELESS_EMPTY_SPECIALIZATION
    //    TemplateWidget() {}
    template< class A1 >
        TemplateWidget( const A1 &a1 ): BasicWidgetType( a1 ) {}
    template< class A1, class A2 >
        TemplateWidget( const A1 &a1, const A2 &a2 ): BasicWidgetType(a1,a2) {}
    template< class A1, class A2, class A3 >
        TemplateWidget( const A1 &a1, const A2 &a2, const A3 &a3 ): BasicWidgetType(a1,a2,a3) {}
    template< class A1, class A2, class A3, class A4 >
        TemplateWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4 ): BasicWidgetType(a1,a2,a3,a4) {}
    template< class A1, class A2, class A3, class A4, class A5 >
        TemplateWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5 ): 
            BasicWidgetType(a1,a2,a3,a4,a5) {}
    template< class A1, class A2, class A3, class A4, class A5, class A6 >
        TemplateWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6 ): 
            BasicWidgetType(a1,a2,a3,a4,a5,a6) {}
    template< class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
        TemplateWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, 
                        const A5 &a5, const A6 &a6, const A7 &a7 ): 
            BasicWidgetType(a1,a2,a3,a4,a5,a6,a7) {}
    template< class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
        TemplateWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, 
                        const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8 ): 
            BasicWidgetType(a1,a2,a3,a4,a5,a6,a7,a8) {}

    virtual ~TemplateWidget() {}
};


namespace UselessImp {
// This will increase dominance distance of __ActiveWidget from Widget
class __ActiveWidget_0__ : public virtual   ActiveWidget     {};
class __ActiveWidget_1__ : public virtual __ActiveWidget_0__ {};
class __ActiveWidget_2__ : public virtual __ActiveWidget_1__ {};
class __ActiveWidget_3__ : public virtual __ActiveWidget_2__ {};
class __ActiveWidget     : public virtual __ActiveWidget_3__ {};
};// namespace UselessImp

/*! \ingroup Widgets
 *  ActivatedWidget converts inactive widget class to active one.
 *  In straight words this class makes widget sensitive for mouse & keyboard input.
 *  If 'OR' is 'true' then Accept will evaluate for InactiveWidgetType or ActiveWidget
 *  else it will allways evaluate to both of them.
 */
template < class InactiveWidgetType, const bool OR=true >

class ActivatedWidget : public InactiveWidgetType, public virtual ActiveWidget
{
public:
    //USELESS_EMPTY_SPECIALIZATION
    //    ActivatedWidget() {}

    template< class A1 >
        ActivatedWidget( const A1 &a1 ): InactiveWidgetType( a1 ) {}

    template< class A1, class A2 >
        ActivatedWidget( const A1 &a1, const A2 &a2 ): InactiveWidgetType(a1,a2) {}

    template< class A1, class A2, class A3 >
        ActivatedWidget( const A1 &a1, const A2 &a2, const A3 &a3 ): InactiveWidgetType(a1,a2,a3) {}

    template< class A1, class A2, class A3, class A4 >
        ActivatedWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4 ): InactiveWidgetType(a1,a2,a3,a4) {}

    virtual ~ActivatedWidget() {}

    virtual bool Accept(const Useless::Widget::Request& request)
    {
        if (OR) { if ( ! InactiveWidgetType::Accept(request) ) if ( ! ActiveWidget::Accept(request) ) return false; }
        else    { bool ok=InactiveWidgetType::Accept(request); ok|=ActiveWidget::Accept(request); return ok; }
        return true;
    }
};


/*! \ingroup Widgets
 *  ParentWidget converts non-compound widget class to compound one.
 *  In straight words this class gives widget ability to have childs.
 */
template < class WidgetType, const bool IS_ACTIVE=false >

class ParentWidget : public WidgetType, public virtual HubWidget
{
public:
    //USELESS_EMPTY_SPECIALIZATION
    //    ParentWidget() {}

    template< class A1 >
        ParentWidget( const A1 &a1 ): WidgetType( a1 ) {}

    template< class A1, class A2 >
        ParentWidget( const A1 &a1, const A2 &a2 ): WidgetType(a1,a2) {}

    template< class A1, class A2, class A3 >
        ParentWidget( const A1 &a1, const A2 &a2, const A3 &a3 ): WidgetType(a1,a2,a3) {}

    template< class A1, class A2, class A3, class A4 >
        ParentWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4 ): WidgetType(a1,a2,a3,a4) {}

    template< class A1, class A2, class A3, class A4, class A5 >
        ParentWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5 ):
            WidgetType(a1,a2,a3,a4,a5) {}

    template< class A1, class A2, class A3, class A4, class A5, class A6 >
        ParentWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6 ):
            WidgetType(a1,a2,a3,a4,a5,a6) {}

    virtual ~ParentWidget() {}

    virtual bool Accept(const Useless::Widget::Request& request)
    {
        bool rval=false;
        if (IS_ACTIVE) { rval|=WidgetType::Accept(request); }
        rval|=HubWidget::Accept(request);
        return rval;
    }

    virtual void Repaint(const Useless::WidgetPainter &painter)
    {
        WidgetType::Repaint(painter); HubWidget::Repaint(painter);
    }

    virtual void Resize (int w, int h)
    {
        WidgetType::Resize(w,h); HubWidget::Resize( GetWidth(), GetHeight() );
    }

    DEFINE_CYCLIC_VISITABLE(HubWidget);
};


/*! \ingroup Widgets
 *  ActivatedWidget converts inactive widget class to active one.
 *  In straight words this class makes widget sensitive for mouse & keyboard input.
 */
template < class WidgetType >

class PaintedWidget : public WidgetType
{
public:
    //USELESS_EMPTY_SPECIALIZATION
    //   PaintedWidget() {}

    template< class A1 >
        PaintedWidget( const A1 &a1 ): WidgetType( a1 ) {}

    template< class A1, class A2 >
        PaintedWidget( const A1 &a1, const A2 &a2 ): WidgetType(a1,a2) {}

    template< class A1, class A2, class A3 >
        PaintedWidget( const A1 &a1, const A2 &a2, const A3 &a3 ): WidgetType(a1,a2,a3) {}

    template< class A1, class A2, class A3, class A4 >
        PaintedWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4 ): WidgetType(a1,a2,a3,a4) {}

    template< class A1, class A2, class A3, class A4, class A5 >
        PaintedWidget( const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5 ): WidgetType(a1,a2,a3,a4,a5) {}

    virtual ~PaintedWidget() {}

    virtual bool IsOver(const Useless::Pos &pos) const { return false; }

};


/*! \ingroup Widgets
 *  MergedWidget merges two types of widgets together into one type.
 *  If 'OR' is 'true' then Accept will evaluate for WidgetA or WidgetB
 *  else it will allways evaluate to both of them.
 */
template< class WidgetA, class WidgetB, const bool OR=true >

class MergedWidget : virtual public WidgetA, virtual public WidgetB
{
public:
    //USELESS_SPECIALIZATION
    //    MergedWidget() {}
    template< class A1 >
        MergedWidget( const A1 &a1 ): WidgetA( a1 ), WidgetB( a1 ) {}
    template< class A1, class A2 >
        MergedWidget( const A1 &a1, const A2 &a2 ): WidgetA( a1,a2 ), WidgetB( a1,a2 ) {}

    virtual ~MergedWidget() {}

    virtual bool Accept(const Useless::Widget::Request& request)
    {
        if (OR) { if ( ! WidgetA::Accept(request) ) if ( ! WidgetB::Accept(request) ) return false; }
        else    { bool ok=WidgetA::Accept(request); ok|=WidgetB::Accept(request); return ok; }
        return true;
    }
};

};//namespace Useless

#endif//__INCLUDED_USELESS_TEMPLATE_WIDGET_H__
