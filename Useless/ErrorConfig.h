#ifndef __INCLUDED_USELESS_ERROR_CONFIG_H__
#define __INCLUDED_USELESS_ERROR_CONFIG_H__

//////////////////////////////////////////////////////////////////
// THIS IS RUN-TIME ERROR CONFIGURATION
//
// + Level: 5 down to 0
// + Pedantic: warning throws exception
// + GUI event logging
//
// Note:
//  Any changes in this file, causes all USELESS libraries to rebuid.
//---------------------------------------------------------------


///////////////////////////////////
// User configuration
//--------------------------------


// Set error level [0..5]
#ifdef NDEBUG
#   define __ERROR_LEVEL_1__
#else
#   define __ERROR_LEVEL_4__
#endif


// Treat all warnings as errors
//#define __ERROR_PEDANTIC__


// Set gui debuging
#if !defined(__GUI_DEBUG_LEVEL_NONE__) &\
    !defined(__GUI_DEBUG_LEVEL_LOW__) &\
    !defined(__GUI_DEBUG_LEVEL_MEDIUM__) &\
    !defined(__GUI_DEBUG_LEVEL_HIGH__)
// Choose one [HIGH, MEDIUM, LOW, NONE]
#   define __GUI_DEBUG_LEVEL_NONE__
#endif


///////////////////////////////////
// Include error macros
//--------------------------------
#include "Useless/Error/Error.h"
#include "Useless/Error/AppError.h"

#endif//__INCLUDED_USELESS_ERROR_CONFIG_H__
