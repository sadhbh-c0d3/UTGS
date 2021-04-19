#ifndef __INCLUDED_USELESS_COM_INITIALIZER_H__
#define __INCLUDED_USELESS_COM_INITIALIZER_H__

#include "Useless/Util/PreInitializer.h"
#include <objbase.h>

namespace Useless {

class USELESS_API COMInitializer
{
    static void Init()    { CoInitialize(NULL);   }
    static void Destroy() { CoUninitialize(); }
    friend class PreInitializer<COMInitializer>;
};

DECLARE_PREINITIALIZER( COMInitializer, COMInitializer, USELESS_API );

};//namespace Useless

#endif//__INCLUDED_USELESS_COM_INITIALIZER_H__
