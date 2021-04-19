#ifndef __INCLUDED_WINDOW_H__
#define __INCLUDED_WINDOW_H__

/*
 * $Id: Window.h,v 1.12 2004/01/27 20:45:41 koolas Exp $
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

#include <windows.h>
#include "Application.h"
#include "Useless/Functional/Signal.h"
#include "Useless/Graphic/Rect.h"

namespace Useless {

/*! \ingroup gApplication
 *
 *  Generalized Win32 Window
 */
class USELESS_API Window
{
public:
    Window(const char *title="IAV Game");
    Window(const char *title, Window *parent, int width, int height, int pos_x, int pos_y) throw();
    virtual ~Window();

    void SetParent( Window *parent );
    Window *GetParent() const;
    void Refresh() { UpdateWindow(_window_handle); }
    void Show() { ::ShowWindow(_window_handle, SW_SHOWDEFAULT); Refresh(); }
    void Hide() { ::ShowWindow(_window_handle, SW_HIDE); }
    void Maximize() { ::ShowWindow(_window_handle, SW_SHOWMAXIMIZED); }
    void Minimize() { ::ShowWindow(_window_handle, SW_SHOWMINIMIZED); }
    void Restore()  { ::ShowWindow(_window_handle, SW_RESTORE); }
    void ShowCursor() { Application::ShowCursor(); }
    void HideCursor() { Application::HideCursor(); }
    void GrabFocus() { ::BringWindowToTop(_window_handle); }
    void InterceptCursor() { ::SetCapture(_window_handle); }
    void ReleaseCursor()   { ::ReleaseCapture(); }
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

    void AllowCursorWrapping(bool t=true);
    bool NoCursorWrapping() const { return !_allow_cursor_wrapping; }

    /////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME          | ARGS
    //---------------------------------------------------------------
    mutable Signal                    OnCreate;      // -none-
    mutable Signal                    OnDestroy;     // -none-
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
    // Low level W32 only
    void OverrideHandler( Application::MessageHandler handler) 
    { 
        Application::RegisterHandler(_window_handle, handler); 
    }

//protected: - allow exploiting
    HWND GetHandle() const
    {
        return _window_handle;
    }
    
    void SetHandle( HWND hwnd)
    {
        _window_handle = hwnd;
        Application::RegisterWindow( _window_handle, this);
    }
    
    HWND ReleaseHandle()
    {
        HWND hwnd = _window_handle;
        _window_handle = 0;
        Application::UnregisterWindow( hwnd);
        DestroyWindow( hwnd);
        return hwnd;
    }

    std::string GetTitle() const { return _title; }
    
private:
    HWND _window_handle;
    std::string _title;

    bool _allow_sizing;
    bool _allow_clear;
    bool _allow_cursor_wrapping;
    
    friend class Application;
    friend class WindowVisitor;
};


inline void Window::SetCursorPos(int x, int y)
{
    Rect  w = GetWindowRect() ;
    Rect  c = GetClientRect() ;
    int frm  = ( w.GetW()-c.GetW() )/2;
    int x0 = w.GetX1() + frm;
    int y0 = w.GetY2() - (c.GetH()+frm);
    ::SetCursorPos( x0+x, y0+y);
}

inline Pos  Window::GetCursorPos() const
{ 
    Point<long> cp; ::GetCursorPos( (LPPOINT)&cp );

    Rect  w = GetWindowRect() ;
    Rect  c = GetClientRect() ;
    int frm  = ( w.GetW()-c.GetW() )/2;
    int x0 = w.GetX1() + frm;
    int y0 = w.GetY2() - (c.GetH()+frm);

    return cp-Pos(x0,y0); 
}

};// namespace Useless

#endif __INCLUDED_WINDOW_H__
