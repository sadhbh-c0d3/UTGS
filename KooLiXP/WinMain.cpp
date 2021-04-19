#include "KoolixpPch.h"

#define USELESS_API __declspec(dllimport)

#pragma warning(disable:4251)
#pragma warning(disable:4267)
#pragma warning(disable:4275)


#if _MSC_VER > 1000
#pragma once
#endif
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

#include <tchar.h>

#include "Useless/System/Application.h"
#include "Useless/Error/Quit.h"

// Include KooLiXP bindings
#include "koolib/xml/XMLModuleBuiltIn.h"
#include "koolib/xml/XMLModuleUseless.h"
#include "koolib/xml/XMLModuleDynamo.h"


#ifdef __DEBUG_LEAKS__
#   include <crtdbg.h>
#endif

//#define __DEBUG_LAYER__

#ifdef __DEBUG_LAYER__
#   include "Bugslayer/BugSlayerUtil.h"

    static long dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;

    static void my_translator(unsigned code, EXCEPTION_POINTERS *pExPtrs )
    {
        FILE *out = NULL;
        out = fopen( "StackDump.log" , "w" );
	    const TCHAR * szBuff = GetFirstStackTraceString ( dwOpts  , pExPtrs  ) ;
	    do
	    {
            fprintf( out , "%s\n" , szBuff );
		    szBuff = GetNextStackTraceString ( dwOpts , pExPtrs ) ;
	    }
	    while ( NULL != szBuff ) ;
        fclose( out );
        throw code;
    }
#endif

Useless::Error g_FatalError("Exception occured. Program will exit.");

int ErrorBox( const std::exception &e )
{
    MessageBox( NULL
            , Useless::ToTCHAR( e.what() ).c_str()
            , Useless::ToTCHAR( Useless::Application::GetTitle() + std::string(" - Exception")).c_str()
            , MB_OK | MB_ICONINFORMATION );
    return 0;
}

int FreeApp ()
{
    try
    {
        Useless::Application::Free();
        return 0;
    }
    catch( const std::exception &e1 ) 
    {
        try
        {
            Useless::Application::SendErrorText( 0 , "Application::Free()" , e1.what() );
        }
        catch( ... )
        {
            return -1;
        }
    }
    catch( ... ) 
    {
        try
        {
            Useless::Application::SendErrorText( 0 , "Application::Free()" , g_FatalError.what() );
        }
        catch( ... )
        {
            return -1;
        }
    }
    return -1;
}

void OnExit()
{
    Useless::Resources::Kill();
}

int ExitApp()
{
    try
    {
        Useless::Application::OnExit();
		OnExit();
        FreeApp();
        return 0;
    }
    catch( const std::exception &e1 ) 
    {
        try
        {
            Useless::Application::SendErrorText( 0 , "Application::OnExit()" , e1.what() );
        }
        catch( ... )
        {
            FreeApp();
            return -1;
        }
    }
    catch( ... ) 
    {
        try
        {
            Useless::Application::SendErrorText( 0 , "Application::OnExit()" , g_FatalError.what() );
        }
        catch( ... )
        {
            FreeApp();
            return -1;
        }
    }            
    return -1;
}

int ErrorExit( const std::exception &x )
{
    ExitApp();
    ErrorBox( x );
    try {
        Useless::Application::SendErrorText( 0 , "Main" , x.what() );
    }
    catch( ... ) {
        return -1;
    }
    return -1;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
#ifdef __DEBUG_LAYER__
    _set_se_translator(my_translator);
    SetDiagAssertOptions ( dwOpts ) ;
#endif

#ifdef __DEBUG_LEAKS__
    int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );    // Get current flag
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;                       // Turn on leak-checking bit
//    tmpFlag |= CRTDBG_CHECK_ALWAYS_DF;
    _CrtSetDbgFlag( tmpFlag );                              // Set flag to the new value
#endif

    Useless::Error ErrorLogInit("Start");
    
    Useless::Application::InitParams parm( hInstance, "Useless" );
    parm.iconName = "icon.ico";
    parm.iconResourceID = 101;
    parm.bigIconResourceID = 101;
    parm.windowClassName = _T("UTGS.Useless.Window");

    Useless::Application::Init( &parm );

    int r = 0;
    try {
        r = Main(); 
    }
    catch( const Useless::Quit &q )
    {
        r = q;
    }
    catch( const Useless::Error &e )
    {
        return ErrorExit( e );
    }
    catch( const std::exception &e )
    {
        return ErrorExit( e );
    }
    catch( ... )
    {
        return ErrorExit( g_FatalError );
    }
    
    r |= ExitApp();

    return r;
}
