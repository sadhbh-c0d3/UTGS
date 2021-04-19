#ifndef __INCLUDED_USELESS_SINGLETON_H__
#define __INCLUDED_USELESS_SINGLETON_H__

/*
**  $Id: Singleton.h,v 1.8 2002/09/13 16:22:11 koolas Exp $
**
**  NAME
**      Singleton
**
**  PURPOSE
**      Singleton has only one instance
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

//#include "unused Threads/Lock.h"

namespace Useless {

template< class T>
class CreateUsing_
{
    public:
		CreateUsing_() {}
		~CreateUsing_() {}
        virtual T* Create()	= 0;
        virtual void Destroy(T * pt) = 0;
};

template< class T>
class CreateUsingNew : public CreateUsing_< T >
{
    public:
        T* Create()			 { return new T; }
        void Destroy(T * pt) { delete pt; }
};


template< class T>
class CreateUsingMalloc : public CreateUsing_< T >
{
    public:    
        T  * Create()		 { return (T*)malloc( sizeof(T) ); }
        void Destroy(T * pt) { free(pt); }
};

/*! \ingroup Util
 *  Use when only one instance of your class is to be allowed in whole application.
 */
template< class T, class CreationPolicy = CreateUsingNew<T> >
class Singleton
{
    public:
        static T& Instance()
        {
            //static Mutex _mutex;
            //Lock guard(_mutex);
            if (! Exists() )
            {
                InstancePtr() = CreationPolicy().Create();    
            }
            return (*InstancePtr());
        }        

        static void Kill()
        {
            CreationPolicy().Destroy( InstancePtr() );
            InstancePtr() = 0;
        }

        static void SafeKill()
        {
            if ( Exists() )
            {
                CreationPolicy().Destroy( InstancePtr() );
                InstancePtr() = 0;
            }
        }

        static bool Exists()
        {
            return !!InstancePtr();
        }

        typedef T type;
        typedef T * pointer;
	typedef CreationPolicy creation_policy;

    private:
        Singleton();
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
        ~Singleton();
        static pointer & InstancePtr();
};

}; // namespace Useless

#include "Useless/Util/SingletonKiller.h"

#endif //__INCLUDED_USELESS_SINGLETON_H__
