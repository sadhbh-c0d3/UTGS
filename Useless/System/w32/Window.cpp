#include "UselessPch.h"

#include "Window.h"
#include "ErrorDialog.h"
#include <string>
#include <algorithm>

namespace Useless {

Window::Window(const char *title)
{
    _title = title;

    TCHAR_string titleTC( ToTCHAR( title));
    
    _window_handle = ::CreateWindowEx(0,Application::GetClass(), titleTC.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Application::GetInstance(), NULL);

    if (!_window_handle) { ErrorDialog(); }

    AllowSizing(true);
    AllowClear(true);
    _allow_cursor_wrapping=false;

    Application::RegisterWindow( _window_handle, this );
}

Window::Window(const char * title, Window* parent, int width, int height, int pos_x, int pos_y)
{
    _title = title;

    TCHAR_string titleTC( ToTCHAR(title));
    
    _window_handle = ::CreateWindowEx(0,Application::GetClass(), titleTC.c_str(), WS_OVERLAPPEDWINDOW,
        pos_x, pos_y, width, height, (parent)? parent->_window_handle : NULL,
        NULL, Application::GetInstance(), NULL);

    if (!_window_handle) { ErrorDialog(); }

    AllowSizing(true);
    AllowClear(true);
    _allow_cursor_wrapping=false;
    Show();

    Application::RegisterWindow( _window_handle, this );
}

Window::~Window()
{
    DestroyWindow( _window_handle );
}

Window* Window::GetParent() const
{
    HWND parent_handle = ::GetParent(_window_handle);
    return Application::GetWindowPtr(parent_handle);
}

void Window::SetParent( Window *parent )
{
    ::SetParent( _window_handle, parent->_window_handle );
}

Useless::Rect Window::GetWindowRect() const
{
    RECT rect;
    ::GetWindowRect(_window_handle, &rect);
    return Useless::Rect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top );
}

Useless::Rect Window::GetClientRect() const
{
    RECT rect;
    ::GetClientRect(_window_handle, &rect);
    return Useless::Rect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top );
}

void Window::Reposition(int x, int y)
{
    Useless::Rect wr = GetWindowRect();
    ::MoveWindow( _window_handle, x, y, wr.GetW(), wr.GetH(), true );
}

void Window::Resize(int w, int h)
{
    Useless::Rect wr = GetWindowRect();
    ::MoveWindow( _window_handle, wr.GetX(), wr.GetY(), w ,h, true );
}

void Window::ShowDecorations()
{
    Hide();
    Rect clr = GetClientRect();   
    SetWindowLong( _window_handle, GWL_STYLE, WS_TILEDWINDOW);
    Rect dr = GetWindowRect() - GetClientRect();   
    Resize( clr.GetW() + dr.GetW(), clr.GetH() + dr.GetH() );
    Show();
}

void Window::HideDecorations()
{
    Hide();
    Rect clr = GetClientRect();
    SetWindowLong( _window_handle, GWL_STYLE, WS_POPUP);
    Resize( clr.GetW(), clr.GetH());
    Show();
}
    
void Window::AllowCursorWrapping(bool t)
{
    if ( t /*&& !_allow_cursor_wrapping*/ )
    {
        InterceptCursor();
    }
    else if ( !t /*&& _allow_cursor_wrapping*/ )
    {
        ReleaseCursor();
    }
    
    _allow_cursor_wrapping=t;
}

};// namespace Useless
