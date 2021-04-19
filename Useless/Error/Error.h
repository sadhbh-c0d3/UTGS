#ifndef __INCLUDED_USELESS_ERROR_H__
#define __INCLUDED_USELESS_ERROR_H__

/*
**  $Id: Error.h,v 1.7 2003/08/08 14:41:19 koolas Exp $
**
**  NAME
**      Error
**
**  PURPOSE
**      Exception handling base class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include <stdarg.h>
#include <exception>

namespace Useless {
/*! \defgroup gError Error Handling */

/*! \ingroup gError
 *  Base of any error class
 */
class USELESS_API Error : public std::exception
{
public:

    Error() {}
    
    Error( const char *e, ... );
    
    virtual ~Error() throw();
    
    // Print error message
    void PrintError( const char *e, va_list ap);
    
    // Get error message text
    virtual const char *GetError() const;

    // std::exception get error message
    virtual const char *what() const throw() { return GetError();}

private:
    enum { BUFLEN=1024 };
    char _errortext[BUFLEN];
};


struct LongError : public Error
{
public:
    LongError( const char *err )
        : _errorMsg( err )
    {}

    virtual const char *
    GetError() const
        { return _errorMsg; }

private:
    const char *_errorMsg;
};

} // namespace Useless

#endif // __INCLUDED_USELESS_ERROR_H__


