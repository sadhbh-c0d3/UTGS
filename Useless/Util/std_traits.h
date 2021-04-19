#ifndef __INCLUDED_USELESS_STD_TRAITS_H__
#define __INCLUDED_USELESS_STD_TRAITS_H__

// Forward declarations of std structures
namespace std
{
    template< class T, class A > class vector;
    template< class T, class A > class list;
    template< class K, class C, class A > class set;
    template< class K, class D, class C, class A > class map;
};

namespace CTC {

template< class T > struct std_traits_fcn
{
    typedef char (&false_t)[1];
    typedef char (&true_t)[2];
    static T& makeT();
    
    template< class Y, class A > static true_t  is_std( ::std::vector<Y,A> &);
    template< class Y, class A > static true_t  is_std( ::std::list<Y,A> &);
    template< class K, class C, class A > static true_t  is_std( ::std::set<K,C,A> &);
    template< class K, class D, class C, class A > static true_t  is_std( ::std::map<K,D,C,A> &);
    template< class Y > static false_t is_std(Y&);
};

template< class T > struct std_traits
{
    typedef std_traits_fcn<T> chk_f;
    enum
    {
        is_std = sizeof( chk_f::is_std( chk_f::makeT())) == sizeof(chk_f::true_t)
    };
};

template< class C >
struct std_defaults
{
    typedef typename C::iterator iterator;
    typedef typename C::const_iterator const_iterator;

    template< class I >
    static bool is_end( const I _I, const C &_C )
    {
        return ( _C.end() == _I);
    }

    static typename C::iterator begin_of( C &_C )
    {
        return _C.begin();
    }

    static typename C::const_iterator begin_of( const C &_C )
    {
        return _C.begin();
    }

    static typename C::iterator end_of( C &_C )
    {
        return _C.end();
    }

    static typename C::iterator end_of( const C &_C )
    {
        return _C.end();
    }
};

};//namespace CTC

#endif//__INCLUDED_USELESS_STD_TRAITS_H__
