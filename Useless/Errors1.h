#ifndef __INCLUED_USELESS_ERRORS_1_H__
#define __INCLUED_USELESS_ERRORS_1_H__

#include "Useless/Error/ErrorMessage.h"

/*! \file Errors.h
 * 
 * Configure or disable messages for specific errors, warnings, logs.
 * 
 */

#ifdef __INCLUDED__USELESS_WIDGET_CYCLER_H__

SET_ERROR_MESSAGE( Useless::WidgetCycler, 0, "An error occured, please contact with software provider.");

#endif

#endif//__INCLUED_USELESS_ERRORS_1_H__
