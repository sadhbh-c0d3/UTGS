#ifndef __INCLUDED_USELESS_GARBAGE_COLLECTOR_H__
#define __INCLUDED_USELESS_GARBAGE_COLLECTOR_H__

/*
 *    $Id: GarbageCollector.h,v 1.4 2002/09/13 16:22:11 koolas Exp $
 *
 *    NAME
 *        GarbageCollector
 *
 *    PURPOSE
 *        Generic trash can, used to store garbage
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/Util/Garbage.h"
#include <vector>

namespace Useless {
/*! \ingroup Util
 *  Collects Garbage.
 */

class GarbageCollector
{
public:
    class GarbageID
    {
    public:
        GarbageID(): _id(0), _gc(NULL) {}
        void* Recycle()
        {
            return _gc->Recycle(_id);
        }
        template< class T >
        void Recycle(T **ptr)
        {
            *ptr = (T*)Recycle();
        }

    private:
        GarbageID( int id, GarbageCollector* gc) : _id(id), _gc(gc) {}
        int _id;
        GarbageCollector* _gc;
        friend class GarbageCollector;
    };

    template < class T >
    GarbageID Trash(T *ptr)
    {
        int i=0;
        int id=0;
        int n = _trash.size();
        while( i<n && _trash[i++].first==id ) { ++id; }
        _trash.push_back( std::make_pair( id, MakeGarbage(ptr) ) );
        return GarbageID(id,this);
    }

    void* Recycle(int id)
    {
        size_t i;
        size_t n = _trash.size();
        void *ptr = NULL;
        for (i=0; i!= n; ++i)
        {
            if ( _trash[i].first == id) 
            {
                ptr = _trash[i].second->Recycle();
                break;
            }
        }
        while( ++i < n )
        {
            _trash[i-1] = _trash[i];
        }
        _trash.resize(n-1);
        return ptr;
    }

    void Purify()
    {
        _trash.clear();
    }

private:
    typedef std::vector< std::pair<int, Garbage::Ptr > > TrashCan;
    TrashCan _trash;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_GARBAGE_COLLECTOR_H__
