#ifndef __INCLUDED_USELESS_SPOINTER_H__
#define __INCLUDED_USELESS_SPOINTER_H__

/*
**  $Id: SPointer.h,v 1.19 2003/01/20 20:40:58 koolas Exp $
**
**  NAME
**      SPointer
**
**  PURPOSE
**      Smart pointer template (reference counter).
**
**  TODO
**      - remove #pragma's
**      - change assert's to debug support ASSERT's
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/System/Assert.h"
//#include "SGT/debug.h"

// disable 'forcing value to bool...' warning
#pragma warning(disable:4800)
// disable 'indentifier was truncated...'
#pragma warning(disable:4786)


namespace Useless {

static const struct dynamic_cast_tag_t {} dynamic_cast_tag;

template<class T>
class SPointer
{
/* Templates goes first
 */
public:
    template< class _T1, class _T2 >
        struct static_assignment_t
        {
            static void apply( _T1 *&lhs, _T2 *rhs )
            {
                lhs = rhs;
            }
        };

    template< class _T1, class _T2 >
        struct dynamic_assignment_t
        {
            static void apply( _T1 *&lhs, _T2 *rhs )
            {
                lhs = dynamic_cast< _T1* >( rhs );
            }
        };

    template< class Y >  SPointer( const SPointer<Y> &sp ) : _object(0), _counter(0) { Assign(sp); }
    template< class Y >  SPointer& operator = ( const SPointer<Y> &sp )   { Assign(sp); return *this; }
    template< class Y >  bool operator ==     ( const SPointer<Y> &sp ) const { return get() == sp.get(); }
    template< class Y >  bool operator !=     ( const SPointer<Y> &sp ) const { return get() != sp.get(); }
    template< class Y >  bool operator  <     ( const SPointer<Y> &sp ) const { return get()  < sp.get(); }
    
    template< class Y >  SPointer( const SPointer<Y> &sp, dynamic_cast_tag_t tag ) : _object(0), _counter(0) { DoAssign(sp, dynamic_assignment_t< T, Y >() ); }

private:
    template< class Y >
        void Assign( const SPointer<Y> &spy )
        {
            DoAssign( spy, static_assignment_t< T, Y >() );
        }
    
    // Some kind of msvc template friendship workaround 
    template< class Y, class _A >
        void DoAssign( const SPointer<Y> &spy, _A a )
        {
            // Treat SPointer<Y> as it were SPointer<T> - exact structure (8 bytes)
            const SPointer<T> &sp = reinterpret_cast< const SPointer<T>& >(spy);
            // Tell compiler that sp._object should be treaten as it were type of Y*
            Y *yptr = reinterpret_cast<Y*>(sp._object);
            T *ptr;
            // Cast Y* to T*
            _A::apply( ptr, yptr );
            if ( _object != ptr )
            {
                if ( NULL != _counter )
                {
                    RemRef();
                }
                _counter = sp._counter;
                _object = ptr;
                if ( NULL != _counter )
                {
                    ++(*_counter);
                }
            }
        }

public:
    // Default - no space allocated
    SPointer()               : _object(0), _counter(0) {}
    // Gain ownership over 'object'
    SPointer( T *object )    : _object(0), _counter(0) { Assign(object); }
    // Copy - increase reference count
    SPointer( const SPointer &sp ) : _object(0), _counter(0) { Assign(sp); }
    // Wrap C reference - there will be no reference counting (pure pointer)
    explicit SPointer( T &reference ) : _object( &reference), _counter(0) {}
    // Remove object if owned and reference count reaches 0
    ~SPointer() { if(_counter) { RemRef(); } }

    SPointer& operator=( T *object ) { Assign(object); return *this; }
    SPointer& operator=( const SPointer &sp ) { Assign(sp); return *this; }


    bool operator()() const { return  _object; }
    bool operator !() const { return !_object; }

    T* operator->() { assert(_object); return _object; }
    T& operator* () { assert(_object); return *_object; }

    T const* operator->() const { assert(_object); return _object; }
    const T& operator* () const { assert(_object); return *_object; }

    int  GetCounter() const { return _counter ? *_counter : 0; }
    bool Unique()     const { return _counter ? *_counter==1 : 0; }

    /* Set object and counter pointers to those provided by user
     */
    void Reset( T *object=0, int *counter=0)
    {
        if ( _object)
        {
            if (_object!=object) { if (_counter) RemRef();}
            else {
                assert( Unique());
                delete _counter;
            }
        }
        assert( object || !counter);
        
        if ( object && !counter)
        {
            Assign(object);
        }
        else {
            _counter = counter;
            _object = object;
        }
    }

    /*! Release unique smart pointer.
     *  This method is const, and should only be used
     *  with temporary SPointers.
     */
    T* Release() const
    {   
        assert(Unique());
        T* p=_object;
        delete _counter;
        const_cast<int*&>(_counter)=0;
        const_cast<T*&>(_object)=0;
        return p;
    }

    /* boost::shared_ptr compatibility
    -----------------------------------------------------------------*/
    typedef T element_type;
    T*        get() const { return _object; }
    int use_count() const { return _counter ? *_counter : 0; }
    bool   unique() const { return _counter ? *_counter==1 : true; }
    T*    dispose()       { if(_counter) RemRef();  }

private:
    T   *_object;
    int *_counter;

    void Assign( T *object )
    {
        if(_object) { RemRef(); }
        if(object)
        {
            try {
                _counter = new int;
                *_counter = 1;
            }
            catch(...) {
                delete object;
                throw;
            }
            _object = object;
        }
    }

    void RemRef() 
    { 
        assert(_object);
        assert(_counter);
        if( (*_counter>0) && ((--*_counter)==0) ) 
        {
            delete _object;
            delete _counter; 
        } 
        _object=0;
        _counter=0; 
    }
};


} // namespace Useless

#endif//__INCLUDED_USELESS_SPOINTER_H__

