#ifndef __INCLUDED__USELESS_STD_ASSOC_VECTOR_H__
#define __INCLUDED__USELESS_STD_ASSOC_VECTOR_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include <vector>
#include <algorithm>
#include "Useless/System/Assert.h"

namespace std {

/*! Just particular std::assoc_vector wich should act like std::map or even std::multimap
 *  The only difference it is implemented on std::vector (rather than std::set)
 *  This structure is supposed to be more efficient (than std::set) in memory space
 *  The complexity of [] operation remains the same O(log(n))
 */
template< class K, class T >
class assoc_vector : private std::vector< std::pair<K,T> >
{
public:
    typedef std::pair<K,T> __assoc;
    typedef std::vector<__assoc> __vector;

    assoc_vector(): _was_sorted(true) 
    {}
    assoc_vector( const assoc_vector &av ): __vector(av), _was_sorted(av._was_sorted) 
    {}
    assoc_vector( const __vector &v ): __vector(v)
    {
        std::sort( begin(), end(), &assoc_vector::less_key );
        _was_sorted = true;
    }

    void insert( const __assoc &p ) 
    {
        push_back(p);
        _was_sorted = false;
    }

    void check_and_prepare()
    {
        if (!_was_sorted) 
        {
            std::sort( begin(), end() , &assoc_vector::less_key );
            _was_sorted = true;
        }
    }

    T& operator[](K n)
    {
        iterator i = begin();
        check_and_prepare();
        i = std::lower_bound( begin(), end(), __assoc(n,T()), &assoc_vector::less_key );
        if ( i==end() || (*i).first!=n )
        {
            push_back( __assoc(n,T()) ); _was_sorted=false;
            i = end(); --i;
        }
        return (*i).second;
    }

    T operator[](K n) const
    {
        iterator i = begin();
        check_and_prepare();
        i = std::lower_bound( begin(), end(), __assoc(n,T()), &assoc_vector::less_key );
        return ( i!=end() && (*i).first==n )? (*i).second : T();
    }

    const T& at(K n) const
    {
        iterator i = begin();
        check_and_prepare();
        i = std::lower_bound( begin(), end(), __assoc(n,T()), &assoc_vector::less_key );
        assert( i!=end() && (*i).first==n );
        return (*i).second;
    }

    __vector::iterator lower_bound(K n)
    {
        check_and_prepare();
        return std::lower_bound( begin(), end(), __assoc(n,T()), &assoc_vector::less_key );
    }
    __vector::iterator upper_bound(K n)
    {
        check_and_prepare();
        return std::upper_bound( begin(), end(), __assoc(n,T()), &assoc_vector::less_key );
    }

    __vector::iterator find(K n)
    {
        iterator i = lower_bound(n);
        return ( i==end() || (*i).first!=n )? end() : i;
    }

    __vector::clear;
    __vector::erase;
    __vector::begin;
    __vector::end;
    __vector::rbegin;
    __vector::rend;
    __vector::size;
    __vector::empty;
    typedef __vector::iterator iterator;
    typedef __vector::const_iterator const_iterator;

private:
    static bool less_key(const __assoc &lhs, const __assoc &rhs )
    {
        return (lhs.first < rhs.first)? 1 : 0;
    }
    bool _was_sorted;
};


};//namespace std
#endif//__INCLUDED__USELESS_STD_ASSOC_VECTOR_H__
