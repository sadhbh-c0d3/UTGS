#ifndef __INCLUDED__USELESS_POINTER_STORAGE_H__
#define __INCLUDED__USELESS_POINTER_STORAGE_H__

#include <vector>
#include <list>
#include "Useless/Util/ReferenceCounter.h"

namespace Useless {

template< class _Storage >
class PointerStorage : private _Storage, public ReferenceCounter
{
public:
    typedef _Storage Storage;
    typedef typename Storage::value_type        Type;
    typedef typename Storage::iterator          Iterator;
    typedef typename Storage::const_iterator    ConstIterator;

	typedef typename Storage::value_type        value_type;
    typedef typename Storage::iterator          iterator;
    typedef typename Storage::const_iterator    const_iterator;

    PointerStorage() {}
    PointerStorage( const Storage &s ): Storage(s) {}
    PointerStorage( const PointerStorage &s ): Storage(s), ReferenceCounter(s) {}

    PointerStorage & operator = ( const PointerStorage &other )
    {
        Dispose();
        Storage::operator=(other);
        ReferenceCounter::operator=(other);
        return *this;
    }

    ~PointerStorage()
    {
        Dispose();
    }

    Storage Release()
    {
        assert( Unique() );
        Storage s = *this;
        clear();
        return s;
    }

    Storage & Lock()
    {
        assert( Unique() );
        return ( *this );
    }
    
    const Storage & Nolock() const
    {
        return ( *this );
    }

    Iterator begin() { return Storage::begin(); }
    Iterator end()   { return Storage::end(); }
    ConstIterator begin() const { return Storage::begin(); }
    ConstIterator end()   const { return Storage::end(); }
    size_t size() const { return Storage::size(); }

private:
    void Dispose()
    {
        if ( Unique() )
        {
            Iterator i = begin();
            Iterator n = end();
            for ( ; i!=n; ++i )
            {
                delete (*i); (*i)=0;
            }
        }
    }
};

template< class T >
struct PtrStorage
{
    typedef PointerStorage< std::list<T*> > List;
    typedef PointerStorage< std::vector<T*> > Vector;
};


template< class _Type >
class InternalPointer : public ReferenceCounter
{
public:
    typedef _Type Type;

    /*! Regular smart pointer (no outer owner)
     */
    InternalPointer( Type *ptr )
        :   _ptr( ptr ), _outer( 0 )
        {}
    
    /*! Pointer to element of outer container, but remember
     *  outer_copy = new PointerStorage<>( outer )
     */
    template< class _PointerStorage >
    InternalPointer( Type *ptr, _PointerStorage &outer_copy )
        :   _ptr( ptr ), _outer( new _PointerStorage( outer_copy ))
        {}

    ~InternalPointer()
    {
        Dispose();
    }

    InternalPointer & operator = ( InternalPointer other )
    {
        Dispose();
        _ptr = other._ptr;
        _outer = other._outer;
        ReferenceCounter::operator=( other );
        return *this;
    }

    Type* operator ->() const { return _ptr; }
    Type& operator *() const { return *_ptr; }

    Type* get() const { return _ptr; }

    Type* Release()
    {
        assert( Unique() );

        delete _storage; _ptr=0; _storage=0;
        _ptr = 0;
    }

private:
    Type             *_ptr;
    ReferenceCounter *_outer;

    void Dispose()
    {
        if ( Unique() )
        {
            if ( 0 != _outer )
                { delete _outer; _outer=0; _ptr=0; }
            else
                { delete _ptr; _ptr=0; }
        }
    }
};


};//namespace Useless
#endif//__INCLUDED__USELESS_POINTER_STORAGE_H__
