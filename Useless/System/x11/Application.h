#ifndef __INCLUDED_APPLICATION_H__
#define __INCLUDED_APPLICATION_H__

/*
 * $Id: Application.h,v 1.2 2003/01/20 20:54:09 koolas Exp $
 *
 *  NAME
 *      Application (X11 version)
 *
 *  PURPOSE
 *      Ease creation of X11 applications
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *  
*/ 

#pragma warning(disable:4786)
#include <map>
#include <string>

#include "Useless/Functional/Signal.h"
#include "Useless/Error/Error.h"
#include "Useless/System/w32/Timer.h"

//typedef ::Window XWindow;

/*! \defgroup gApplication Application */

/*! \ingroup gApplication
 *  The main entry point of users programm.
 *  This function should execute Application::Run() at the ending;
 */
int Main();

namespace Useless {

class Window;

/*! \ingroup gApplication
 *  Static class. It manages windows, system-events and incoming-errors.
 */
class Application
{
public:
    static void SetIdleDT(int dt) { _minimum_idle_dt = dt; }
    static Signal_2<int, int> OnIdle;
    static Signal OnUpdate;
    
    static Timer GetStartTime() { return _app_start_time; }

    /* Methods below are "static virtual"  */
    static  void SendError( int priority, const char* module_name, const Error& error );
    static  void SendErrorText( int priority, const char* module_name, const char *error_text );
    static  void OnExit();
    /*----------------------------------------------------------------------------*/

    /* Use: InstancePtr()->Terminate() call to terminate application */
      void Terminate();
    static Application* InstancePtr() { return NULL; }
    static const char * GetTitle() { return _app_title; }

private:
    static int   _minimum_idle_dt;
    static int   _counter;
    static Timer _app_start_time;

private:
    static const char *_app_title;
    static const char *_wnd_class;
    static const char *_icon_name;
    static int         _mouse_cursor_x;
    static int         _mouse_cursor_y;
    static int         _cursor_status;
    
    //typedef std::map< XWindow, Window* > AllWindows;
    //static AllWindows *_windows;

    //typedef std::map< XWindow, MessageHandler > MessageHandlers;
    //static MessageHandlers *_message_handlers;
};


};// namespace Useless;
#endif __INCLUDED_APPLICATION_H__
