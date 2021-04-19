#ifndef __INCLUDED_KOOLIB_STD_UTIL_H__
#define __INCLUDED_KOOLIB_STD_UTIL_H__

#include <map>
#include <assert.h>

namespace StdUtil {

    template< class _Map >

        struct MapIteratorProxy
        {
            typedef _Map Map;
            typedef typename Map::key_type Key;
            typedef typename Map::iterator Iterator;
            Iterator &_it;
            Key       _name;
            MapIteratorProxy( const Key &name, Iterator &it ): _name(name), _it(it) {}
        };



    template< class _Key, class _Val, class _Comp, class _Alloc >

        MapIteratorProxy< std::map< _Key, _Val, _Comp, _Alloc > >

        map_iterator_proxy(
                const _Key &name,
                std::map< _Key, _Val, _Comp, _Alloc >::iterator &it
                )
        {
            return MapIteratorProxy< std::map< _Key, _Val, _Comp, , _Alloc > >( name, it );
        }



    template< class _Key, class _Val, class _Comp, class _Alloc >

        bool 

        map_find_iterator(
                std::map< _Key, _Val, _Comp, _Alloc > &m,
                MapIteratorProxy< std::map< _Key, _Val, _Comp, _Alloc > > &p
                )
        {
            p._it = m.find( p._name );
            return ( p._it != m.end() );
        }
    
    template< class _Key, class _Val, class _Comp, class _Alloc >

        bool 

        operator >> (
                std::map< _Key, _Val, _Comp, _Alloc > &m,
                MapIteratorProxy< std::map< _Key, _Val, _Comp, _Alloc > > &p
                )
        {
            p._it = m.find( p._name );
            if ( p._it != m.end() )
            {
                return true;
            }
            else
            {
                std::memset( &p._it, 0x0, sizeof( MapIteratorProxy< std::map< _Key, _Val, _Comp, _Alloc > >::Iterator ));
                return false;
            }
        }



    template< class _Map >

        struct DictIter
        {
            typedef _Map                          Map;
            typedef typename _Map::iterator       Iterator;
            typedef typename _Map::key_type       Key;
            typedef typename _Map::value_type     Value;
            typedef typename Value::second_type   Data;
            typedef typename _Map::key_compare    Comp;
            typedef typename _Map::allocator_type Alloc;
            Iterator _it;

            Data * operator ->() { assert(!IsInvalid()); return &(*_it).second; }
            Data * operator ->() const { assert(!IsInvalid()); return &(*_it).second; }

            const Key & operator () () const { assert(!IsInvalid()); return (*_it).first; }

            MapIteratorProxy< _Map >
                operator () ( const Key &key ) const { return MapIteratorProxy< _Map >( key, const_cast< Iterator &>( _it )); }

            const Value & operator * () const { assert(!IsInvalid()); return (*_it); }
            Value & operator * () { assert(!IsInvalid()); return (*_it); }
            DictIter & operator ++ () { assert(!IsInvalid()); ++_it; return (*this); }
            DictIter & operator -- () { assert(!IsInvalid()); --_it; return (*this); }

            DictIter() { SetInvalid(); }
            DictIter( const Iterator &it ): _it(it) {}
            DictIter & operator = ( const Iterator &it ) { _it = it; return (*this); }

            bool operator != ( const DictIter &it ) const
            {
                return (_it != it._it);
            }

            bool operator == ( const DictIter &it ) const
            {
                return !(*this != it );
            }

            void SetInvalid() { std::memset( &_it, 0x0, sizeof( Iterator )); }
            bool IsInvalid () const { return (0L == reinterpret_cast< const int & >( _it )); }
        };

};//StdUtil

#endif//__INCLUDED_KOOLIB_STD_UTIL_H__
