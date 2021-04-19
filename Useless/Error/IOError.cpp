/*
**  $Id: IOError.cpp,v 1.7 2003/01/20 20:40:58 koolas Exp $
**
**  NAME
**      IOError
**
**  PURPOSE
**      Exception handling class used to report io errors.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/Error/IOError.h"

namespace Useless {


IOError::IOError( const char *e, ... )
{
    va_list arglist;
    va_start( arglist, e );
    PrintError(e, arglist );
    va_end( arglist );
}


IOError::~IOError() throw()
{
}


} // namespace Useless
