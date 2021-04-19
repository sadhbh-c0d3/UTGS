#ifndef __INCLUDED_USELESS_UTIL_ADVASSERT_H__
#define __INCLUDED_USELESS_UTIL_ADVASSERT_H__

#include <assert.h>

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
};//Useless

/*! \file Assert.h
 *
 *  TODO: implemeted only for windows. This x11 version is dummy file.
 *
 *  Here I shall define several assert macros:
 *  
 *  assert( expr)  - in debug gives call trace, in retail builds it dissappears
 *  verify( expr)  - in debug like assert, in retail like debug assert
 *  warning( expr) - in both debug and retail, warning is being logged
 *  log_until( expr)  - log if not expression (debug only)
 *
 */

#define C_assert( x ) assert( x );
#define C_verify( x ) if ( !(x) ) { throw 0; }
#define C_warning( x )
#define C_log_until( x )
#define C_msg_assert( x, m ) assert( x );
#define C_msg_verify( x, m ) if ( !(x) ) { throw 0; }
#define C_msg_warning( x, m )
#define C_log_until( x, m )




//! It is bad idea to overload known macro names...
//#ifdef assert
//#   undef assert
//#endif//assert

//#ifdef verify
//#   undef verify
//#endif//verify

//#define assert(x) C_assert(x)
//#define verify(x) C_verify(x)

#endif // __INCLUDED_USELESS_UTIL_ADVASSERT_H__
