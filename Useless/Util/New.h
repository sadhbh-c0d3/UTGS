#ifndef __INCLUDED_USELESS_CREATE_H__
#define __INCLUDED_USELESS_CREATE_H__

#include "Useless/Util/TemplateDefines.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

template< class T > T* New()
{
    return new T;
}

#define __CREATE_MACRO(n) \
template< class T, _TPL_##n##_ > T* New( _TPL_ARGS_##n ) \
{ \
    return new T( _TPL_PASS_ARGS_##n ); \
}

__CREATE_MACRO(1)
__CREATE_MACRO(2)
__CREATE_MACRO(3)
__CREATE_MACRO(4)
__CREATE_MACRO(5)
__CREATE_MACRO(6)
__CREATE_MACRO(7)
__CREATE_MACRO(8)
__CREATE_MACRO(9)
__CREATE_MACRO(10)

#undef __CREATE_MACRO

template< class T > SPointer<T> SPNew() { return new T; }

#define __CREATE_MACRO(n) \
template< class T, _TPL_##n##_ > SPointer<T> SPNew( _TPL_ARGS_##n ) \
{ \
    return new T( _TPL_PASS_ARGS_##n ); \
}

__CREATE_MACRO(1)
__CREATE_MACRO(2)
__CREATE_MACRO(3)
__CREATE_MACRO(4)
__CREATE_MACRO(5)
__CREATE_MACRO(6)
__CREATE_MACRO(7)
__CREATE_MACRO(8)
__CREATE_MACRO(9)
__CREATE_MACRO(10)

#undef __CREATE_MACRO


};//namespace Useless
#endif//__INCLUDED_USELESS_CREATE_H__
