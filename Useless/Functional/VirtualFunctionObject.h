#ifndef __INCLUDED__USELESS_VIRTUAL_FUNCTION_OBJECT_H__
#define __INCLUDED__USELESS_VIRTUAL_FUNCTION_OBJECT_H__

#include "Useless/Functional/PackedFunctor.h"

namespace Useless {

template< class FunctionObjectType, class RetType >
class VirtualFunctionObjectR : public PackedFuncR< RetType >
{
public:
    typedef RetType type;

    VirtualFunctionObjectR( const FunctionObjectType &f ): _f(f) {}

    virtual RetType operator()()
    {
        return _f();
    }

private:
    FunctionObjectType _f;
};

template< class RetType, class FObj >
PkFuncR< RetType > VFObjR( const FObj &f )
{
    return new VirtualFunctionObjectR< FObj, RetType >(f);
}



template< class FunctionObjectType >
class VirtualFunctionObject : public PackedFunc
{
public:
    typedef void type;

    VirtualFunctionObject( const FunctionObjectType &f ): _f(f) {}

    virtual void operator()()
    {
        _f();
    }

private:
    FunctionObjectType _f;
};

template< class FObj >
PkFunc VFObj( const FObj &f )
{
    return new VirtualFunctionObject< FObj >(f);
}

};//namespace Useless
#endif;//__INCLUDED__USELESS_VIRTUAL_FUNCTION_OBJECT_H__
