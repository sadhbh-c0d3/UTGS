#ifndef __INCLUDED__USELESS_SAFE_PTR_H__
#define __INCLUDED__USELESS_SAFE_PTR_H__

namespace Useless {

/*! SafePtr is temporary owner of the object.
 *  May be used to preserve safety of the creation
 *  of several objects at the same instuction.
 */   
template< class T > class SafePtr
{
public:    
    typedef T type;
    SafePtr( T *p ): _p(p) {}
    ~SafePtr() { delete _p; }

    operator T*()
    {
        T *p = _p;
        _p = 0;
        return p;
    }
    
private:
    T *_p;

#ifdef WIN32
    SafePtr( const SafePtr &p );
#endif
};
    
};//namespace Useless 
#endif//__INCLUDED__USELESS_SAFE_PTR_H__

