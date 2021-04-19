#ifndef __INCLUDED_USELESS_BETTER_ASSERT_H__
#define __INCLUDED_USELESS_BETTER_ASSERT_H__

#include "Useless/ErrorConfig.h"

// Some copy-pasted stuff to avoid unnecessary definitions
//-------------------------------------------------------------
//
// #include "Bugslayer/DiagAssert.h"
//

#define DA_SHOWMSGBOX       0x0001
#define DA_SHOWODS          0x0002
#define DA_SHOWSTACKTRACE   0x0004

typedef void(AssertCallbackFunction)(char*);

int USELESS_API __stdcall DiagAssert ( unsigned long dwOverrideOpts,
                 const char * szMsg,
                 const char * szFile,
                 unsigned long dwLine,
                 AssertCallbackFunction *callbackFunction=0 );
//-------------------------------------------------------------

namespace Useless {
    struct AssertPriority
    {
        enum Priorities
        {
            Critical=0,  //< throws
            Error=1,     //< throws
            Warning=2,   //< throws when pedantic
            Message=2,   //< like warning
            Small_Log=3, //< log only
            Medium_Log=4,//< log only
            Huge_Log=5   //< log only
        };
    };

    // Helper functor used to redirect error messages to log file.
    template< const int _Priority >
    struct AssertLogger
    {
        static void Write( char *buffer )
        {
            SendErrorText( _Priority, "Assertion", buffer);
        }
        
        static void WriteError( const Error &error )
        {
            SendError( _Priority, "Assertion", error);
        }
    };


//Useless
//-------------------------------------------------------------

//! Assert with message-box and call-trace
#define FULL_DEBUG_ASSERT(x,m,p)\
    do {\
        if ( !(x) &&\
        DiagAssert( (DA_SHOWSTACKTRACE | DA_SHOWMSGBOX),  m, __FILE__, __LINE__, &::Useless::AssertLogger<p>::Write))\
        {\
            __debugbreak();\
        }\
    } while (0)

//! Assert with message-box, but without call-trace
#define DEBUG_ASSERT(x,m,p)\
    do {\
        if ( !(x) &&\
        DiagAssert( DA_SHOWMSGBOX,  m, __FILE__, __LINE__, &::Useless::AssertLogger<p>::Write))\
        {\
            __debugbreak();\
        }\
    } while (0)


//! Assert with call-trace and without message-box
#define QUIET_ASSERT(x,m,p)\
    do {\
        if ( !(x))\
        {\
        DiagAssert ( DA_SHOWSTACKTRACE, m, __FILE__, __LINE__, &::Useless::AssertLogger<p>::Write);\
        }\
    } while (0)

//! Assert without call-trace and without message-box (log only)
#define QUICK_ASSERT(x,m,p)\
    if ( !(x))\
    {\
        ::Useless::Error error("File: %s\n"\
                    "Line: %s\n"\
                    "Expression: %s"\
                    ,__FILE__\
                    ,__LINE__\
                    , m);\
        ::Useless::AssertLogger<p>::WriteError( error.GetError());\
    }

};//namespace Useless

#endif//__INCLUDED_USELESS_BETTER_ASSERT_H__
