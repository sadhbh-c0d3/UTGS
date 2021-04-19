#ifndef __INCLUDED_USELESS_WIDGET_H__
#define __INCLUDED_USELESS_WIDGET_H__

/*
 *    $Id: Widget.h,v 1.44 2004/07/02 18:06:19 koolas Exp $
 *
 *    NAME
 *        Widget
 *
 *    PURPOSE
 *        GUI Primitive
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/
#include "Useless/Graphic/RectList.h"
#include "Useless/Functional/Signal.h"
#include "Useless/GUI/Display/WidgetPainter.h"
#include "Useless/GUI/WidgetInterfaces.h"
#include "Useless/Util/GarbageCollector.h"
#include "Useless/Util/Visitor.h"

namespace Useless {

//USES
    class MouseCursor;
    class WidgetEnvironment;

///////////////////////////////////////////////////////
//  CLASS: Widget
//
/*! \defgroup Widgets Widgets
 *  \ingroup gGUI
 *  Every element of GUI is a \a Widget.
 *  \code
 *  Every Widget must overeload members:
    int GetWidth() cost
    int GetHeight() const
    void Repaint(const WidgetPainter &painter)
 *  \endcode
 *
 */
/*! \ingroup Widgets
 *  Base Widget Interface.
 */
class USELESS_API Widget: virtual public IWidget, public BaseVisitable<>
{
public:
    Widget();
    virtual ~Widget();

    virtual Widget& Atom() { return *this; }
    DEFINE_VISITABLE();
    
    /////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME          | ARGS
    //---------------------------------------------------------------
    mutable Signal_1<bool>           OnVisible;     // Visible
    mutable Signal_2<int,int>        OnIdle;        // T, Dt
    mutable Signal_2<int,int>        OnResize;      // Width, Height
    mutable Signal_1<Pos>            OnReposition;  // Position
    mutable Signal_1<Rect>           OnActorMove;   // actor boudry
    mutable Signal                   OnFocus;       //
    mutable Signal                   OffFocus;      //
    //
    //  NOTE: const Widgets may also send signals
    /////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //  STANDARD Routines
    //
    //! Widget visibility flag
    void Show();
    void Hide();
    void SetOpacity( int opacity ) { _opacity=opacity; SetDirty(); }
    int  GetOpacity() const { return _opacity; }
    bool IsVisible      ()  const { return _is_visible; }
    void SetVisible     (bool t)  { if(t) Show(); else Hide(); }
    //! Check if widget is visible in Workspace
    bool IsRealyVisible ()  const;
    //! On-top flag. Each Show will execute _parent->BringOnTop(this);
    void SetOnTop       (bool t=true);
    bool IsOnTop        ()  const { return _is_on_top; }
    //! Ask if position is inside widget area. Used by ActiveWidget to determine motion events
    virtual bool IsOver( const Pos& pos ) const { return GetBoundingRect().IsInside(pos.x, pos.y); }

    //! When phantom attribute is set, widget does not shadow input.
    void SetPhantom( bool t ) { _is_phantom = t; }
    bool IsPhantom() const { return _is_phantom; }
    bool IsOverAndNotPhantom( const Pos &pos ) const { return (IsOver(pos) && !IsPhantom()); }

    //! Render widget using provided painter. Only Workspace (or parent) should call this method.
    virtual void Repaint  ( const WidgetPainter &painter ) = 0;
    //! Resize widget. By default widget cannot resize.
    virtual void Resize         ( int w, int h ) {}
    /*! Client area is defined as:
     *  a) sender = this  : part of bounding rect consummed by this widget
     *  b) IsChild(sender): place assigned for use by given child
     *  c) sender = NULL  : as if sender = this
     */
    virtual Rect GetClientRect  ( Widget const* sender=NULL ) const { return _bound-GetPosition(); }
    //! Position of this widget in parent coordinate space
    void SetPosition    ( const Pos &pos ) { SetBoundingRect( Rect(pos.x,pos.y,_bound.GetW(),_bound.GetH()) ); }
    Pos  GetPosition    () const { return _bound.GetPos(); }
    //! Allocate space for this widget in parent coordinate space
    void SetBoundingRect( const Rect &r );
    void SetDimensions  ( int w, int h ) { SetBoundingRect( Rect(_bound.GetX(),_bound.GetY(),(w>0)?w:0,(h>0)?h:0) ); }
    //! Get boundary information. In widget coordinate space
    Rect GetBoundingRect () const { return Rect( 0, 0, GetWidth(), GetHeight() ); }
    virtual int GetWidth () const { return _bound.GetW(); }
    virtual int GetHeight() const { return _bound.GetH(); }
    
