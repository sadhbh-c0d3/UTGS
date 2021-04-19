#ifndef __INCLUDED_USELESS_COPYING_FACTORY_H__
#define __INCLUDED_USELESS_COPYING_FACTORY_H__

/*! To register polymorphic type copier use macro below.
 */
#define REGISTER_COPIER( Namspc, BaseType, Type ) \
static Useless::RegisterCopier< Namspc::BaseType, Namspc::Type > Namspc##_##Type##_Is_Registered_##BaseType;

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include "Useless/ErrorConfig.h"
#include "Useless/System/TypeInfo.h"
#include "Useless/Util/StaticCheck.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Util/CTC.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/PreInitializer.h"

namespace Useless {

template< class BaseT, class T >
class CopyingCreator
{
public:
    static BaseT* Copy(const BaseT& t)
    {
        const T& ref = dynamic_cast<const T&>(t);
        return new T( ref );
    }
};

template< class BaseT >
class CopyingFactory
{
public:
    template< class T > 
    static BaseT* CreateCopy(const T &t) { return CopyingCreator<BaseT,T>::Copy(t); }
};

template< class BaseT, class T >
class RTCopyingCreator
{
public:
    static void* Copy(const void *pinstance)
    {
        BaseT *p_zero = (BaseT *)pinstance;
        T* ptr = dynamic_cast<T*>( p_zero );
        if (!ptr) return 0;
        T* pcp = new T(*ptr);
        BaseT *p_ret = pcp;
        return p_ret;
    }
};

class USELESS_API RTCopyingFactory
{
public:
    typedef std::pair< TypeInfo, TypeInfo > __Key;
    typedef void* (*__Creator)(const void*);
    typedef AssocVector< __Key, __Creator > __Creators;

    static void Init() { _creators = new __Creators; }
    static void Destroy() { delete _creators; }
    static void Register( __Key, __Creator );

    template< class BaseT >
    static BaseT* CreateCopy(const BaseT &t)
    { 
        __Key key( typeid(BaseT),typeid(t) );
        __Creators::Iterator it;
        __Creator *creator = (*_creators).Find( key, (__Creators::ConstIterator*)&it );
        BaseT *p_ret = 0;
        for(; !p_ret && !!it && it.Key()==key; ++it )
        {
            creator = &it.Value();
            p_ret = (BaseT*)(*creator)( (const void*)(&t) );
        }
        if (!p_ret)
        {
            throw Error("RTCopyingFactory::CreateCopy(): Creating type %s from %s wasn't registered",
                key.second.Name(), key.first.Name() );
        }
        return p_ret;
    }

private:
    static __Creators *_creators;
};

DECLARE_PREINITIALIZER( RTCopyingFactory, RTCopyingFactory, USELESS_API );



template< class BaseT, class T >
struct RegisterCopier
{
    RegisterCopier()
    {
        RTCopyingFactory::Register(
            RTCopyingFactory::__Key( typeid(BaseT), typeid(T) ),
            &Useless::RTCopyingCreator<BaseT,T>::Copy );
    }
};

/*! \defgroup RTTI Type Detection 
 *  \ingroup Util
 */

/*! \ingroup RTTI
 *  Create copy of polymorphic object.
 *  Before use, you must register RTTI of all copyable classes.
 *  To do this use: REGISTER_COPIER( namespace, type_name).
 */
template< class BaseT, class PtrT = BaseT* >
class CopyOf
{
public:
    template< class T >
    CopyOf( const T &t )
    {
        if ( typeid(T)==typeid(t) ) 
            { pt = CopyingFactory< BaseT >::CreateCopy( t ); }
        else 
            { pt = RTCopyingFactory::CreateCopy(t); }
    }

    CopyOf( const BaseT& t)
    {
        pt = RTCopyingFactory::CreateCopy(t);
    }

    CopyOf( const SPointer<BaseT> &sp_t )
    {
        pt = sp_t.Release();
    }

    virtual ~CopyOf() {}
    operator BaseT&() { return *pt; }
    PtrT operator &() { return  pt; }

private:
    PtrT pt;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_COPYING_FACTORY_H__
