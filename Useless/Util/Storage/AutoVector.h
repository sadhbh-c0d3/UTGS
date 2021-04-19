#ifndef __INCLUDED_USELESS_AUTO_VECTOR_H__
#define __INCLUDED_USELESS_AUTO_VECTOR_H__

#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/CommonReference.h"

namespace Useless {

/*! \ingroup gStorage
 *  Automatic Associative Vector. Inserting value will automatically generate
 *  index, which may be then used to remove value.
 */
template< class T >
class AutoVector : public AssocVector<int, T>
{
public:
    AutoVector() { _self_pointer.SetTarget(this); }
    virtual ~AutoVector() { _self_pointer.SetTarget(NULL); }
    
    class ID 
    {
    public:
        ID(): _id(0) {}
        void Remove() { if(_vecref) _vecref->Remove(_id); _vecref.SetTarget(NULL); }
        operator bool() { return _vecref; }
        operator int () { return _id; }

    private:
        ID(int id, const CommonReference< AutoVector<T> > &vecref): _id(id), _vecref(vecref) {}
        int                              _id;
        CommonReference< AutoVector<T> > _vecref;
        friend class AutoVector<T>;
    };

    ID Insert(const T& t)
    {
        int i=0, n=_data.size();
        int first_free = 0;
        for(; i<n && _data[i].first< first_free; ++i) {}
        for(; i<n && _data[i].first==first_free; ++i) { ++first_free; }
        AssocVector<int,T>::Insert(first_free, t);
        return ID(first_free, _self_pointer);
    }

    AssocVector<int, T>& GetMap() { return *this; }

private:
    CommonReference< AutoVector<T> > _self_pointer;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_AUTO_VECTOR_H__
