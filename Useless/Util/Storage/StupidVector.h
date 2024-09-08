#ifndef __INCLUDED_USELESS_STUPID_VECTOR_H__
#define __INCLUDED_USELESS_STUPID_VECTOR_H__

#include <vector>
#include "Useless/Error/Error.h"
#include "Useless/Util/Storage/VectorIterator.h"
#include "Useless/System/Assert.h"

namespace Useless {

/*! \ingroup gStorage
 *  Used by \a StupidVector to return result of carrige operations.
 *  Structure holds 'value' as reference to original.
 *  Reference type is used for interface compatibility with Assoc type defined with AssocVector.
*/
template< class T >
class Reference
{
public:
                   Reference(): _pvalue(0) {}
                   Reference( T& value): _pvalue(&value) {}
                    T& Value()       { return  *_pvalue; }
              const T& Value() const { return  *_pvalue; }
           operator bool () const { return !!_pvalue; }
Reference& operator =(const Reference<const T>& a) { _pvalue=(T*)&a.Value(); return *this; }

private:
        T  *_pvalue;
};

/*! \ingroup gStorage
 *  Just a vector with carrige interface.
 *  PHP like vector. It stores internal carrige, and gives each() function.
 */
template< class T >
class StupidVector : public std::vector<T>
{
public:
    StupidVector(): _current(0) {}
    virtual ~StupidVector() {}

    typedef VectorIterator<T>       Iterator;
    typedef VectorIterator<const T> ConstIterator;
    typedef const T ConstT;

    virtual void Insert( const T& val )
    {
        push_back(val);
    }

    T& operator [](int id) 
    {
        T* p = Find(id);
        assert(p);
        return *p;
    }

    T* Find(int id ) const
    {
        if (id < (int)this->size() ) return (T*)&this->at(id);
        else                    return NULL;
    }

    template< class Ty>
    const int* FindID( const Ty &value, ConstIterator *p_iterator = NULL ) const
    {
        int i, n=this->size();
        for ( i=0; i<n; ++i)
        {
            if ( this->at(i) == value )
            {
                _current = i;
                if (p_iterator) (*p_iterator) = ConstBegin() + i;
                return &_current;
            }
        }
        return NULL;
    }

    void Remove( int i) 
    {
        int n=this->size();
        for (; i<n-1; ++i )
        {
            this->at(i) = this->at(i+1);
        }
        if (n>0) { this->resize(n-1); }
    }

    //! Remove current
    void Remove()      
    { 
        int i,n;
        for( i=_current, n=this->size(); i<n-1; ++i )
        {
            this->at(i) = this->at(i+1);
        }
        this->resize(n-1);
    }


    int  Size()  const { return this->size(); }
    bool Empty() const { return this->empty();}
    void Clear()       { this->clear();       }
    void Reserve(long len) { this->reserve(len); }
    void Resize(long len) { this->resize(len); }

    ConstIterator ConstBegin() const
    {
        return this->empty() ? ConstIterator(NULL,0) : ConstIterator( (ConstT*)&*this->begin(), this->size() );
    }

    ConstIterator ConstEnd() const
    {
        return this->empty() ? ConstIterator(NULL,0) : ConstIterator( (ConstT*)&*this->begin(), this->size() ) += this->size();
    }

    Iterator Begin()
    {
        return this->empty() ? Iterator(NULL,0) : Iterator( (T*)&*this->begin(), this->size() );
    }

    Iterator End()
    {
        return this->empty() ? Iterator(NULL,0) : Iterator( (T*)&*this->begin(), this->size() ) += this->size();
    }


///////// PHP style things - maybe it's not a good idea
    typedef Useless::Reference<T> Reference;
    Reference First()
    {
        _current=0;
        if ( !this->empty() )
        { 
            return Reference(this->at(_current) );
        }
        else return Reference();
    }

    Reference Last()
    {
        _current= this->size()-1;
        if ( !this->empty() )
        { 
            return Reference(this->at(_current) );
        }
        else return Reference();
    }

    Reference Each()
    {
        if ( _current >= 0 && _current < this->size() )
        { 
            assert(_current >= 0);
            int k = _current++;
            return Reference(this->at(k) );
        }
        else return Reference();
    }

    Reference EachReversed()
    {
        if ( _current >= 0 && _current < this->size() )
        { 
            int k = _current--;
            return Reference(this->at(k) );
        }
        else return Reference();
    }

    Reference Next()
    {
        if ( _current+1 < this->size() )
        { 
            int k = ++_current;
            return Reference(this->at(k) );
        }
        else return Reference();
    }

    Reference Previous()
    {
        if ( _current > 0 ) 
        { 
            int k = --_current;
            return Reference(this->at(k) );
        }
        else return Reference();
    }

    typedef Useless::Reference<const T> ConstReference;
    ConstReference First() const
    {
        _current=0;
        if ( !this->empty() )
        { 
            return ConstReference(this->at(_current) );
        }
        else return ConstReference();
    }

    ConstReference Last() const
    {
        _current= this->size()-1;
        if ( !this->empty() )
        { 
            return ConstReference(this->at(_current) );
        }
        else return ConstReference();
    }

    ConstReference Each() const
    {
        if ( _current >= 0 && _current < this->size() )
        { 
            assert(_current >= 0);
            int k = _current++;
            return ConstReference(this->at(k) );
        }
        else return ConstReference();
    }

    ConstReference EachReversed() const
    {
        if ( _current >= 0 && _current < this->size() )
        { 
            int k = _current--;
            return ConstReference(this->at(k) );
        }
        else return ConstReference();
    }

    ConstReference Next() const
    {
        if ( _current+1 < this->size() )
        { 
            int k = ++_current;
            return ConstReference(this->at(k) );
        }
        else return ConstReference();
    }

    ConstReference Previous() const
    {
        if ( _current > 0 ) 
        { 
            int k = --_current;
            return ConstReference(this->at(k) );
        }
        else return ConstReference();
    }


    const T& operator [](int id) const
    {
        T* p = Find(id);
        if (!p) throw Error("AssocVector::[](): No such id. Try to use Find().");
        return *p;
    }
   
#   ifndef WIN32
    const T& at(int i) const { return std::vector<T>::operator[](i);}
    T& at(int i) { return std::vector<T>::operator[](i);}
#   endif
    
protected:
    mutable int _current;

};


};//namespace Useless

#endif//__INCLUDED_USELESS_STUPID_VECTOR_H__
