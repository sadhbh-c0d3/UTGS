#ifndef __INCLUDED_USELESS_ASSOC_VECTOR_H__
#define __INCLUDED_USELESS_ASSOC_VECTOR_H__

#include <vector>
#include "Useless/System/Assert.h"
#include "Useless/Error/Error.h"
#include "Useless/Util/Storage/VectorIterator.h"
#include <algorithm>

namespace Useless {

/*! \defgroup gStorage Storage 
 *  \ingroup Util 
 */


/*! \ingroup gStorage
 *  Associative vector with internal carrige.
 *  PHP style: list($key,$value) = each($table);
 */
template< class T_ID, class T_VALUE >
class AssocVector
{
    typedef std::pair< T_ID, T_VALUE >             __Pair;
    typedef std::vector< __Pair >                  __Data;
    typedef std::pair< const T_ID, T_VALUE >       __AssocPair;
    typedef std::pair< const T_ID, const T_VALUE > __ConstPair;
    typedef std::vector< __ConstPair >             __ConstData;

public:
    AssocVector() {}
    virtual ~AssocVector() {}

    typedef  VectorIterator<__ConstPair>                                    ConstVectorIterator;
    typedef  AssocIterator<const T_ID, const T_VALUE, ConstVectorIterator > ConstIterator;

    typedef  VectorIterator<__AssocPair>                        VectorIterator;
    typedef  AssocIterator<T_ID, T_VALUE, VectorIterator >      Iterator;


    AssocVector( const __Data &ref ): _data(ref) { std::sort(begin(),end()); }
    operator const std::vector<T_VALUE>& () { return _data; }

    //! Insert association: id => val
    void Insert( const T_ID& id, const T_VALUE& val )
    {
        ConstIterator i; Find(id, &i);
        while( !!i && i.Key()==id ) { ++i; }

        AddSpace(i);
        _data[i] = __Pair(id,val);
    }

    //! Find record with spec. id and remove it
    void Remove( const T_ID& id)
    {
        int i,n;
        for( i=0, n=_data.size(); i<n && _data[i].first!=id; ++i ) {}
        if (i==n) return;
        for( ; i<n-1; ++i )
        {
            _data[i] = _data[i+1];
        }
        _data.resize(n-1);
    }

    //! Remove element at spec. position
    void RemoveAt(int position)
    { 
        int i,n;
        for( i=position, n=_data.size(); i<n-1; ++i )
        {
            _data[i] = _data[i+1];
        }
        _data.resize(n-1);
    }

