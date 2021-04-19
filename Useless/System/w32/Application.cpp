#include "UselessPch.h"

#include "Useless/Error/Quit.h"
#include "Application.h"
#include "ErrorDialog.h"
#include "Window.h"
#include "Useless/Functional/DelayedExecutor.h"
#include <windowsx.h>
#include <zmouse.h>
#include "Useless/Util/LangSelector.h"


#include "Useless/ErrorConfig.h"

namespace Useless {

    int Application::_minimum_idle_dt = 4;
    Signal_2<int, int>    Application::OnIdle;
    Signal                Application::OnUpdate;

    int                   Application::_encodingTable[256];

    HINSTANCE                       Application::_instance;
    Application::AllWindows        *Application::_windows = NULL;
    Application::MessageHandlers   *Application::_message_handlers = NULL;
    int                             Application::_counter = 0;
    int                             Application::_mouse_cursor_x;
    int                             Application::_mouse_cursor_y;
    int                             Application::_cursor_status;
    Timer                           Application::_app_start_time;
    const TCHAR                    *Application::_wnd_class = _T ("UTGS.Useless.Window");
    int                             Application::_argc;
    const LPWSTR                   *Application::_argv;

    USELESS_API bool G_EnableDDrawScr = 0;
    USELESS_API bool G_DisableAltTab = 0;

