/*
**  $Id: Error.cpp,v 1.8 2003/05/19 12:46:00 koolas Exp $
**
**  NAME
**      Error
**
**  PURPOSE
**      Exception handling base class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Error.h"
#include <stdio.h>

namespace Useless {

Error::Error( const char *e, ... )
{
    va_list arglist;
    va_start( arglist, e );
    PrintError( e, arglist);
    va_end( arglist );

    FILE *useErrors = fopen("Errors.txt", "r");
    if ( useErrors )
    {
        fclose( useErrors );
        FILE *errors = fopen("Errors.txt", "a+");
        fprintf( errors, "-> %s\n", _errortext );
        fclose( errors );
    }
}


Error::~Error() throw()
{
}

void Error::PrintError( const char *e, va_list arglist)
{
#   ifdef WIN32
    _vsnprintf( _errortext, BUFLEN-2, e, arglist );
#   else
    vsnprintf( _errortext, BUFLEN-2, e, arglist );
#   endif
    _errortext[ BUFLEN-1 ] = 0;
}

const char *Error::GetError() const
{
    return _errortext;
}


} // namespace Useless

