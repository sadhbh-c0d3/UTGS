#ifndef __INCLUDED_WINDOW_H__
#define __INCLUDED_WINDOW_H__

/*
 * $Id: Window.h,v 1.2 2003/01/20 20:54:09 koolas Exp $
 *
 *  NAME
 *      Window (W32 version) 
 *
 *  PURPOSE
 *      Window class for W32
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *
*/ 

#include "Application.h"
#include "Useless/Functional/Signal.h"
#include "Useless/Graphic/Rect.h"

//typedef ::Window X11Window;

namespace Useless {

/*! \ingroup gApplication
 *
 *  Generalized Win32 Window
 */
class Window
{
public:
    Window(const char *title="IAV Game");
    Window(const char *title, Window *parent, int width, int height, int pos_x, int pos_y) throw();
    virtual ~Window();

    Window *GetParent() const;
    void Refresh();
    void Show();
    void Hide();
    void Maximize();
    void Minimize();
    void Restore();
    void ShowCursor();
    void HideCursor();
    void GrabFocus();
    void InterceptCursor();
    void ReleaseCursor(); 
    void SetCursorPos(int x, int y);
    Pos  GetCursorPos() const;
    void Reposition(int x, int y);
    void Resize(int w, int h);
    void ShowDecorations();
    void HideDecorations();

    Rect GetWindowRect() const;
    Rect GetClientRect() const;

    void AllowSizing(bool t=true) { _allow_sizing=t; }
    bool NoSizing() const { return !_allow_sizing; }

    void AllowClear(bool t=true){ _allow_clear=t; }
    bool NoClear() const { return !_allow_clear; }

    /////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME          | ARGS
    //---------------------------------------------------------------
    mutable Signal                    OnActivate;    // -none-
    mutable Signal                    OnDeactivate;  // -none-
    mutable Signal                    OnPaint;       // -none-
    mutable Signal_2<int, int>        OnMove;        // pos. x, pos. y
    mutable Signal_2<int, int>        OnResize;      // width, height
    mutable Signal_2<int, int>        OnIdle;        // t, Dt
    mutable Signal_1<int>             OnKeyPress;    // key code
    mutable Signal_1<int>             OnKeyRelease;  // key code
    mutable Signal_1<int>             OnChar;        // character code
    mutable Signal_2<int, int>        OnMouseBtn;    // status bits, changed bits mask
    mutable Signal_1<int>             OnDoubleClick; // button no
    mutable Signal_2<Pos, Pos>        OnMouseMove;   // pos., D-pos.
    mutable Signal_2<int, int>        OnMouseWheel;  // offset, D-offset
    //
    //  NOTE: const Windows may also send signals
    /////////////////////////////////////////////////////////////////

    //////////////////////
    // Low level X11 only
    //void OverrideHandler( Application::MessageHandler handler) 
    //{ 
    //    Application::RegisterHandler(_window_handle, handler); 
    //}

    //XWindow GetXWindow() const { return _x_window; }
    
    std::string GetTitle() const { return _title; }

private:
    //XWindow _x_window;
    std::string _title;

    bool _allow_sizing;
    bool _allow_clear;

    friend class Application;
    friend class WindowVisitor;
};

};//namespace Useless

#endif __INCLUDED_WINDOW_H__
