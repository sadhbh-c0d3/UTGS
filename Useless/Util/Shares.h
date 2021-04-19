#ifndef __INCLUDED_USELESS_SHARES_H__
#define __INCLUDED_USELESS_SHARES_H__

/*
 * $Id: Shares.h,v 1.8 2002/09/13 16:22:11 koolas Exp $
 *
 *  NAME
 *      Share
 *
 *  PURPOSE
 *      Many objects sharing reference to one common object
 *
 *  NOTE
 *      When reference target is assigned:
 *      1)  old target is REMOVED
 *      2)  all objects know new reference
 *
 *      This object has REFERENCE COUNTER
 *
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
class Shared
{
private:
    friend class Share<T>;

    Shared(): _counter(1), _p_shared_t(0), _array_type(0)  {}
    Shared(T *pt, bool is_array=false): _counter(1), _array_type(is_array), _p_shared_t(pt) {}
    ~Shared()
    {
        if (_array_type) { delete[] _p_shared_t; }
        else             { delete   _p_shared_t; }
    }

       T* GetTarget() const                      { return _p_shared_t; }
    void  SetTarget(T *pt, bool is_array=false)  
    { 
        if (_array_type) { delete[] _p_shared_t; }
        else             { delete   _p_shared_t; }
        _p_shared_t=pt; _array_type=is_array; 
    }
    void ReleaseTarget() { _p_shared_t = NULL;   }

    T* operator->() const { return  _p_shared_t; }
    T& operator *() const { return *_p_shared_t; }

     int AddRef() { return ++_counter; }
     int RemRef() { return _counter>0 ?  --_counter : _counter; }

//////////////
    Shared(const Shared&);
    Shared& operator=(const Shared&);

       T   *_p_shared_t;
     int    _counter;
    bool    _array_type;
};




/*! \ingroup Util
 *  Many objects may be sharing reference to one common object.
 *  The common object is removed automatically when
 *  last reference is being erased.
 *  This class makes it easier to handle.
 */
template< class T >
class Share
{
public:
    Share(): _p_shared(0) { _p_shared = new Shared<T>; assert(_p_shared); }
    Share(const Share &share)
    {
        _p_shared = share._p_shared;
        _p_shared->AddRef();
    }
    Share(T *object) { _p_shared = new Shared<T>; assert(_p_shared); _p_shared->SetTarget(object); }
    ~Share() 
    { 
        if ( _p_shared && !_p_shared->RemRef() ) 
        { 
            delete _p_shared; 
        } 
    }

    T* operator->() const { return _p_shared->GetTarget(); }
    T& operator* () const { return *(*_p_shared); }
    operator bool() const { return !!_p_shared->GetTarget(); }

    Share& operator=(const Share &share)
    {
        if ( _p_shared && !_p_shared->RemRef() ) { delete _p_shared; }
        _p_shared = share._p_shared;
        _p_shared->AddRef();
        return *this;
    }

    bool operator==(const Share &share) const
    {
        return share._p_shared == _p_shared;
    }

       T* GetTarget() const                      { return _p_shared->GetTarget(); }
    void  SetTarget(T *pt, bool is_array=false)  { _p_shared->SetTarget(pt, is_array); }
    void  ReleaseTarget()                        { _p_shared->ReleaseTarget(); }

private:
    Shared<T> *_p_shared;
};



};//namespace Useless
#endif//__INCLUDED_USELESS_SHARES_H__
