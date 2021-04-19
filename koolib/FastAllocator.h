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
last mod: $Id: FastAllocator.h,v 1.1 2005/03/08 17:27:27 koolas Exp $

 ********************************************************************/

#include <deque>

namespace Memory {

    // Generic fast object allocator
    template< class _Type >
        struct FastAllocator
        {
            template< class __Type, const int _Size >
                struct Page
                {
                    struct Data
                    {
                        char _bytes[ sizeof( __Type ) ];
                        __Type *Construct()
                        {
                            return new (_bytes) __Type();
                        }
                    };
                    Data    _data[ _Size ];
                    size_t  _size;
                    Page   *_prev;
                    Page   *_next;
                    Page(): _size(0), _prev(0), _next(0) {}
                    ~Page() { delete _next; }

                    __Type *GetNew( Page **pLastPage )
                    {
                        if ( _size < _Size )
                        {
                            return (_data + _size++)->Construct();
                        }
                        else
                        {
                            _next = new Page();
                            _next->_prev = this;
                            *pLastPage = _next;
                            return _next->GetNew( pLastPage );
                        }
                    }
                };

            enum { PageSize = 64 }; 
            typedef Page< _Type, PageSize > PageType;
            PageType *_firstPage;
            PageType *_lastPage;
            std::deque< typename PageType::Data * > _free;

            FastAllocator()
            {
                _lastPage = _firstPage = new PageType();
            }

            ~FastAllocator()
            {
                delete _firstPage;
            }

            _Type * GetNew()
            {
                if ( !_free.empty() )
                {
                    _Type *ptr = _free.front()->Construct();
                    _free.pop_front();
                    return ptr;
                }
                else
                {
                    return _lastPage->GetNew( &_lastPage );
                }
            }

            void Return( _Type *ptr )
            {
                ptr->~_Type();
                _free.push_back( reinterpret_cast< PageType::Data *>( ptr ));
            }

            static FastAllocator &Instance()
            {
                static FastAllocator s_FA;
                return s_FA;
            }
        };

    template< class _Type >
        struct FastObject : _Type
        {
            void Release()
            {
                FastAllocator< FastObject< _Type > >::Instance().Return( this );
            }
        };

    template< class _Type > _Type * FastAllocate()
    {
#ifdef NDEBUG
        return FastAllocator< FastObject< _Type > >::Instance().GetNew();
#else
#pragma message("warning: Debug version does NOT use FastAllocator (some performance problems)")
        return new _Type();
#endif
    }
    
};//Memory
    

