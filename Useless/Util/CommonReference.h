#ifndef __INCLUDED_USELESS_COMMON_REFERENCE_H__
#define __INCLUDED_USELESS_COMMON_REFERENCE_H__

/*
 * $Id: CommonReference.h,v 1.8 2002/09/13 16:22:11 koolas Exp $
 *
 *  NAME
 *      CommonReference
 *
 *  PURPOSE
 *      Many objects sharing reference to one common object
 *
 *  NOTE
 *      When reference target is assigned:
 *      1)  old target is NOT REMOVED
 *      2)  all objects know new reference
 *
 *      This object has REFERENCE COUNTER
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *
*/ 

#include "Useless/System/Assert.h"

namespace Useless {

template< class T > class Share;
template< class T >
class CommonObject
{
public:
    CommonObject():      _p_shared_t(0), _counter(1)  {}
    CommonObject(T *pt): _p_shared_t(pt), _counter(1) {}
    ~CommonObject() {}

       T* GetTarget() const  { return _p_shared_t; }
    void  SetTarget(T *pt)   { _p_shared_t=pt;     }

    T* operator->() const { return  _p_shared_t; }
    T& operator *() const { return *_p_shared_t; }

     int AddRef() { return ++_counter; }
     int RemRef() { return _counter>0 ?  --_counter : _counter; }

private:
      T *_p_shared_t;
    int  _counter;
};



/*! \ingroup Util 
 *  Many objects may be sharing reference to one common object.
 *  This class makes it easier to handle.
 */
template< class T >
class CommonReference
{
public:
    CommonReference(): _p_shared(0) { _p_shared = new CommonObject<T>; assert(_p_shared); }
    CommonReference(const CommonReference &share)
    {
        _p_shared = share._p_shared;
        _p_shared->AddRef();
    }
    CommonReference(T *object) { _p_shared = new CommonObject<T>; assert(_p_shared); _p_shared->SetTarget(object); }
    ~CommonReference()              
    {  
        if ( _p_shared && !_p_shared->RemRef() ) 
        { 
            delete _p_shared; 
        } 
    } 

    T* operator->() const { return _p_shared->GetTarget(); }
    T& operator* () const { return *(*_p_shared); }
    operator bool() const { return !!_p_shared->GetTarget(); }

    CommonReference& operator=(const CommonReference &share)
    {
        if ( _p_shared && !_p_shared->RemRef() ) { delete _p_shared; }
        _p_shared = share._p_shared;
        _p_shared->AddRef();
        return *this;
    }

    bool operator==(const CommonReference &share) const
    {
        return share._p_shared == _p_shared;
    }

       T* GetTarget() const { return _p_shared->GetTarget(); }
    void  SetTarget(T *pt)  { _p_shared->SetTarget(pt); }

private:
    CommonObject<T> *_p_shared;
};



};//namespace Useless
#endif//__INCLUDED_USELESS_COMMON_REFERENCE_H__
