#ifndef __INCLUDED_SINGLETON_KILLER_H__
#define __INCLUDED_SINGLETON_KILLER_H__

#include "Useless/Util/PreInitializer.h"

namespace Useless {

    template< class SingletonType >
        class SingletonKiller
        {
            public:
                typedef PreInitializer< SingletonKiller > PostDestr;

            private:
                static void Init() {}
                static void Destroy() { SingletonType::SafeKill(); }

                friend class PreInitializer< SingletonKiller >;
        };


    /* Type this lines into place where you will use killer
       --------------------------------------------------------
       in .h:
       */
    
#define DECLARE_SINGLETON(base_name, singleton_name, _API ) \
        typedef Useless::Singleton< base_name > singleton_name; \
        template class _API Singleton< singleton_name::type, singleton_name::creation_policy >; \
        template class _API SingletonKiller< singleton_name >; \
        DECLARE_PREINITIALIZER( SingletonKiller< singleton_name >, singleton_name##Killer, _API );
        
#define DECLARE_SINGLETON_CTOR(base_name, singleton_name, ctor_type, _API ) \
        typedef Useless::Singleton< base_name, ctor_type > singleton_name; \
        template class _API Singleton< singleton_name::type, singleton_name::creation_policy >; \
        template class _API SingletonKiller< singleton_name >; \
        DECLARE_PREINITIALIZER( SingletonKiller< singleton_name >, singleton_name##Killer, _API );
        
        /*
           in .cpp:
           */
#define INIT_SINGLETON( singleton_name, _API ) \
        USELESS_SPECIALIZATION singleton_name::pointer &\
            Singleton< singleton_name::type, singleton_name::creation_policy >::\
            InstancePtr() { static singleton_name::pointer s_ptr = 0; return s_ptr; } \
	INIT_PREINITIALIZER( SingletonKiller< singleton_name >, singleton_name##Killer, _API );


};//namespace Useless

#endif//__INCLUDED_SINGLETON_KILLER_H__
