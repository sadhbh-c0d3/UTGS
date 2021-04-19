#ifndef __INCLUDED_USELESS_WORKSPACE_H__
#define __INCLUDED_USELESS_WORKSPACE_H__

#pragma warning(disable:4250) // inherit via dominance
#pragma warning(disable:4786) // stl turncated names
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Form.h"
#include "Useless/GUI/Widgets/FormWidget.h"
#include "Useless/GUI/PopupCycler.h"
#include <list>

namespace Useless{

// REQUIRE:
    class GUIMaster;
    class ScreenMan;

///////////////////////////////////////
//  CLASS: Workspace
//
/*! \ingroup gGUI 
 *  Workspace is main screen area.
 *  The class manages widgets, poups, menus and hints.
 */
class USELESS_API Workspace : public Form, public PopupCycler
{
public:
    Workspace( GUIMaster &gui );
    ~Workspace();

    /*! Use those methods to manage widgets */
    void AddToDesktop ( Widget *widget, const Pos &p = Pos() );
    void AddPopup     ( Popup  *popup,  const Pos &p = Pos() );
    void AddMenu      ( Widget *menu_widget, const Pos &p = Pos() );
    void AddHint      ( Widget *hint_widget, const Pos &p = Pos() );

    void    Remove    ( Widget *widget );
    Widget* Retrieve  ( Widget *widget );

    Rect GetWorkspaceRect() const;

    void ShowAtPointer ( Widget *any_widget );
    void ShowAtPosition( const Pos &p, Widget *any_widget );
     Pos WhereIsPointer() const;

                 void  SetPreviousCursor();
                 void  SetCursor( const MouseCursor &mc);
    const MouseCursor &GetCursor() const;
                 void  ShowCursor();
                 void  HideCursor();
                 bool  IsCursorShown() const;

    enum  { NO_LAYER = 0,
            DESKTOP_LAYER,
            POPUPS_LAYER,
            MENUS_LAYER,
            HINTS_LAYER };

      int InWhichLayer( Widget *widget ) const;

    /////////////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME                      | ARGS
    //-----------------------------------------------------------------------
    mutable Signal                    OnHideHints;               // none
    mutable Signal                    OnHideMenus;               // none


/////////////////////////////////////////////////////////////////////////////
private:
    typedef TemplateWidget< HubWidget, FormWidget > FormHub;

    FormHub     _desktop_layer;
    FormHub     _popups_layer;
    HubWidget   _menus_layer;
    HubWidget   _hints_layer;

    /*! Methods called by ScreenMan */
    void  Repaint();
    void  AcceptDirtyDesktop( const Rect &dirty );
    void  AcceptDirtyPopups ( const Rect &dirty );
    void  AcceptDirtyMenus  ( const Rect &dirty );
    void  AcceptDirtyHints  ( const Rect &dirty );
    enum  { DESKTOP=1, POPUPS=2, MENUS=4, HINTS=8, SCREEN=16 };
    void  ResendDirty       ( const Rect &dirty, int flags );
    void  Resize            ( int w, int h );

    void  AcceptInputCursor ( const Pos &pos, const Pos &dpos );
    void  AcceptInputButton ( int status, int changed );
    void  AcceptDoubleClick ( int button_no );
    void  AcceptInputWheel  ( int zpos, int dz );
    void  AcceptInputChar   ( int code );
    void  AcceptInputKey    ( int key, bool pressed );
    void  AcceptIdleCalls   ( int T, int Dt );

    bool  ResendRequest( const Widget::Request &request, const Widget::Request &alt_request, int flags );
    void  RestoreFocus();

    void  HideMenus();
    void  HideHints();
    
  
    unsigned long  _input_intercepting_layer;
    unsigned long  _focus_holding_layer;
    bool  _dirty_lock;
    bool  _desktop_accept_status;
    bool  _popups_accept_status;
    bool  _menus_accept_status;
    bool  _hints_accept_status;
    bool  _refresh_pointer_accepts;

    BitVector _pressed_keys_map;

    friend class ScreenMan;
    friend class GUIMaster;

    GUIMaster &_gui;
    GUIMaster &GetGUI() { return _gui; }
    void SetEnvironment( const WidgetEnvironment *penv );

public:
    void  NoticeInputIntercept( bool t, Widget *sender );
    void  NoticeWidgetMovement( Widget *sender );
    void  SetDirty            ( const Rect &dirty );
    bool  AnyVisibleHints     ();
    bool  AnyVisibleMenus     ();
    bool  AnyVisiblePopups    ();
    bool  IsKeyPressed        ( int key_code ) const;

    FormHub* GetDesktop() { return &_desktop_layer; }
    HubWidget* GetMenus() { return &_menus_layer; }
    HubWidget* GetHints() { return &_hints_layer; }

    int GetWidth() const;
    int GetHeight() const;


/* Form overloads
---------------------------------------*/
    virtual void    SetFocussed ( Widget *widget );
    virtual void InvalidateFocus( Widget *sender );
    virtual Widget* GetFocussed () const;

private:
    Widget *_p_focussed;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_WORKSPACE_H__
