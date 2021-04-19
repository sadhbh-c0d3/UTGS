#ifndef __INCLUDED__USELESS__REFERENCE_COUNTER_H__
#define __INCLUDED__USELESS__REFERENCE_COUNTER_H__

#include "Useless/Error/Error.h"

namespace Useless {

class ReferenceCounter
{
public:
    ReferenceCounter(bool do_reset=true): _p_count(0)
    {
        if (do_reset) { Reset();}
    }
    
    ReferenceCounter( const ReferenceCounter &counter )
    {
        _p_count = counter._p_count;
        if ( !!_p_count ) { ++*_p_count; }
    }

    ReferenceCounter& operator =( const ReferenceCounter &counter )
    {
        Dispose();
        _p_count = counter._p_count;
        if ( !!_p_count ) { ++*_p_count; }
        return *this;
    }

    virtual ~ReferenceCounter()
    {
        Dispose();
    }

    void Reset()
    {
        Dispose();
        _p_count = new int(0);
        try 
        { ++(*_p_count);
        } catch(...)
        { throw Error("ReferenceCounter(): allocation error"); 
        }
        
    }

    bool Unique() const   { return ( !!_p_count && (*_p_count)==1 ); }
    int  GetCount() const { return ((!_p_count)? 0 : (*_p_count) ); }

private:
    int *_p_count;

    void Dispose()
    {
        if ( _p_count && --(*_p_count) == 0 )
        {
            delete _p_count;
        }
        _p_count = 0;
    }
};

};//namespace Useless
#endif//__INCLUDED__USELESS__REFERENCE_COUNTER_H__
