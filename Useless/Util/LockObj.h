#ifndef __INCLUDED__USELESS_LOCK_OBJ_H__
#define __INCLUDED__USELESS_LOCK_OBJ_H__

#include "Useless/Util/TemplateDefines.h"
#include "Useless/Error/Error.h"
#include <typeinfo>

namespace Useless {

template< typename AnyLockable, int Ok=0>
class LockObj
{
public:
    LockObj( AnyLockable &lockable ) : _lockable(lockable) 
    {
		if ( _lockable.Lock() != Ok )
		{
			throw Error("LockObj failed %s", typeid(_lockable).name());
		}
    }
    
#define USELESS_LOCK_OBJ_CTOR(n) \
    _TPL_##n LockObj( AnyLockable &lockable, _TPL_ARGS_##n ) : _lockable(lockable) \
    { \
       if ( _lockable.Lock( _TPL_PASS_ARGS_##n ) != Ok ) { throw 0; } \
    }

USELESS_LOCK_OBJ_CTOR(1)
USELESS_LOCK_OBJ_CTOR(2)
USELESS_LOCK_OBJ_CTOR(3)
USELESS_LOCK_OBJ_CTOR(4)
USELESS_LOCK_OBJ_CTOR(5)
USELESS_LOCK_OBJ_CTOR(6)
USELESS_LOCK_OBJ_CTOR(7)
USELESS_LOCK_OBJ_CTOR(8)

        ~LockObj() { _lockable.Unlock(); }
private:
    AnyLockable &_lockable;
};


};//namespace Useless
#endif//__INCLUDED__USELESS_LOCK_OBJ_H__
