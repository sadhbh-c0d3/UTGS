#ifndef __INCLUDED_USELESS_PRE_INITIALIZER_H__
#define __INCLUDED_USELESS_PRE_INITIALIZER_H__

namespace Useless {

    /*! \ingroup Util
     *  PreInitialisers are used to initialize static elements of classes
     *  before main application starts, and to terminate them after user programm end.
     */
    template< class _Type >
        class PreInitializer
        {
            public:
                typedef _Type type;


                PreInitializer()
                {
                    if ( 0 == Counter()++ )
                    {
                        _Type::Init();
                    }
                }

                ~PreInitializer()
                {
                    if ( --Counter() == 0 )
                    {
                        _Type::Destroy();
                    }
                }

            private:
                static int & Counter();
        };

    
#pragma warning(disable:4661)

#define DECLARE_PREINITIALIZER( _Type, _Name, _API ) \
    template class _API PreInitializer< _Type >; \
    static PreInitializer< _Type > s_PreInitializer##_Name;

#define INIT_PREINITIALIZER( _Type, _Name, _API ) \
    USELESS_SPECIALIZATION int & PreInitializer< _Type >::Counter() { static int s_count = 0; return s_count; }

#define INIT_PREINITIALIZER_ABSTRACT( _Type, _Name, _API ) \
    USELESS_SPECIALIZATION int & PreInitializer< _Type >::Counter() { static int s_count = 0; return s_count; }

};//namespace Useless
#endif//__INCLUDED_USELESS_PRE_INITIALIZER_H__
