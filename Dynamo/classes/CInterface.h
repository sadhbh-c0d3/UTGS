#ifndef __INCLUDED_DYNAMO_CINTERFACE_H__
#define __INCLUDED_DYNAMO_CINTERFACE_H__

#include "Dynamo/interfaces.h"

namespace Dynamo {

    struct CInterface : virtual IInterface
    {
        CInterface(): _refCount(0)
        {
        }

        ~CInterface()
        {
        }

        int _refCount;

        int AddRef()
        {
            return ++_refCount;
        }

        int RemRef()
        {
            assert( _refCount > 0 );
            if ( 0 == --_refCount )                
            {
                Release();
                return 0;
            }
            else {
                return _refCount;
            }
        }

        virtual void Release()
        {
            delete this;
        }
    };

};//Dynamo

#endif//__INCLUDED_DYNAMO_CINTERFACE_H__
