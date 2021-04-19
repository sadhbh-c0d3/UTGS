#ifndef __INCLUDED_USELESS_METHOD_POOL_H__
#define __INCLUDED_USELESS_METHOD_POOL_H__

/*
 *    $Id: MethodPool.h,v 1.6 2003/01/20 20:40:58 koolas Exp $
 *
 *    NAME
 *        MethodPool
 *
 *    PURPOSE
 *        MethodPool selects best method to fit argument type
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/


#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/System/TypeInfo.h"

namespace Useless {

namespace __MethodPool {

template< class Callee, class ArgBase >
class Executor
{
public:
    typedef void (Callee::*MethodBase)(ArgBase&);

    void operator()( Callee &obj, ArgBase &arg) const
    {
        (obj.*_method)(arg);
    }
    
    template< class Method > void Assign(Method m)
    {
        _method = (MethodBase&)m;
    }

    MethodBase _method;
};

};//namespace __MethodPool

/*! \ingroup RTTI
 *  Pool of polymorphic methods. Mechanism selects method
 *  whose argument type is identical to type passed at execution time.
 *  \warning Mechanism bases on RTTI.
 */
template< class Callee, class ArgBase >
class MethodPool
{
public:

    /*! Register new method to Pool */
    template< class Arg, class Ret, class A >
        void Add( Ret (Callee::*method)(A), const Arg& )
        {
            __Executor e;
            e.Assign(method);
            _methods.Insert( typeid(Arg), e );
        }

    /*! Execute best method */
    template< class ACallee, class Arg >
        bool operator()( ACallee& callee, Arg& arg )
        {
            __Executor *e = _methods.Find( typeid(arg) );
            if (e) { (*e)( callee, arg ); return true; }
            else return false;
        }

private:
    typedef __MethodPool::Executor< Callee, ArgBase >   __Executor;
    typedef AssocVector< TypeInfo, __Executor >         __Executors;
                                    __Executors          _methods;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_METHOD_POOL_H__
