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
    SortedVector( const std::vector<T> &ref ): StupidVector<T>(ref) { std::sort(this->begin(),this->end()); }

    virtual void Insert( const T& x )
    {
        int i,n; T r=x;
        for( i=0, n=this->size(); i<n;  ++i ) 
        {
            if (this->at(i)<=x ) continue;
            else
            {
                T t = this->at(i);
                this->at(i) = r;
                r = t;
            }
        }
        this->push_back(r);
    }


    const int* FindID( const T &value, StupidVector<T>::ConstIterator *p_iterator = NULL) const
    {
        int n = this->size();
        int p = 0;
        int q = n;

        while( p<q )
        {
            int i = (p+q)/2; T k = this->at(i);
            if ( value == k )
            {
                while( i>0 && this->at(i-1)==value ) {--i;}
                this->_current = i;
                if (p_iterator) (*p_iterator) = this->ConstBegin() + i;
                return &this->_current;
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
        if (p_iterator) (*p_iterator) = this->ConstBegin() + p;
        return NULL;
    }

};

}//namespace Useless
#endif//__INCLUDED_USELESS_SORTED_VECTOR_H__
