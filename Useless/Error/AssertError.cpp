/*
**  $Id: AssertError.cpp,v 1.6 2003/01/20 20:40:58 koolas Exp $
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

#include "UselessPch.h"

#include "AssertError.h"


namespace Useless {

AssertError::AssertError( const char *cond, const char *file, int line )
    : Error("Assertion \"%s\" failed in file \"%s\" at line %i.",cond,file,line)
{
}


AssertError::~AssertError() throw()
{
}


} // namespace Useless

