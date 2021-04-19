#ifndef __INCLUDED_USELESS_APP_ERROR_H__
#define __INCLUDED_USELESS_APP_ERROR_H__

///////////////////////////////////////////////////////////////////////////////
//
// IF YOU ARE SUPPOSED TO USE DEFAULT ERROR CONFIG THEN INCLUDE ErrorConfig.h
//
//

#include "Useless/UselessConfig.h"
#include "Useless/Error/Error.h"

namespace Useless {

    USELESS_API void SendErrorText( int priority, const char *module_name, const char *error_text );
	USELESS_API void SendError( int priority, const char* module_name, const Error& error );

};//namespace Useless

namespace Useless {

#ifdef  __ERROR_LEVEL_5__
    #define __ERROR_LEVEL_4__
    #define __ERROR_LEVEL_3__
    #define __ERROR_LEVEL_2__
    #define __ERROR_LEVEL_1__
    #define __ERROR_LEVEL_0__
    #define       HUGE_LOG(e,n)  ::Useless::SendError(5, n, e);
#else
    #define       HUGE_LOG(e,n) {}
#endif

#ifdef  __ERROR_LEVEL_4__
    #define __ERROR_LEVEL_3__
    #define __ERROR_LEVEL_2__
    #define __ERROR_LEVEL_1__
    #define __ERROR_LEVEL_0__
    #define     MEDIUM_LOG(e,n)  ::Useless::SendError(4, n, e);
#else
    #define     MEDIUM_LOG(e,n) {}
#endif

#ifdef  __ERROR_LEVEL_3__
    #define __ERROR_LEVEL_2__
    #define __ERROR_LEVEL_1__
    #define __ERROR_LEVEL_0__
    #define      SMALL_LOG(e,n)  ::Useless::SendError(3, n, e);
#else
    #define      SMALL_LOG(e,n) {}
#endif

#ifdef  __ERROR_LEVEL_PEDANTIC__
    throw e;
#elif defined __ERROR_LEVEL_2__
    #define __ERROR_LEVEL_1__
    #define __ERROR_LEVEL_0__
    #define    APP_WARNING(e,n)  ::Useless::SendError(2, n, e);
#else
    #define   APP_WARNING(e,n) {}
#endif

#ifdef  __ERROR_LEVEL_1__
    #define __ERROR_LEVEL_0__
    #define      APP_ERROR(e,n)  ::Useless::SendError(1, n, e);
#else
    #define      APP_ERROR(e,n) throw e;
#endif

#ifdef  __ERROR_LEVEL_0__
    #define   APP_CRITICAL(e,n)  ::Useless::SendError(0, n, e);
#else
    #define   APP_CRITICAL(e,n)  throw e;
#endif


};//namespace Useless
#endif//__INCLUDED_USELESS_APP_ERROR_H__