    //! Few helper methods
    //@{
    enum OuterAlignment // Used by AlignTo():
    {
        LEFT_BORDER=1,  // stitches its own right border with widgets left border
        TOP_BORDER=2,   // stitches its own bottom border this widgets top border
        RIGHT_BORDER=3,
        BOTTOM_BORDER=4,
        LU_CORNER=1<<8, // if used with {LEFT|RIGHT}_BORDER stitch upper corners
        RB_CORNER=2<<8  // if used with {LEFT|RIGHT}_BORDER stitch bottom corners
    };
    
    //! Align self to outer boundary of widget.
    void AlignTo        ( Widget const* widget, int outer_alignment );
    //! Align self inside parent-client area.
    void Align          ( int inner_alignment);
    //! Place widget relatively in parent-client area.
    void Place          ( const Pos &pos, const Pos &base);
    //! Place and resize widget relatively in parent-client area.
    void Place          ( const Rect &rect, const Pos &base);
    //! Make self fit whole parent-client area.
    void Fill           ( const Rect &area=Rect(), const Rect &padding=Rect() );
    //! Relatively move self.
    void Move           ( const Pos &dpos );
    //! Relatively resize self.
    void Enlarge        ( const Pos &dsize );
    //! Relatively adjust z-index.
    void Dive           ( int dz );
    //! Resize widget to fit alignment constrains.
	void AlignSize      ( const Rect &offsetAndSize, const Pos &placement = Pos(0,0));
    //@}

    ////////////////////////////////////////////////////////////////////
    //  INPUT Handling
    //
    enum Conditions { NORMAL=0, SHADOWED=1, INACTIVE=2, OFFSIDE=3, FOCUSED=4 };
    
    //! Manage recursive mutexes for pointer motion events (mouse and buttons)
    //@{
    virtual bool InputIsEager   () const { return _input_eager > 0; }
    virtual void InputIncEager  (Widget *sender=NULL);
    virtual void InputDecEager  (Widget *sender=NULL);
    virtual void InputIntercept (Widget *sender=NULL);
    virtual void InputRelease   (Widget *sender=NULL);
    virtual bool HasInput       () const { return _input_intercept > 0; }
    //@}

    /*! Methods are shortcuts to p_environment->GetForm().SetFocussed(this)
     *  It means environment must be set before using them */
    //@{
    void FocusIntercept ();
    void FocusRelease   ();
    //@}
    //! Tells wether this widget has focus inside its Form.
    bool HasFocus       () const;
    //! Tells wether widget is ancestor of focussed widget
    bool OnFocusWay     () const;

    //! Reset any input flags, and tell parent Form to release this focus
    void DoInputReset   (Widget *sender=NULL);

    ////////////////////////////////////////////////////////////////////
    //  DIRTY REGION Handling
    //
    //! set dirty part of a widget
    void SetDirty  ( const Rect &dirty );
    //! set dirty whole widget
    void SetDirty  () { SetDirty( GetBoundingRect() ); }
    
    void BeAlwaysDirty( bool t) { _always_dirty = t; }
    bool IsAlwaysDirty() const { return _always_dirty;}

    //! calcualtes dirty region according to 'area_before_changes' and current area
    void SetExposed( const Rect &area_before_changes );

    //! dirty recursive-lock. adds 'op' to '_dirty_lock'
    int  LockDirtyOp( int op ) { return _dirty_lock += op; }

    ////////////////////////////////////////////////////////////////////
    //  PARENT Assigning
    //
    Widget*      GetParent       () const                      { return _parent; }
    void         SetParent       (Widget* some_widget);

