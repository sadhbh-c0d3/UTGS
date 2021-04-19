#ifndef __INCLUDED_USELESS_SORTED_VECTOR_H__
#define __INCLUDED_USELESS_SORTED_VECTOR_H__

#include "Useless/Util/Storage/StupidVector.h"
#include <algorithm>

namespace Useless{
/*! \ingroup gStorage
 *  It's vector that is suposed to be allways sorted.
 *  \warning Insertion time is complete-linear
 */
template< class T >
class SortedVector : public StupidVector<T>
{
public:
    SortedVector() {}
    SortedVector( const std::vector<T> &ref ): StupidVector<T>(ref) { std::sort(begin(),end()); }

    virtual void Insert( const T& x )
    {
        int i,n; T r=x;
        for( i=0, n=size(); i<n;  ++i ) 
        {
            if ( at(i)<=x ) continue;
            else
            {
                T t = at(i);
                at(i) = r;
                r = t;
            }
        }
        push_back(r);
    }


    const int* FindID( const T &value, ConstIterator *p_iterator = NULL) const
    {
        int n = size();
        int p = 0;
        int q = n;

        while( p<q )
        {
            int i = (p+q)/2; T k = at(i);
            if ( value == k )
            {
                while( i>0 && at(i-1)==value ) {--i;}
                _current = i;
                if (p_iterator) (*p_iterator) = ConstBegin() + i;
                return &_current;
            }
            else if ( value < k )
            {
                q = (q==i)? q-1 : i;
            }
            else
            {
                p = (p==i)? p+1 : i;
            }
        }
        if (p_iterator) (*p_iterator) = ConstBegin() + p;
        return NULL;
    }

};

}//namespace Useless
#endif//__INCLUDED_USELESS_SORTED_VECTOR_H__
