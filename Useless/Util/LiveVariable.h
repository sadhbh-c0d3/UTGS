#ifndef __INCLUDED_USELESS_LIVE_VARIABLE_H__
#define __INCLUDED_USELESS_LIVE_VARIABLE_H__

#include "Useless/Functional/Signal.h"

namespace Useless {

/* \defgroup ObjSync Object Synchronization \ingroup Util*/

/*! \ingroup ObjSync
 *  Live Variable sends OnUpdate signal every single time, when it's
 *  value is being changed.
 */
template< class T >
class LiveVariable
{
public:
    LiveVariable(): _variable(0) {}
    LiveVariable(const T&variable): _variable(variable) {}

    LiveVariable& operator=(const T &variable) 
        { 
            if (_variable!=variable)
            {
                _variable=variable;
                OnUpdate.Send_1(_variable);
            }
            return *this; 
        }

    operator const T&() const
        { 
            return _variable; 
        }

    void Set(const T &variable) { (*this) = variable; }

    Signal_1<T> OnUpdate;

private:
    T _variable;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_LIVE_VARIABLE_H__
