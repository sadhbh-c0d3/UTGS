#ifndef __INCLUDED_USELESS_MULTIMETHOD_H__
#define __INCLUDED_USELESS_MULTIMETHOD_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include "Useless/Error/Error.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/SPointer.h"
#include "Useless/System/TypeInfo.h"
#include "Useless/TemplateConfig.h"

namespace Useless {

namespace __MultiMethod {

template< class CalleeBase, class ArgBase >
class Executor
{
public:
    virtual void operator()( CalleeBase&, ArgBase&) = 0;

    typedef SPointer<Executor> Ptr;
};

template< class Callee, class Arg, class Method, class CalleeBase, class ArgBase >
class TheExecutor : public Executor< CalleeBase, ArgBase >
{
public:
    TheExecutor( Method method ): method(method) {}

    virtual void operator()( CalleeBase& callee, ArgBase& arg)
        {
            ( dynamic_cast<Callee&>(callee).*method )( dynamic_cast<Arg&>(arg) );
        }

private:
    Method method;
};

};//namespace __MultiMethod


/*! \ingroup RTTI
 *  Multi-Method mechanism looks at table for method from class
 *  identical to callee and taking argument type identical to passed one.
 *
 *  \warning Mechanism bases on RTTI.
 */
template< class CalleeBase, class ArgBase >
class MultiMethod
{
public:
    template< class Arg, class Callee, class A, class Ret >
    void Add( Ret (Callee::*method)(A), Arg& )
        {
            TypeInfo c(typeid(Callee));
            TypeInfo a(typeid(Arg));
            _executors.Insert( __Key( c, a ),
                new __MultiMethod::TheExecutor<Callee, Arg, Ret (Callee::*)(A), CalleeBase, ArgBase>(method) );
        }

    template< class Callee, class Arg >
    bool operator()( Callee& callee, Arg& arg )
        {
            __Key key( typeid(callee), typeid(arg) );
            __SP_Executor *e = _executors.Find( key );
            if (e) { (**e)( callee, arg ); return true; }
            else return false;
        }

private:
    typedef std::pair< TypeInfo, TypeInfo> __Key;
    typedef USELESS_TYPENAME __MultiMethod::Executor< CalleeBase, ArgBase>::Ptr   __SP_Executor;
    typedef AssocVector< __Key, __SP_Executor > __Executors;
    __Executors    _executors;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_MULTIMETHOD_H__
