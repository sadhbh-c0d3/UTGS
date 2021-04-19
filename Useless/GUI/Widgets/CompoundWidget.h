#ifndef __INCLUDED_USELESS_COMPOUND_WIDGET_H__
#define __INCLUDED_USELESS_COMPOUND_WIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/Functional/MethodPool.h"
#include "Useless/Util/PreInitializer.h"

namespace Useless {

/*! \ingroup Widgets
 *  Base for any widgets, which may contain any other widgets.
 */
class USELESS_API CompoundWidget: virtual public Widget
{
public:
    CompoundWidget();
    virtual ~CompoundWidget() {}
    
    mutable Signal_1<WidgetPainter>  OnPaint;       // Paiter

    /*! Finds first parent who is CompoundWidget.
     *  child is a widget last visited, whose direct parent is ancestor
     */
    static CompoundWidget* Ancestor( Widget *whose, Widget* &child);

    /*! SafeAncestor will raise exception if failed */
    static CompoundWidget& SafeAncestor( Widget &whose, Widget* &child);

    ///////////////////////////////////////////////////////////////////////
    //  USER METHODS TO RELOAD
    virtual   bool  IsChild      (const Widget *widget) const = 0;
    virtual   bool  HasChilds    ()                     const = 0;
    virtual    int  ChildrenCount()                     const = 0;

    virtual RefIter  FirstChild() const = 0;
    virtual RefIter  LastChild () const = 0;

    ///////////////////////////////////////////////////////////////////////
    //  OPTIONAL OVERLOADS
    virtual void RepaintAction(const WidgetPainter &painter) {}
    virtual bool DispatchIdle ( int t, int dt);

    //////////////////////////////////////////////////////////////////////
    //  INTERNALLY RELOADED METHODS
    virtual void Repaint        (const WidgetPainter& painter );
            void DoRepaint      (const WidgetPainter &painter);
    virtual bool Accept         (const Request& request);
            void GetAcceptStatus(bool *pointer_was_over_widget, bool *widget_accepted_request);

    virtual void InputIntercept(Widget *sender=NULL);
    virtual void InputRelease  (Widget *sender=NULL);

    virtual bool IsFullSolid   () const;
    virtual void GetSolidArea  ( RectList &rects );

    virtual void SetEnvironment( const WidgetEnvironment *enviro);
    virtual const WidgetEnvironment* GetChildrenEnvironment() const { return GetEnvironment(); }

protected:
    bool        _handled;
    bool        _was_top;
    Pos         _cursor;
    Widget     *_input_interceptor;
    
private:
    void IterSetEnvironment ( Widget *who );
    void IterRepaint        ( Widget *who, const WidgetPainter* p_painter );
    void IterIdleCalls      ( Widget *who, int t, int dt );

    void IterAccept        ( Widget *who, Request const* p_request);
    void IterInputCursor   ( const InputCursor &input);
    void IterInputButton   ( const InputButton &input);
    void IterDoubleClick   ( const DoubleClick &input);
    void IterInputWheel    ( const InputWheel  &input);
    void IterInputKeyboard ( const InputKeyboard &input);
    void IterInputChar     ( const InputChar &input);

     int GetCondition        ( Widget *who, int previous_cond);

    static MethodPool<CompoundWidget, const Widget::Request > *_handlers;
    static void  Init();
    static void  Destroy();
    friend class PreInitializer<CompoundWidget>;

    mutable Widget *_last_who;

};

DECLARE_PREINITIALIZER( CompoundWidget, CompoundWidget, USELESS_API );

};//namespace Useless

#endif//__INCLUDED_USELESS_COMPOUND_WIDGET_H__
