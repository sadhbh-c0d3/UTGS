#ifndef __INCLUDED_USELESS_ASSERTERROR_H__
#define __INCLUDED_USELESS_ASSERTERROR_H__

/*
**  $Id: AssertError.h,v 1.5 2003/01/20 20:40:58 koolas Exp $
**
**  NAME
**      AssertError
**
**  PURPOSE
**      Exception handling class used to report failed assertions.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#ifndef __INCLUDED_USELESS_ERROR_H__
#include "Useless/Error/Error.h"
#endif // __INCLUDED_USELESS_ERROR_H__


namespace Useless {

/*! \ingroup gError 
 *  Used when assertion raises exception
 */
class USELESS_API AssertError : public Error
{
public:
    AssertError( const char *cond, const char *file, int line );
    virtual ~AssertError() throw();
};


} // namespace Useless

#endif // __INCLUDED_USELESS_ASSERTERROR_H__


