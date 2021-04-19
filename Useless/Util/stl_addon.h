#ifndef __INCLUDED_USELESS_STL_ADDON_H__
#define __INCLUDED_USELESS_STL_ADDON_H__

#include <map>

/*! This file contains some helper usefull template 
 *  functions and functors to be used with STL
 */

namespace Useless {

/*! Search whole container to check if given iterator lies inside it
 */
template< class Container, class It >
bool is_iterator_valid( const Container &c, const It &it )
{
    typedef typename Container::iterator iterator;
    iterator i=c.begin(), i2=c.end();
    for (; i!=i2; ++i )
    {
        if ( i==it ) { return true; }
    }
    return false;
}

/*! Insert range of values into map container (msvc W/A for STL partial templ. spec.)
 */
template< class Container, class It, class OIt >
void insert( const It &first, const It &last, Container &out, OIt &out_it )
{
    std::copy( first, last, std::insert_iterator<Container>( out, out_it ));
}


//! helper to get value associated with key
template< class _Kty, class  _Ty, class _Pr, class _Alloc >
bool try_get( const std::map< _Kty, _Ty, _Pr, _Alloc > &m, _Kty key, _Ty *result )
{
    std::map< _Kty, _Ty, _Pr, _Alloc >::const_iterator i = m.find( key );
    if ( i != m.end() )
    {
        (*result) = (*i).second;
        return true;
    }
    else
    {
        return false;
    }
}


// helper to get required attribute (throws exception if not found)
template< class T >
const T& get_attribute( const std::map<std::string,T> &m, const std::string &e )
{
    std::map<std::string,T>::const_iterator i=m.find(e), i2=m.end();
    
    if ( i==i2 )
    {
        throw Error("Attribute '%s' was not found", e.c_str());
    }
    return (*i).second;
}

// helper to get attribute if avaliable (returns false if not found)
template< class T1, class T2, typename T3, typename T4 >
bool try_attribute( const std::map<T1,T2> &m, const T3 &e, T4 *x )
{
    std::map<T1,T2>::const_iterator i=m.find(e), i2=m.end();
    
    if ( i!=i2 )
    {
        *x = (*i).second;
        return true;
    }
    return false;
}

// helper to get integer attribute if avaliable (returns false if not found)
template< class T1, class T3 >
bool try_attribute_i( const std::map<T1, std::string > &m, const T3 &e, int *x )
{
    std::map<T1, std::string >::const_iterator i=m.find(e), i2=m.end();
    
    if ( i!=i2 )
    {
        *x = to_integer( (*i).second );
        return true;
    }
    return false;
}

// helper to get hexadecimal attribute if avaliable (returns false if not found)
template< class T1, class T3 >
bool try_attribute_h( const std::map<T1, std::string > &m, const T3 &e, int *x )
{
    std::map<T1, std::string >::const_iterator i=m.find(e), i2=m.end();
    
    if ( i!=i2 )
    {
        *x = to_hex( (*i).second );
        return true;
    }
    return false;
}


// helper to get real attribute if avaliable (returns false if not found)
template< class T1, class T3 >
bool try_attribute_r( const std::map<T1, std::string > &m, const T3 &e, double *x )
{
    std::map<T1, std::string >::const_iterator i=m.find(e), i2=m.end();
    
    if ( i!=i2 )
    {
        *x = to_real( (*i).second );
        return true;
    }
    return false;
}

/*! Functor to be used with for_each accumulationg sum
 */
template< typename N >
struct sum
{
    sum(): _s(0) {}

    void operator ()( N x ) const { _s += x; }
    operator N () const { return _s; }

    mutable N _s;
};

/*! Functor to be used with for_each accumulationg multiplications
 */
template< typename N >
struct multiply
{
    multiply(): _s(0) {}

    void operator ()( N x ) const { _s *= x; }
    operator N () const { return _s; }

    mutable N _s;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_STL_ADDON_H__
