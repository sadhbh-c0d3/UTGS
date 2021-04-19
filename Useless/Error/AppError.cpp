#include "UselessPch.h"

#include "Useless/Error/AppError.h"
#include "Useless/System/Application.h"
#include "Useless/Error/Quit.h"
#include <time.h>
#include <stdio.h>

#include "Useless/System/DbgOut.h"

namespace Useless {

char g_lastErrorModuleName[256] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0 };

char g_lastErrorMessage[256] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0 };

int g_errorCounter = 1;

void SendErrorText( int priority, const char* module_name, const char *error_text )
{
    Application::SendErrorText( priority, module_name, error_text);
}

void SendError( int priority, const char* module_name, const Error& error )
{
    SendErrorText( priority, module_name, error.GetError());
}

/*! Default error handler.
 *  It's "static virtual" method of Application.
 *  You may define your own version of this handler.
 */
void Application::SendErrorText( int priority, const char* module_name, const char *error_text )
{
    size_t title_len = strlen(Application::_app_title);
    title_len = (title_len<250)? title_len : 250;
    char app_title[256];
    memset( app_title, 0L, 250);
    memcpy( app_title, Application::_app_title, title_len);
    memcpy( app_title+title_len, ".log", 4);
    
    if ( 0 != strcmp( g_lastErrorModuleName, module_name ) ||
         0 != strcmp( g_lastErrorMessage, error_text ) )
    {
        size_t l1 = strlen( module_name );
        size_t l2 = strlen( error_text );
        memset( g_lastErrorModuleName, 0, 256 );
        memset( g_lastErrorMessage, 0, 256 );
        memcpy( g_lastErrorModuleName, module_name, l1>255 ? 255 : l1 );
        memcpy( g_lastErrorMessage, error_text, l2 > 255 ? 255 : l2 );
        g_lastErrorModuleName[255] = 0x00;
        g_lastErrorMessage[255] = 0x00;
                
        FILE *logfile=fopen( app_title, "a+");
        try 
        { 
            if ( g_errorCounter > 1 )
            {
                fprintf( logfile, "\tRepeated [ %i ] times\n", g_errorCounter );
                print_dbg( "\tRepeated [ %i ] times\n", g_errorCounter );
            }

            print_dbg( "[Useless]$ P%i %s: %s \n", priority, module_name, error_text);
        
            static bool log_was_started = false;
            if (!log_was_started)
            {
                time_t t;
                time(&t);
                char *time_chars = asctime( gmtime(&t));
                fprintf( logfile, "\n Date: %s", time_chars);
                for ( int ii=0; ii!=120; ++ii ) { fputs("-", logfile); }
                fputs("\n", logfile);
                log_was_started = true;
            }
            fprintf( logfile, "%i[%s]$ %s \n", priority, module_name, error_text);
            fclose(logfile);

        } catch(...) { try{ fclose(logfile); } catch(...){;} }        
        g_errorCounter = 1;
    }
    else
    {
        ++g_errorCounter;
    }
    
    if (priority < 2) 
    { 
        throw Error( error_text );
    }
}

};//namespace Useless
