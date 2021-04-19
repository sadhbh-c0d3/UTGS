#ifndef __INCLUDED__USELESS__VIRTUAL_VALUE_H__
#define __INCLUDED__USELESS__VIRTUAL_VALUE_H__

#include "Useless/Util/CopyingFactory.h"

namespace Useless {

template< class T >
class VirtualValue
{
public:
    VirtualValue() : _x(0) {}
    VirtualValue( T *p ) : _x(p) {}
	VirtualValue( const T &x ) : _x( &CopyOf<T>(x) ) {}
	VirtualValue( const VirtualValue &v ) : _x(0)
    {
        if (v._x) _x = &CopyOf<T>(*v._x);
    }

	~VirtualValue()
    {
        delete _x;
    }

	VirtualValue& operator = ( const VirtualValue &v )
    {
        if (_x) delete _x;
        if (v._x) _x = &CopyOf<T>(*v._x);
        return *this;
    }

	const T* operator ->() const { return _x; }
	T* operator ->() { return _x; }

	const T& operator *() const { return *_x; }
	T& operator *() { return *_x; }

	operator const T& () const { return *_x; }
	operator T& () { return *_x; }

	bool operator !() const { return !_x; }

	T* get() { return _x; }

private:
	T *_x;
};

};//namespace Useless
#endif//__INCLUDED__USELESS__VIRTUAL_VALUE_H__