    virtual void BringOnTop      (Widget *some_widget) {}
    virtual void Lift            (int num_layers, Widget *some_widget) {}

    virtual void Setup           ();
    virtual void Cleanup         ();

    virtual  Pos GetAbsolutePos  (const Pos  &pos ) const;
    virtual  Pos GetRelativePos  (const Pos  &pos ) const;
    virtual Rect GetAbsoluteRect (const Rect &rect) const;
    virtual Rect GetRelativeRect (const Rect &rect) const;

    ////////////////////////////////////////////////////////////////////
    //  Group Functions
    //
    virtual bool IsChild      (const Widget *widget) const { return false; }
    virtual bool HasChilds    ()                     const { return false; }
    virtual  int ChildrenCount()                     const { return 0;     }

    class Iterator;
    class RefIter;

    virtual RefIter FirstChild() const;
    virtual RefIter LastChild () const;

    template< class FObj > void ForEach   ( const FObj & );
    template< class FObj > void ForEachRev( const FObj & );


    ////////////////////////////////////////////////////////////////////
    //  MISC Functions
    //
    virtual      void  SetEnvironment( const WidgetEnvironment *enviro);
    const WidgetEnvironment* GetEnvironment() const { return _p_environment; }

                 void  SetMouseCursor     (MouseCursor &mc) const;
                 void  RestoreMouseCursor () const;
    const MouseCursor &GetMouseCursor() const;

    /*! Notify ancestors about this widget existence.
     *  Notification includes parent Form and Workspace.
     *  Hidden widget is considered as not existing.
     */
    void NotifyAncestors( bool I_exist );

    virtual int GetActivationKey() const { return -1; } // a key used to activate widget

    template < class Ptr > void Posses( const Ptr &ptr) { _personal_trash_can.Trash(ptr); }

    virtual      bool operator !() const { return false; } // Tell if widget is NOT NullWidget

    ////////////////////////////////////////////////////////////////////
    //  Event Handling  (defined in Useless/GUI/Widgets/WidgetRequest.h)
    //
                      class     Request;
                      class     InputCursor;
                      class     InputButton;
                      class     DoubleClick;
                      class     InputWheel;
                      class     InputReset;
                      class     InputKeyboard;
                      class     InputChar;
                      class     InputFocus;
                      class     InputAccel;

    virtual bool Accept(const Request& request);

protected:
    ////////////////////////////////////////////////////////////////////
    //                  Widget properties                             
    //                                                                
               virtual void      Attach();
               virtual void      Detach();

    int   _input_eager;       // counts times the widget called Input{Inc/Dec}Eager().
                             // when '0' then no input is given to this widget.
    int   _input_intercept; // spin-lock for pointer input interception
                           // when '0' then this widget has not intercepted input
    int   _dirty_lock;    // spin-lock disables SetDirty, derivates has free access to it.
    
    bool  _always_dirty; // Causes widget be dirty forever (set true for often changing widgets)

    // Helper class for locking dirty   
    struct DirtyLock
    {
        DirtyLock( Widget *p ): p(p) { p->LockDirtyOp(1); }
        ~DirtyLock() { p->LockDirtyOp(-1); }
        Widget *p;
    };
    
private:
    ////////////////////////////////////////////////////////////////////
    //                  Widget properties                             
    //                                                                
                       bool     _is_visible;
                       bool     _is_on_top;
                       bool     _is_phantom;
                        int     _opacity;
                       Rect     _bound;
                     Widget    *_parent;
    const WidgetEnvironment    *_p_environment;
           GarbageCollector     _personal_trash_can;


    Widget(const Widget&); // PREVENT COPYING WIDGETS

    Widget(int);           // INIT NULL WIDGET
    friend class NullWidget;

    ////////////////////////////////////////////////////////////////////
    // Section for debugging
    //
    static int _widget_last_id;
           int _this_widget_id;
    friend class DebugWidgetEventLogger;
};

};// namespace Useless

#include "Useless/GUI/Widgets/WidgetRequest.h"
#include "Useless/GUI/Widgets/WidgetIterator.h"

#endif// __INCLUDED_USELESS_WIDGET_H__
