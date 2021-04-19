/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: FreeListManager.h,v 1.3 2004/11/17 18:17:38 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_FREELIST_MANAGER_H__
#define __INCLUDED_FREELIST_MANAGER_H__

#include <vector>

namespace FreeList {

    struct DummyInitializer
    {
        template< class _T > _T& operator ()( int name, _T &x ) { return x; }
    };
    
    struct DummyDeinitializer
    {
        template< class _T > _T& operator ()( int name, _T &x ) { return x; }
    };

    template< class _Array, class _F >
    struct ArrayInitializerType
    {
        ArrayInitializerType( const _Array &a, _F f ) : _a(a), _f(f) {}
        const _Array &_a;
        _F _f;

        template< class _T > _T& operator ()( int name, _T &x )
        {
            if ( _a )
            {
                _f( x, _a[ name ]);
            }
            return x;
        }
    };

    template< class _T, class _F > ArrayInitializerType< _T, _F >
        ArrayInitializer( const _T &array, _F f )
        {
            return ArrayInitializerType< _T, _F >( array, f );
        }


    template< class _Item >
    struct Manager
    {
        struct BlockInfo
        {
            _Item _item;
            int   _size;
            int   _next;
            int   _refs;
            BlockInfo():_size(1),_next(-1),_refs(0){}
        };

        typedef std::vector< BlockInfo > ItemCollection;
        ItemCollection _items;
        int       _first;
        int       _free;

        Manager(): _first(0), _free(0)
        {
        }

        ~Manager()
        {
        }

        template< class _NameArray, class _Initializer >
        void GenObjects( int count, _NameArray names, _Initializer &init )
        {
            if ( _free < count )
            {
                int distance = (count-_free);
                _items.resize( _items.size() + distance );
                _free += distance;
            }

            // Iterate over unallocated blocks.
            for ( int k=0, i=_first, N=_items.size(); k<count; )
            {
                assert( i < N ); //i must be inside allocation space
                assert( _free > 0 ); //there must be enought free space

                BlockInfo &block = _items[i];

                assert ( 0 == block._refs ); //i is always over free block
                
                block._item = _Item(); //set default value
                init( k, block._item ); //fire user initializer function
                block._refs = 1; //mark block as used
                names[k++] = i; //store block name
                --_free;

                // go to next block
                if ( -1 == block._next ) {
                    ++i;
                }
                else {
                    i = block._next;
                }

                _first = i;
            }
        }
        
        template< class _NameArray >
        void GenObjects( int count, _NameArray names )
        {
            GenObjects( count, names, DummyInitializer() );
        }

        template< class _NameArray, class _Deinitializer >
        void DestroyObjects( int count, _NameArray names, _Deinitializer &deinit )
        {
            assert( count <= _items.size() );

            for ( int i=0; i!=count; ++i )
            {
                int name = names[i];
                names[i] = -1;
                assert( name >= 0 );
                assert( name < _items.size() );

                BlockInfo &block = _items[ name ];                
                
                assert( block._refs > 0 );

                block._refs = 0;
                deinit( name, block._item );
                ++_free;

                if ( _first > name )
                {
                    block._next = _first;
                    _first = name;
                }
                else
                {
                    int k=_first;

                    for ( int j=_first; j < name; )
                    {
                        BlockInfo &block2 = _items[j];
                        k = j;
                        j = block2._next;
                    }

                    BlockInfo &block1 = _items[k];
                    block._next = block1._next;
                    block1._next = name;
                }
            }
        }
        
        template< class _NameArray >
        void DestroyObjects( int count, _NameArray names )
        {
            DestroyObjects( count, names, DummyDeinitializer() );
        }

        int GenObject()
        {
            int name;
            GenObjects( 1, &name );
            return name;
        }

        void DestroyObject( int name )
        {
            DestroyObjects( 1, &name );
        }

        _Item & Lookup( int name )
        {
            assert( name >= 0 );
            assert( name < _items.size() );
            BlockInfo &block = _items[ name ];
            assert( block._refs > 0 );
            return block._item;
        }
        
        const _Item & Lookup( int name ) const
        {
            assert( name >= 0 );
            assert( name < _items.size() );
            const BlockInfo &block = _items[ name ];
            assert( block._refs > 0 );
            return block._item;
        }

        _Item & operator [] ( int name )
        {
            return Lookup( name );
        }

        const _Item & operator [] ( int name ) const
        {
            return Lookup( name );
        }

        bool Exists( int name ) const
        {
            return (name >=0 && name < _items.size());
        }

        int GetName( const _Item &item ) const
        {
            union _helper_union_
            {
                int c;
                _Item BlockInfo::*ptr;
            };

            _helper_union_ u;
            u.ptr = &BlockInfo::_item;
            const char *ptr1 = (const char *)(&item);
            ptr1 -= u.c;
            const BlockInfo *bptr1 = (BlockInfo *)ptr1;
            const BlockInfo *bptr2 = &(_items[0]);

            return bptr1 - bptr2;
        }

        struct iterator : ItemCollection::iterator
        {
            typedef _Item value_type;
            ItemCollection::iterator _end;

            explicit iterator( ItemCollection::iterator i,
                    ItemCollection::iterator end ): ItemCollection::iterator(i), _end(end) {}

            _Item & operator * () const
            {
                assert( !is_end() );
                return get_block()._item;
            }

            iterator & operator ++()
            {
                assert( !is_end() );
                do next(); while ( !is_end() && ( 0 == get_block()._refs ));
                return (*this);
            }
            
            BlockInfo& get_block() const
            {
                return ( ItemCollection::iterator::operator *());
            }

            void next()
            {
                ItemCollection::iterator::operator ++();
            }

            bool is_end() const
            {
                return ( _end == (*this) );
            }

        private:
            iterator & operator --() {};
        };
        
        struct const_iterator : ItemCollection::const_iterator
        {
            typedef _Item value_type;
            ItemCollection::const_iterator _end;

            explicit const_iterator( ItemCollection::const_iterator i,
                    ItemCollection::const_iterator end ): ItemCollection::const_iterator(i), _end(end) {}

            const _Item & operator * () const
            {
                assert( !is_end() );
                return get_block()._item;
            }
            
            const_iterator & operator ++()
            {
                assert( !is_end() );
                do next(); while ( !is_end() && ( 0 == get_block()._refs ));
                return (*this);
            }
            
            const BlockInfo & get_block() const
            {
                return ( ItemCollection::const_iterator::operator *());
            }
            
            void next()
            {
                ItemCollection::const_iterator::operator ++();
            }
            
            bool is_end() const
            {
                return ( _end == (*this) );
            }
        
        private:
            iterator & operator --() {};
        };

        iterator begin() { return iterator( _items.begin(), _items.end() ); }
        iterator end()   { return iterator( _items.end(), _items.end() ); }
        const_iterator begin() const { return const_iterator( _items.begin(), _items.end() ); }
        const_iterator end()   const { return const_iterator( _items.end(), _items.end() ); }

        template< class _F > _F fold( _F f ) { return std::for_each( begin(), end(), f ); }
    };


};//FreeList

#endif//__INCLUDED_FREELIST_MANAGER_H__
