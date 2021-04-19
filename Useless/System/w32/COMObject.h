#ifndef __INCLUDED_USELESS_COM_OBJECT_H__
#define __INCLUDED_USELESS_COM_OBJECT_H__

#include <assert.h>

namespace Useless {

template < class COMT > struct COMObject
{
    COMObject(): _ptr(0) {}
    
    /*! Use existing reference to existing COM object */
    COMObject( COMT *ptr, bool add_ref=false ): _ptr(ptr)
    {
        if ( add_ref) { _ptr->AddRef(); }
    }

    /*! Query interface to existing COM object */
    template< class T >
    COMObject( const COMObject<T> &c )
    {
        c.QueryInterface( *this );
    }

    /*! Add new reference to existing COMObject */
    USELESS_CONSTRUCTOR_SPECIALIZATION
    COMObject( const COMObject &c )
    { 
        _ptr = c._ptr;
        if (_ptr) _ptr->AddRef(); 
    }

    /*! Create instance of COM object */
    COMObject( REFCLSID clsid, DWORD context, LPUNKNOWN outer=NULL ): _ptr(0)
    {
        Create( clsid, context, outer );
    }

    /*! Remove one reference to existing COM object */
    ~COMObject()
    {
        Release();
    }

    /*! Remove one reference to existing COM object */
    void Release()
    {
        if (_ptr) { _ptr->Release(); _ptr=0; }
    }

    /*! Create instance of COM object */
    void Create( REFCLSID clsid, DWORD context, LPUNKNOWN outer=NULL )
    {
        assert( !_ptr );
        CoCreateInstance( clsid, outer, context,__uuidof(COMT), (void **)&_ptr);
    }
   
    /*! operator -> for pointer sybtax */
    COMT* operator->() const { return  _ptr; }
    /*! operator * for pointer sybtax */
    COMT& operator *() const { return *_ptr; }
    
	template< class T >
	bool QueryInterface( COMObject<T> &where ) const
	{
		assert( _ptr );
		return _ptr->QueryInterface(__uuidof(T), where);
	}

    /*! Query interface to existing COM object */
    template< class T >
    COMObject<COMT>& operator =( const COMObject<T> &c )
    {
        Release();
        c.QueryInterface( *this );
        return *this;
    }

    /*! Switch reference to other existing COM object */
	USELESS_CONSTRUCTOR_SPECIALIZATION
    COMObject<COMT>& operator =( const COMObject<COMT> &c )
    {
        Release();
        _ptr = c._ptr;
        if (_ptr) _ptr->AddRef();
        return *this;
    }

    /*! Assign new reference to existing COM object */
    void Assign( COMT *ptr )
    {
        Release();
        _ptr = ptr;
        _ptr->AddRef();
    }

    bool operator ==( const COMObject<COMT> &c ) const
    {
        return c._ptr == _ptr;
    }

    COMT** operator &()
    {   
        assert( !_ptr );
        return &_ptr;
    }

    operator void**()
    {
        assert( !_ptr );
        return (void**)&_ptr;
    }

    bool operator !() const
    {
        return !_ptr;
    }

    REFIID Id() const { return __uuidof(COMT); }

    COMT* get() const { return _ptr; } //small letter for std compatibility

    typedef COMT * Ptr;

private:
    COMT *_ptr;
};


/*! COM_cast may be used instead of annoying IUnknown::QueryInterface
 @{*/
template< class T, class Q >
COMObject<T> COM_cast( Q *q )
{
    T *p;
    q->QueryInterface( __uuidof(T), (void**)&p);
    return p;
}

template< class T, class Q >
COMObject<T> COM_cast( const COMObject<Q> &q )
{
    return COM_cast<T>( q.get());
}

/*@}*/

};//namespace Useless

#endif//__INCLUDED_USELESS_COM_OBJECT_H__
