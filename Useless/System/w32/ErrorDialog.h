#ifndef __INCLUDED_ERROR_DIALOG_H__
#define __INCLUDED_ERROR_DIALOG_H__
#include <windows.h>

/*
 * $Id: ErrorDialog.h,v 1.3 2002/09/12 14:27:38 koolas Exp $
 *
 *  NAME
 *      ErrorDialog 
 *
 *  PURPOSE
 *      Only GDI errors
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *
*/ 

namespace Useless {

/*! \ingroup gApplication
 *  Dialog for display standard Win32 errors.
 */
class USELESS_API ErrorDialog
{
public:
    ErrorDialog();
    ~ErrorDialog() {}
};

};// namespace Useless

#endif __INCLUDED_ERROR_DIALOG_H__