    /*! Just find record with spec. id 
     *  Uses bunary search method
     *  Method return first entry with ID=id or
     *  if nothing was found (*p_iterator) is set just after place 
     *  where element with ID=id should be placed
     */
    T_VALUE* Find(const T_ID &id, ConstIterator *p_iterator=NULL ) const
    {
        int p = 0;
        int q = _data.size();

        while( p<q )
        {
            int i = (p+q)/2;
            T_ID k = _data[i].first;
            if ( id == k )
            {
                while( i>0 && _data[i-1].first == id ) {--i;}
                if (p_iterator) (*p_iterator) = ConstBegin() + i;
                return (T_VALUE*)&(_data[i].second);
            }
            else if ( id < k )
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
    
    T_VALUE* Find(const T_ID &id, Iterator *p_iterator )
    {
        return Find( id, (ConstIterator*)p_iterator);
    }

    //! Search vector by value
    template< class T_VAL>
    const T_ID* FindID(const T_VAL& value, ConstIterator *p_iterator = NULL ) const
    {
        for ( int i=0, n = _data.size(); i<n; ++i)
        {
            if ( _data[i].second == value )
            {
                if (p_iterator) (*p_iterator) = ConstBegin() + i;
                return &(_data[i].first);
            }
        }
        if (p_iterator) (*p_iterator) = ConstEnd();
        return NULL;
    }
    
    template< class T_VAL>
    const T_ID* FindID(const T_VAL& value, Iterator *p_iterator )
    {
        return FindID( value, (ConstIterator*)p_iterator);
    }

    int  Size()  const { return _data.size(); }
    bool Empty() const { return _data.empty();}
    void Clear()       { _data.clear();       }

    /*! Find record with spec. id.
     *  If it doesn't exist create one.
     */
    T_VALUE& operator [](const T_ID &id) 
    {
        T_VALUE* p = Find(id);
        if (!p)
        {
            Insert(id, T_VALUE() );
            p = Find(id);
        }
        return *p;
    }
    
    /*! Find record with spec. id.
     * \attention If it doesn't exist throw exception.
     */
    const T_VALUE& operator [](const T_ID &id) const
    {
        T_VALUE* p = Find(id);
        if (!p) throw Error("AssocVector::[](): No such id. Try to use Find().");
        return *p;
    }

    ConstIterator ConstBegin() const
    {
        return (_data.empty() ? 
                ConstVectorIterator(NULL,0) :
                ConstVectorIterator( (__ConstPair*)(&_data[0]), _data.size() )
                );
    }

    ConstIterator ConstEnd() const
    {
        return (_data.empty() ? 
                ConstVectorIterator(NULL,0) :
                ConstVectorIterator( (__ConstPair*)(&_data[0]), _data.size() ) += _data.size()
                );
    }

    Iterator Begin()
    {
        return (_data.empty() ? 
                VectorIterator(NULL, 0) :
                VectorIterator( (__AssocPair*)(&_data[0]), _data.size() )
               );
    }

    Iterator End()
    {
        return (_data.empty() ? 
                VectorIterator(NULL, 0) :
                VectorIterator( (__AssocPair*)(&_data[0]), _data.size() ) += _data.size()
                );
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    /* Direct array access GetKey 
    */
    //@{
    T_ID GetKey(int i) const
    {
        assert( i>=0 && i<_data.size() );
        return _data[i].first;
    }

    T_VALUE& GetValue(int i)
    {
        assert( i>=0 && i<_data.size() );
        return _data[i].second;
    }

    void SetValue(int i, const T_VALUE &val)
    {
        assert( i>=0 && i<_data.size() );
        _data[i].second = val;
    }

    const T_VALUE& GetValue(int i) const
    {
        assert( i>=0 && i<_data.size() );
        return _data[i].second;
    }

    int GetNumber( const T_ID &id) const
    {
        ConstIterator i; Find(id,&i); return i;
    }

    template< class T_VAL>
    int GetNumberByValue( const T_VAL &value) const
    {
        ConstIterator i; FindID(value,&i); return i;
    }

    /*! Function inserts entry before element that is at position. If position==Size() then element is put at end.
     */
    void HardInsert( int position, const T_VALUE &value)
    {
        T_ID id = ( position>0 )? _data[position-1].first : ( position<_data.size() )? _data[position+1].first : T_ID();
        AddSpace(position);
        _data[position] = __Pair( id, value);
    }

    /*! Function removes entry at position, by moving all following entries one-step-back
     */
    void HardRemove( int position )
    {
        int n=_data.size(); assert( position>=0 && position<n );
       
        while( ++position < n )
        {
            _data[position-1] = _data[position];
        }
        _data.resize(n-1);
    }

    /*! Function adds one gap in vector by moving all following entries one-step-forward
     */
    void AddSpace( int position )
    {
        int n=_data.size(); assert( position>=0 && position<=n );

        if (n>0) { _data.push_back( _data.back() ); }
        else     { _data.push_back( __Pair() ); }
        
        while( position < --n )
        {
            _data[n] = _data[n-1];
        }      
    }

    /*! Function moves element from_position to_position. Key of the element is subject to change */
    void MoveElement( int from_position, int to_position )
    {
        int n=_data.size(); assert( from_position>=0 && from_position<n && to_position>=0 && to_position<n );
        T_VALUE val = _data[from_position].second;

        if ( from_position < to_position )
        {
            while ( from_position <= to_position && from_position<n-1 )
            {_data[from_position] = _data[from_position+1]; ++from_position; }
        }
        else
        {
            while ( to_position <= from_position && from_position>0 )
            {_data[from_position] = _data[from_position-1]; --from_position;}
        }
        _data[to_position].second = val;
    }
    //@}

protected:
    __Data _data;
};


};//namespace Useless

#endif//__INCLUDED_USELESS_ASSOC_VECTOR_H__
