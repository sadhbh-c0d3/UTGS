#ifndef __INCLUDED_USELESS_VECTOR_TRAITS_H__
#define __INCLUDED_USELESS_VECTOR_TRAITS_H__


// Forward declarations of structures
namespace Useless
{
    template< class K, class D > class AssocVector;
    template< class T > class AutoVector;
    template< class T > class SortedVector;
    template< class T > class StupidVector;
};
//namespace Useless

namespace CTC {

template< class T > struct VectorTraitsFcn
{
    typedef char (&false_t)[1];
    typedef char (&true_t)[2];
    static T& makeT();
    
    template< class K, class D > static true_t  isVector( ::Useless::AssocVector<K,D> &);
    template< class Y > static true_t  isVector( ::Useless::AutoVector<Y> &);
    template< class Y > static true_t  isVector( ::Useless::SortedVector<Y> &);
    template< class Y > static true_t  isVector( ::Useless::StupidVector<Y> &);
    template< class Y > static false_t isVector(Y&);
};

template< class T > struct VectorTraits
{
    typedef VectorTraitsFcn<T> chk_f;
    enum
    {
        isVector = sizeof( chk_f::isVector( chk_f::makeT())) == sizeof(chk_f::true_t)
    };
};

template< class C >
struct VectorDefaults
{
    typedef typename C::Iterator iterator;
    typedef typename C::ConstIterator const_iterator;

    template< class I >
    static bool is_end( const I _I, const C &_C )
    {
        return ( !_I);
    }

    static typename C::Iterator begin_of( C &_C )
    {
        return _C.Begin();
    }

    static typename C::ConstIterator begin_of( const C &_C )
    {
        return _C.ConstBegin();
    }

    static typename C::Iterator end_of( C &_C )
    {
        return _C.End();
    }

    static typename C::ConstIterator end_of( const C &_C )
    {
        return _C.ConstEnd();
    }
};


};
//namespace CTC


#endif//__INCLUDED_USELESS_VECTOR_TRAITS_H__