    void Application::Init( InitParams *parm )
    {
        _instance = parm->hInstance;
        _app_title = parm->appTitle;
        if ( parm->iconName )
            _icon_name = parm->iconName;
        if ( parm->iconResourceID )
            _icon_resource_id =parm->iconResourceID;
        if ( parm->bigIconResourceID )
            _big_icon_resource_id = parm->bigIconResourceID;
        if ( parm->windowClassName )
            _wnd_class = parm->windowClassName;
        
        _windows = new AllWindows;
        _message_handlers = new MessageHandlers;

        _argv = CommandLineToArgvW(GetCommandLineW(), &_argc);

        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX); 

        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = (WNDPROC)Application::EventHandler;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = Application::GetInstance();
        wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_BACKGROUND+1);
        wcex.lpszMenuName   = 0;
        wcex.lpszClassName  = Application::GetClass();

        if ( _icon_resource_id || _big_icon_resource_id )
        {
            if ( _big_icon_resource_id )
                wcex.hIcon      = LoadIcon(wcex.hInstance, MAKEINTRESOURCE( _big_icon_resource_id ) );
            else
                wcex.hIcon      = LoadIcon(wcex.hInstance, MAKEINTRESOURCE( _icon_resource_id ) );

            if ( _icon_resource_id )
                wcex.hIconSm    = LoadIcon(wcex.hInstance, MAKEINTRESOURCE( _icon_resource_id ) );
            else
                wcex.hIconSm    = LoadIcon(wcex.hInstance, MAKEINTRESOURCE( _big_icon_resource_id ) );
        }
        else if ( _icon_name )
        {   
            wcex.hIcon      = LoadIcon(wcex.hInstance, ToTCHAR(_icon_name).c_str());
            wcex.hIconSm    = LoadIcon(wcex.hInstance, ToTCHAR(_icon_name).c_str());
        }
        else
        {
            wcex.hIcon      = LoadIcon(NULL, IDI_APPLICATION);
            wcex.hIconSm    = LoadIcon(NULL, IDI_APPLICATION);
        }

        SetEncoding("cp1250");    

        if ( !RegisterClassEx(&wcex) ) { ErrorDialog(); }

    }

    void Application::Free()
    {
        if ( _windows ) { delete _windows; }
        if ( _message_handlers ) { delete _message_handlers; }
        GlobalFree( (HGLOBAL)_argv );
        ::InvalidateRect( NULL, NULL, TRUE );
    }

    void Application::Terminate()
    {
        PostQuitMessage(0L);
    }

    void Application::SetEncoding( const char *name )
    {
        std::locale loc( LangSelector::Instance()[ name ] );
        std::string encoded;
        std::wstring unified;
        for ( int i=0; i < 256; ++i ) { encoded += i; }
        Read( unified, encoded, loc );
        for ( int j=0; j < 256; ++j ) { _encodingTable[j] = unified[j]; }
    }

    void Application::RegisterWindow(HWND handle, Window* window)
    {
        (*_windows)[handle] = window;
        if( !(window->GetParent()) ) { ++_counter; }

        //@alt-tab
        //RegisterHotKey( handle, 100, MOD_ALT, VK_TAB );
    }

    void Application::UnregisterWindow( HWND hwnd)
    {
        //@alt-tab
        //UnregisterHotKey( hwnd, 100 );
        AllWindows::iterator it = (*_windows).find( hwnd);
        if ( it != (*_windows).end())
        {
            MessageHandlers::iterator im = (*_message_handlers).find( (*it).first);
            if ( im != (*_message_handlers).end())
            {
                (*_message_handlers).erase( im);
            }
            (*_windows).erase( it);
        }
    }

    void Application::RegisterHandler(HWND handle, MessageHandler handler)
    {
        (*_message_handlers)[handle] = handler;
    }

    Window* Application::GetWindowPtr(HWND handle)
    {
        if( (*_windows).find(handle) != (*_windows).end())
        {
            return (*_windows)[handle];
        }
        else { return NULL; }
    }

    Timer clickTime;
    Pos   clickPos;

    LRESULT CALLBACK Application::EventHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if ( (*_message_handlers).find(handle)!=(*_message_handlers).end() )
        {
            return (*((*_message_handlers)[handle]))(handle,message,wParam,lParam);
        }
        else
        {
            return DoHandleEvent( handle, message, wParam, lParam);
        }
    }

    LRESULT Application::DoHandleEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Timer procTime;
        Window* window = GetWindowPtr(handle);
        if (!window)
        {
            return DefWindowProc(handle, message, wParam, lParam);
        }
        switch(message)
        {
            case WM_CREATE:
                {
                    window->OnCreate.Send();
                }
            case WM_DESTROY: // close application when last window is closed
                window->OnDestroy.Send();
                UnregisterWindow( window->_window_handle);
                if (( 0 == window->GetParent()) && ( 0 == --_counter )) 
                {
                    PostQuitMessage(0);
                }
                break;

            case WM_PAINT:
                window->OnPaint.Send();
                break;

            case WM_ACTIVATE:
                if ( wParam==WA_INACTIVE )
                {
                    window->OnIdle.Disable();
                    window->OnDeactivate.Send();
                }
                else 
                {
                    window->OnActivate.Send();
                    window->OnIdle.Enable();
                }
                break;

            case WM_SIZE:
                window->OnResize.SetArg1(lParam&0xFFFF);
                window->OnResize.SetArg2(lParam>>16);
                window->OnResize.Send();
                return 0L;

            case WM_MOVE:
                window->OnMove.SetArg1(lParam&0xFFFF);
                window->OnMove.SetArg2(lParam>>16);
                window->OnMove.Send();
                return 0L;

            case WM_GETMINMAXINFO:
                {
                    if( window->NoSizing() )
                    {
                        Useless::Rect r = window->GetClientRect();
                        MINMAXINFO* pMinMax = (MINMAXINFO*) lParam;

                        DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
                        DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
                        DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
                        DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );

                        pMinMax->ptMinTrackSize.x = r.w + dwFrameWidth * 2;
                        pMinMax->ptMinTrackSize.y = r.h + dwFrameHeight * 2 + dwCaptionHeight;

                        pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
                        pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
                    }
                }
                return 0L;

            case WM_ERASEBKGND:
                {
                    if( window->NoClear() ) { return 0L; }
                }


                //@alt-tab
                //case WM_HOTKEY:
                //    {
                //        if ( wParam == 100 && G_DisableAltTab )
                //        {
                //            return 0L;
                //        }
                //        break;
                //    }

                // ---- ??? It doesnt work ??? ----
                // Double click
                /*case WM_LBUTTONDBLCLK:
                  window->OnDoubleClick.Send_1(0);
                  break;
                  case WM_RBUTTONDBLCLK:
                  window->OnDoubleClick.Send_1(1);
                  break;
                  case WM_MBUTTONDBLCLK:
                  window->OnDoubleClick.Send_1(2);
                  break;*/


                // Double click W/A for windows are not sent WM_{LRM}BUTTONDBLCLK messages
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
                {
                    switch( message )
                    {
                        case WM_LBUTTONDOWN: window->OnMouseBtn.Send_2(0x1,0x1); break;
                        case WM_RBUTTONDOWN: window->OnMouseBtn.Send_2(0x2,0x2); break;
                        case WM_MBUTTONDOWN: window->OnMouseBtn.Send_2(0x4,0x4); break;
                    }

                    int dx = GetSystemMetrics( SM_CXDOUBLECLK );
                    int dy = GetSystemMetrics( SM_CYDOUBLECLK );
                    Pos pos = Pos( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );

                    if ( procTime - clickTime < 0.4 && (pos-clickPos).Abs()<=Pos(dx,dy) )
                    {
                        switch( message )
                        {
                            case WM_LBUTTONDOWN: window->OnDoubleClick.Send_1(0); break;
                            case WM_RBUTTONDOWN: window->OnDoubleClick.Send_1(1); break;
                            case WM_MBUTTONDOWN: window->OnDoubleClick.Send_1(2); break;
                        }
                    }
                    clickTime = procTime;
                    clickPos = pos;
                    break;
                }

                /*
                // Button press & release
                case WM_LBUTTONDOWN:
                window->OnMouseBtn.Send_2(0x1,0x1);
                break;
                case WM_RBUTTONDOWN:
                window->OnMouseBtn.Send_2(0x2,0x2);
                break;
                case WM_MBUTTONDOWN:
                window->OnMouseBtn.Send_2(0x4,0x4);
                break;*/

            case WM_LBUTTONUP:
                window->OnMouseBtn.Send_2(0x0,0x1);
                break;

            case WM_RBUTTONUP:
                window->OnMouseBtn.Send_2(0x0,0x2);
                break;

            case WM_MBUTTONUP:
                window->OnMouseBtn.Send_2(0x0,0x4);
                break;

                // Wheel
            case WM_MOUSEWHEEL:
                {
                    unsigned int t = wParam>>16;
                    short dz = (unsigned short&)t;
                    window->OnMouseWheel.Send_2( dz, dz);
                }
                break;

                // Motion
            case WM_MOUSEMOVE:
            case WM_NCMOUSEMOVE:
                {
                    if ( message == WM_MOUSEMOVE )
                    {
                        if (_cursor_status==-1)
                        {
                            ::ShowCursor(false);
                            _cursor_status=0;
                        }
                    }
                    else if ( message == WM_NCMOUSEMOVE )
                    {
                        if (_cursor_status==0)
                        {
                            if ( window->NoCursorWrapping() )
                            {
                                ::ShowCursor(true);
                                _cursor_status=-1;
                            }
                        }
                    }

                    unsigned int u = lParam & 0xffff;
                    unsigned int v = lParam >> 16;
                    short x = (unsigned short&)u;
                    short y = (unsigned short&)v;

                    bool reset = false;

                    if ( !window->NoCursorWrapping() )
                    {
                        Rect cr = window->GetClientRect();

                        if ( cr.GetX1() >= x )
                        {
                            x = cr.GetX2() - 2;
                            _mouse_cursor_x += cr.GetW();
                            reset = true;
                        }
                        else if ( cr.GetX2() - 1 <= x )
                        {
                            x = cr.GetX1() + 1;
                            _mouse_cursor_x -= cr.GetW();
                            reset = true;
                        }

                        if ( cr.GetY1() >= y )
                        {
                            y = cr.GetY2() - 2;
                            _mouse_cursor_y += cr.GetH();
                            reset = true;
                        }
                        else if ( cr.GetY2() - 1 <= y )
                        {
                            y = cr.GetY1() + 1;
                            _mouse_cursor_y -= cr.GetH();
                            reset = true;
                        }
                    }

                    Pos dp( x-_mouse_cursor_x, y-_mouse_cursor_y );
                    window->OnMouseMove.Send_2( Pos(x,y), dp );
                    _mouse_cursor_x = x;
                    _mouse_cursor_y = y;

                    if ( reset )
                    {
                        window->SetCursorPos( x, y);
                    }
                }
                break;

                // Keyboard
            case WM_KEYDOWN:
                {
                    int key_code = (lParam & 0x01FF0000) >> 16;
                    window->OnKeyPress.Send_1(key_code);
                }
                break;

            case WM_KEYUP:
                {
                    int key_code = (lParam & 0x01FF0000) >> 16;
                    window->OnKeyRelease.Send_1(key_code);
                }
                break;

            case WM_CHAR: //WM_UNICHAR - not defined in 95/98/NT
                {
                    //assert( wParam < 256 && wParam >= 0 );
                    //int code = _encodingTable[ wParam ];
					// characters are already encoded in ucs-2
					wchar_t code = (wchar_t)wParam;
                    window->OnChar.Send_1( code );
                }
                break;
        }
        return DefWindowProc(handle, message, wParam, lParam);
    }

    int Application::Run()
    {
        if( (*_windows).empty() ) { APP_CRITICAL( Useless::Error("Run(): Application has no windows."),"Application"); }
        MSG msg;
        Useless::Timer init_time;
        Useless::Timer last_time;
        bool do_idle = true;
        HACCEL hAccelTable = 0;
        for(;;)
        {
            Application::OnUpdate.Send();
            Useless::Timer loopT1;
            if( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
            {
                if ( 0 == GetMessage(&msg, NULL, 0, 0)) // case WM_QUIT
                {
                    break;
                }
                /*if ( _windows->empty() )
                {
                    break;
                }*/
                if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else
            {
                Signal::Dispatch();

                if( !do_idle )
                {
                    WaitMessage();
                    do_idle = true;
                }
                else
                {
                    //int t = timeGetTime();
                    Useless::Timer t;
                    int dt = (int)((t-last_time)*1000.0);
                    if( dt >= _minimum_idle_dt )
                    {
                        int arg1 = (int)((t-init_time)*1000.0);
                        int arg2 = dt;
                        do_idle = OnIdle.Send_2( arg1, arg2);

                        AllWindows::iterator wnd_it = (*_windows).begin();
                        for (; wnd_it!=(*_windows).end(); ++wnd_it)
                        {
                            (*wnd_it).second->OnIdle.Send_2( arg1, arg2);
                        }

                        last_time = t;
                        Useless::Timer loopT2;
                        long Dt = (int)(_minimum_idle_dt - ( (loopT2-loopT1)*1000.0 ));
                        if (Dt>0) { Sleep(Dt); }
                    }
                    OnTimer::Instance().Execute();
                }
            }
        }
        return msg.wParam;
    }

    int Application::Step( int flags )
    {
        MSG msg;
        HACCEL hAccelTable = 0;

        if ( flags & DEQUEUE )
        {
            while ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
            {
                if ( 0 == GetMessage(&msg, NULL, 0, 0)) // case WM_QUIT
                {
                    if ( flags & QUIT_EXCEPTION) // quit application
                    {
                        throw Quit();
                    }
                    else {
                        return false;
                    }
                }
                /*if ( _windows->empty() )
                {
                    if ( flags & QUIT_EXCEPTION) // quit application
                    {
                        throw Quit();
                    }
                    else {
                        return false;
                    }
                }*/
                if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            Signal::Dispatch();
        }
        if ( flags & IDLE )
        {
            OnIdle.Send();

            AllWindows::iterator wnd_it = (*_windows).begin();
            for (; wnd_it!=(*_windows).end(); ++wnd_it)
            {
                (*wnd_it).second->OnIdle.Send();
            }
        }
        if ( flags & TIMER )
        {
            OnTimer::Instance().Execute();
        }

        return true;
    }

};//namespace Useless
