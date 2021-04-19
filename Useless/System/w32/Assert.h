#ifndef __INCLUDED_USELESS_UTIL_ADVASSERT_H__
#define __INCLUDED_USELESS_UTIL_ADVASSERT_H__

#include "Useless/System/w32/BetterAssert.h"

/*! \file Assert.h
 *
 *  Here I define several assert macros:
 *
 *  assert( expr)  - in debug gives call trace, in retail builds it dissappears
 *  verify( expr)  - in debug like assert, in retail like debug assert
 *  warning( expr) - in both debug and retail, warning is being logged
 *  log_until( expr)  - log if not expression (debug only)
 *
 *  One may create its own assert macros using thos defined below
 *  
 *   FULL_DEBUG_ASSERT( expr, msg, pri) - call trace, message box and debug log
 *   DEBUG_ASSERT( expr, msg, pri)      - message box and debug log
 *   QUIET_ASSERT( expr, msg, pri)      - call trace and debug log
 *   QUICK_ASSERT( expr, msg, pri)      - debug log only
 *
 *   pri - priority is being checked by Application::SendErrorText()
 *
 *   Used values:
 *      1 = AssertPriority::Error   - throw exception (and exit)
 *      2 = AssertPriority::Warning - pass thru, log assertion message
 *
 *   Other values: (for logging purposes)
 *      3 = AssertPriority::Small_Log  - if ERROR_LEVEL is set to 3 or greater (see ErrorConfig.h)
 *      4 = AssertPriority::Medium_Log - if ERROR_LEVEL is set to 4 or greater
 *      5 = AssertPriority::Huge_Log   - if ERROR_LEVEL is set to 5
 *
 */


#if !defined(NDEBUG)
#   define C_assert(x)  FULL_DEBUG_ASSERT(x, #x, ::Useless::AssertPriority::Warning )
#   define C_verify(x)  FULL_DEBUG_ASSERT(x, #x, ::Useless::AssertPriority::Warning )
#   define C_warning(x) QUIET_ASSERT(x, #x, ::Useless::AssertPriority::Warning )
#   define C_log_until(x) QUIET_ASSERT(x, #x, ::Useless::AssertPriority::Small_Log )

#   define C_msg_assert(x,m)  FULL_DEBUG_ASSERT(x, m, ::Useless::AssertPriority::Warning )
#   define C_msg_verify(x,m)  FULL_DEBUG_ASSERT(x, m, ::Useless::AssertPriority::Warning )
#   define C_msg_warning(x,m) QUIET_ASSERT(x, m, ::Useless::AssertPriority::Warning )
#   define C_msg_log_until(x,m) QUIET_ASSERT(x, m, ::Useless::AssertPriority::Small_Log )

#else
#   define C_assert(x)
#   define C_verify(x)  QUIET_ASSERT(x, #x, ::Useless::AssertPriority::Error )
#   define C_warning(x) QUIET_ASSERT(x, #x, ::Useless::AssertPriority::Warning )
#   define C_log_until(x)

#   define C_msg_assert(x,m)
#   define C_msg_verify(x,m)  QUIET_ASSERT(x, m, ::Useless::AssertPriority::Error )
#   define C_msg_warning(x,m) QUIET_ASSERT(x, m, ::Useless::AssertPriority::Warning )
#   define C_msg_log_until(x,m)

#endif



//! It is bad idea to overload known macro names...
#ifdef assert
#   undef assert
#endif//assert

#ifdef verify
#   undef verify
#endif//verify

#define assert(x) C_assert(x)
#define verify(x) C_verify(x)


#endif // __INCLUDED_USELESS_UTIL_ADVASSERT_H__
