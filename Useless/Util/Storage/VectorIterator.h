#ifndef __INCLUDED_USELESS_VECTOR_ITERATOR_H__
#define __INCLUDED_USELESS_VECTOR_ITERATOR_H__

#include "Useless/System/Assert.h"

namespace Useless {

//! \ingroup Storage
template< class T >
struct VectorIterator
{
    VectorIterator(): _pointer(0), _first(0), _last(0) {}
    VectorIterator( T *first, T *last): _first(first), _last(last), _pointer(first) {}
    VectorIterator( T *first, int size): _first(first), _last(first+size), _pointer(first) {}

          T& Value()       { assert(!!(*this)); return *_pointer; }
    const T& Value() const { assert(!!(*this)); return *_pointer; }

          T* operator ->()       { assert(!!(*this)); return  _pointer; }
          T& operator * ()       { assert(!!(*this)); return *_pointer; }
    const T& operator * () const { assert(!!(*this)); return *_pointer; }
             operator T*() const { return _pointer; }
           operator int () const { return (_pointer - _first); }
    VectorIterator& operator =(int n) { _pointer = _first + n; return *this; }

    VectorIterator& operator ++()    { ++_pointer; return *this; }
    VectorIterator& operator --()    { --_pointer; return *this; }
    VectorIterator  operator ++(int) { VectorIterator r(*this); ++_pointer; return r; }
    VectorIterator  operator --(int) { VectorIterator r(*this); --_pointer; return r; }
    VectorIterator  operator +(int n) const { VectorIterator r(*this); r+=n; return r; }
    VectorIterator  operator -(int n) const { VectorIterator r(*this); r-=n; return r; }
    bool operator! ()                        const { return !_pointer || _pointer<_first || _pointer>=_last; }
    bool operator==(const VectorIterator &i) const { return _pointer==i._pointer; }
    bool operator< (const VectorIterator &i) const { return _pointer< i._pointer; }
    bool operator<=(const VectorIterator &i) const { return _pointer<=i._pointer; }
    bool operator> (const VectorIterator &i) const { return _pointer> i._pointer; }
    bool operator>=(const VectorIterator &i) const { return _pointer>=i._pointer; }

    VectorIterator& operator+=(int num) { _pointer+=num; return *this; }
    VectorIterator& operator-=(int num) { _pointer-=num; return *this; }


private:
    T *_pointer;
    T *_first;
    T *_last;
};

//! \ingroup Storage
template< class K, class V, class I >
struct AssocIterator
{
    AssocIterator() {}
    AssocIterator(const I &iterator): _iterator(iterator) {}

    AssocIterator& operator ++()    { ++_iterator; return *this; }
    AssocIterator& operator --()    { --_iterator; return *this; }
    AssocIterator  operator ++(int) { AssocIterator r(*this); ++_iterator; return r; }
    AssocIterator  operator --(int) { AssocIterator r(*this); --_iterator; return r; }

    const K&   Key() const { return (*_iterator).first;  }
          V& Value()       { return (*_iterator).second; }
    const V& Value() const { return (*_iterator).second; }

          I& GetIterator()       { return _iterator; }
    const I& GetIterator() const { return _iterator; }


    //! Get associated value
    //@{ 
          V* operator ->()       { return &( (*_iterator).second );}
          V& operator * ()       { return    (*_iterator).second;  }
    const V& operator * () const { return    (*_iterator).second;  }
             operator V*() const { return &( (*_iterator).second );}
    operator int() const { return _iterator.operator int(); }
    AssocIterator& operator+=(int num)     { _iterator+=num; return *this; }
    AssocIterator& operator-=(int num)     { _iterator-=num; return *this; }
    AssocIterator  operator +(int n) const { AssocIterator r(*this); r+=n; return r; }
    AssocIterator  operator -(int n) const { AssocIterator r(*this); r-=n; return r; }
    AssocIterator& operator =(int n)       { _iterator=n; return *this; }
    //@}

    //! Compare lexycographically
    //@{
    bool operator! ()                       const { return !_iterator; }
    bool operator==(const AssocIterator &i) const { return *_iterator == *i._iterator; }
    bool operator< (const AssocIterator &i) const { return *_iterator <  *i._iterator; }
    bool operator<=(const AssocIterator &i) const { return *_iterator <= *i._iterator; }
    bool operator> (const AssocIterator &i) const { return *_iterator >  *i._iterator; }
    bool operator>=(const AssocIterator &i) const { return *_iterator >= *i._iterator; }
    //@}

private:
    I _iterator;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_VECTOR_ITERATOR_H__
