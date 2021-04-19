#ifndef __INCLUDED_USELESS_DDRAW_ERROR_H__
#define __INCLUDED_USELESS_DDRAW_ERROR_H__

#include "Useless/Error/Error.h"

namespace Useless {

/*! \ingroup gError 
 *  Direct Draw specific error
 */
class DDError : public Error
{
public:
    DDError(long dd_error_code);
    virtual ~DDError() {}

private:
    char *_temporary;
    char *Deduce(long dd_error_code);
};

};//namespace Useless
#endif//__INCLUDED_USELESS_DDRAW_ERROR_H__
