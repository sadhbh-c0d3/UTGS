#ifndef __INCLUDED_APPLICATION_H__
#define __INCLUDED_APPLICATION_H__

/*
 * $Id: Application.h,v 1.20 2004/09/01 18:20:40 koolas Exp $
 *
 *  NAME
 *      Application (W32 version)
 *
 *  PURPOSE
 *      Ease creation of W32 applications
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *  
*/ 

#pragma warning(disable:4786)
#include <map>
#include <windows.h>
#include <string>
#include <iterator>
#include <algorithm>

#include "Useless/Functional/Signal.h"
#include "Useless/Error/Error.h"
#include "Useless/System/w32/Timer.h"

/*! \defgroup gApplication Application */

/*! \ingroup gApplication
 *  The main entry point of users programm.
 *  This function should execute Application::Run() at the ending;
 */
int Main();

namespace Useless {

class Window;

/*! Convert from regualr chars to windows TCHAR
*/
inline std::basic_string<TCHAR>
    ToTCHAR( const std::basic_string<char> &y)
{
    std::basic_string<TCHAR> valueTC;
    std::copy( y.begin(), y.end(), std::back_inserter(valueTC));
    return valueTC;
}

typedef std::basic_string<TCHAR> TCHAR_string;


/*! \ingroup gApplication
 *  Static class. It manages windows, system-events and incoming-errors.
 */
class USELESS_API Application
{
public:
    static void SetIdleDT(int dt) { _minimum_idle_dt = dt; }
    static Signal_2<int, int> OnIdle;
    static Signal OnUpdate;
    
    static Timer GetStartTime() { return _app_start_time; }

    /* Methods below are "static virtual"  */
    static  void SendErrorText( int priority, const char *module_name, const char *error_text );
    static  void OnExit();
    /*----------------------------------------------------------------------------*/

    static  void SendError( int priority, const char* module_name, const Error& error )
    {
        SendErrorText( priority, module_name, error.GetError());
    }

    /* Use: InstancePtr()->Terminate() call to terminate application */
      void Terminate();
    static Application* InstancePtr() { return NULL; }

    static const char * GetTitle() { return _app_title; }
    
    static void SetEncoding( const char *name );

private:
    static int   _minimum_idle_dt;
    static int   _counter;
    static Timer _app_start_time;

///////////////////////////////////////////// Win32 specific
public:
/*! \section */
    struct USELESS_API InitParams
    {
        HINSTANCE    hInstance;
        const char * appTitle;
		const char * iconName;
        int iconResourceID;
        int bigIconResourceID;
        const TCHAR *windowClassName;

        InitParams( HINSTANCE lhInstance, const char *lappTitle )
            : hInstance( lhInstance ), appTitle( lappTitle ),
		    iconName(NULL),
            iconResourceID(0),
            bigIconResourceID(0),
            windowClassName(NULL)
        {}
    };
    
    static void Init( InitParams *parm );
    static void Free();
    static HINSTANCE GetInstance() { return _instance; }
    static const TCHAR* GetClass() { return _wnd_class; }
    static void RegisterWindow(HWND handle, Window* window);
    static void UnregisterWindow( HWND window);
    static Window* GetWindowPtr(HWND handle);
    static int Run();
    enum { DEQUEUE=1, IDLE=2, TIMER=2, QUIT_EXCEPTION=4 };
    static int Step( int flags = DEQUEUE | IDLE | TIMER );

    typedef Useless::FTetraryR<HWND, UINT, WPARAM, LPARAM, LRESULT>::Ptr MessageHandler;
    static void RegisterHandler(HWND handle, MessageHandler);

    static LRESULT CALLBACK EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT DoHandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    enum CursorStatus {
        CURSOR_VISIBLE = 1
    };
    static void ShowCursor() { ::ShowCursor(true); _cursor_status|=(CURSOR_VISIBLE); }
    static void HideCursor() { ::ShowCursor(false); _cursor_status&=(~CURSOR_VISIBLE); }
    static int  GetCursorStatus() { return _cursor_status; }

    static int    GetArgC() { return _argc; }
    static const LPWSTR* GetArgV() { return _argv; }

private:

    static HINSTANCE   _instance;
    static const char *_app_title;
    static const char *_icon_name;
    static int   _icon_resource_id;
    static int   _big_icon_resource_id;
    static const TCHAR *_wnd_class;
    static int         _mouse_cursor_x;
    static int         _mouse_cursor_y;
    static int         _cursor_status;
    static int         _encodingTable[256];
    static int          _argc;
    static const LPWSTR *_argv;
    
    typedef std::map< HWND, Window* > AllWindows;
    static AllWindows *_windows;

    typedef std::map< HWND, MessageHandler > MessageHandlers;
    static MessageHandlers *_message_handlers;
};


};// namespace Useless;
#endif __INCLUDED_APPLICATION_H__
