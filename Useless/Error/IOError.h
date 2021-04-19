#ifndef __INCLUDED_USELESS_IOERROR_H__
#define __INCLUDED_USELESS_IOERROR_H__

/*
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

#ifndef __INCLUDED_USELESS_ERROR_H__
#include "Useless/Error/Error.h"
#endif //__INCLUDED_USELESS_ERROR_H__


namespace Useless {

/*! \ingroup gError 
 *  File IO specific error
 */
class USELESS_API IOError : public Error
{
public:
    IOError( const char *e, ... );
    virtual ~IOError() throw();
};


} // namespace Useless

#endif // __INCLUDED_USELESS_IOERROR_H__


