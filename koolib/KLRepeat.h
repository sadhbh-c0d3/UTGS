#define KL_REPEAT_0( _Macro )
#define KL_REPEAT_1( _Macro ) _Macro( 1 )
#define KL_REPEAT_2( _Macro ) KL_REPEAT_1( _Macro ) _Macro( 2 )
#define KL_REPEAT_3( _Macro ) KL_REPEAT_2( _Macro ) _Macro( 3 )
#define KL_REPEAT_4( _Macro ) KL_REPEAT_3( _Macro ) _Macro( 4 )
#define KL_REPEAT_5( _Macro ) KL_REPEAT_4( _Macro ) _Macro( 5 )
#define KL_REPEAT_6( _Macro ) KL_REPEAT_5( _Macro ) _Macro( 6 )
#define KL_REPEAT_7( _Macro ) KL_REPEAT_6( _Macro ) _Macro( 7 )
#define KL_REPEAT_8( _Macro ) KL_REPEAT_7( _Macro ) _Macro( 8 )
#define KL_REPEAT_9( _Macro ) KL_REPEAT_8( _Macro ) _Macro( 9 )
#define KL_REPEAT_10( _Macro ) KL_REPEAT_9( _Macro ) _Macro( 10 )
#define KL_REPEAT( _N, _Macro ) KL_REPEAT_##_N

#define KL_COMMA_REPEAT_0( _Macro )
#define KL_COMMA_REPEAT_1( _Macro ) _Macro( 1 )
#define KL_COMMA_REPEAT_2( _Macro ) KL_COMMA_REPEAT_1( _Macro ), _Macro( 2 )
#define KL_COMMA_REPEAT_3( _Macro ) KL_COMMA_REPEAT_2( _Macro ), _Macro( 3 )
#define KL_COMMA_REPEAT_4( _Macro ) KL_COMMA_REPEAT_3( _Macro ), _Macro( 4 )
#define KL_COMMA_REPEAT_5( _Macro ) KL_COMMA_REPEAT_4( _Macro ), _Macro( 5 )
#define KL_COMMA_REPEAT_6( _Macro ) KL_COMMA_REPEAT_5( _Macro ), _Macro( 6 )
#define KL_COMMA_REPEAT_7( _Macro ) KL_COMMA_REPEAT_6( _Macro ), _Macro( 7 )
#define KL_COMMA_REPEAT_8( _Macro ) KL_COMMA_REPEAT_7( _Macro ), _Macro( 8 )
#define KL_COMMA_REPEAT_9( _Macro ) KL_COMMA_REPEAT_8( _Macro ), _Macro( 9 )
#define KL_COMMA_REPEAT_10( _Macro ) KL_COMMA_REPEAT_9( _Macro ), _Macro( 10 )
#define KL_COMMA_REPEAT( _N, _Macro ) KL_REPEAT_##_N

#define KL_KOMMA_0( _Macro ) _Macro
#define KL_KOMMA_1( _Macro ) , _Macro
#define KL_KOMMA_2( _Macro ) , _Macro
#define KL_KOMMA_3( _Macro ) , _Macro
#define KL_KOMMA_4( _Macro ) , _Macro
#define KL_KOMMA_5( _Macro ) , _Macro
#define KL_KOMMA_6( _Macro ) , _Macro
#define KL_KOMMA_7( _Macro ) , _Macro
#define KL_KOMMA_8( _Macro ) , _Macro
#define KL_KOMMA_9( _Macro ) , _Macro
#define KL_KOMMA_10( _Macro ) , _Macro
#define KL_KOMMA( _N, _Macro ) KL_COMMA_##_N( _Macro )

#define KL_DECL_TEMPLATE_CLASS_A       ( _N ) class _A##_N
#define KL_DECL_CONST_REFERENCE_A      ( _N ) const _A##_N & a##_N
#define KL_DECL_REFERENCE_A            ( _N ) _A##_N & a##_N
#define KL_DECL_VALUE_A                ( _N ) _A##_N a##_N
#define KL_DECL_MEMBER_A               ( _N ) _A##_N a##_N
#define KL_VALUE_A                     ( _N ) a##_N
#define KL_TEMPLATE_CLASS_A            ( _N ) _A##_N

namespace KL {

    template< class _R >
        struct result_traits
        {
            typedef _R result_type;

#define KL_RESULT_TRAITS_RESULT_TYPE_R_CALL_F( _N ) \
            template< KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A ), class _F > \
                static result_type \
                call( _F f KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A ))) \
                { \
                    return (*f)( KL_COMMA_REPEAT( _N, KL_VALUE_A )); \
                }
            KL_REPEAT( 10, KL_RESULT_TRAITS_RESULT_TYPE_R_CALL_F );

#define KL_RESULT_TRAITS_RESULT_TYPE_R_CALL_METHOD( _N ) \
            template< class _C, KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )), class _F > \
                static result_type \
                call_method( _F f, _C c KL_KOMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A ))) \
                { \
                    return ((*c).*f)( KL_COMMA_REPEAT( _N, KL_VALUE_A )); \
                }
            KL_REPEAT( 10, KL_RESULT_TRAITS_RESULT_TYPE_R_CALL_METHOD );
        };

    struct result_traits< void >
    {
        typedef struct {} result_type;

#define KL_RESULT_TRAITS_RESULT_TYPE_VOID_CALL_F( _N ) \
        template< KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A ), class _F > \
            static result_type \
            call( _F f KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A ))) \
            { \
                (*f)( KL_COMMA_REPEAT( _N, KL_VALUE_A )); \
                    return result_type(); \
            }
        KL_REPEAT( 10, KL_RESULT_TRAITS_RESULT_TYPE_VOID_CALL_F );

#define KL_RESULT_TRAITS_RESULT_TYPE_VOID_CALL_METHOD( _N ) \
        template< class _C, KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )), class _F > \
            static result_type \
            call_method( _F f, _C c KL_KOMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A ))) \
            { \
                ((*c).*f)( KL_COMMA_REPEAT( _N, KL_VALUE_A )); \
                    return result_type(); \
            }
        KL_REPEAT( 10, KL_RESULT_TRAITS_RESULT_TYPE_R_CALL_METHOD );
    };

    template< class _R >
        struct function_object
        {
            typedef result_traits< _R >::result_type result_type;
            virtual ~function_object() {}
            function_object(): _refCount(0) {} int _refCount;
            void add_ref() { ++_refCount; }
            void rem_ref() { if ( 0 <= --_refCount ) { release(); } }
            virtual void release() { delete this; }
        };

#define KL_FUNCTION_OBJECT( _N ) \
    template< class _R _F KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )) > \
        struct function_object_##_N : function_object< _R > \
        { \
            typedef result_traits< _R >::result_type result_type; \
                virtual result_type operator () ( KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A )) = 0;\
        };
    KL_REPEAT( 10, KL_FUNCTION_OBJECT );

#define KL_FUNCTION_OBJECT_FUNCTION_WRAPPER( _N ) \
    template< class _R, class _F KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )) > \
        struct function_object_function_wrapper_##_N : \
        function_object_##_N< _R KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) > \
        { \
            typedef result_traits< _R >::result_type result_type; \
                function_object_function_wrapper_##_N( F f ): _f(f) {} _F _f; \
                result_type operator () ( KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A )) \
                { \
                    return result_traits< _R >::call( KL_COMMA_REPEAT( _N, KL_VALUE_A )); \
                } \
        };
    KL_REPEAT( 10, KL_FUNCTION_OBJECT_FUNCTION_WRAPPER );

#define KL_FUNCTION_OBJECT_METHOD_WRAPPER( _N ) \
    template< class _R, class _M, class _C KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )) > \
        struct function_object_method_wrapper_##_N : \
        function_object_##_N< _R, _C KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) > \
        { \
            typedef result_traits< _R >::result_type result_type; \
                function_object_method_wraper_##_N( M m ): _m(m) {} _M _m; \
                result_type operator () ( _C c KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_CONST_REFERENCE_A ))) \
                { \
                    return result_traits< _R >::call_method( c KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_VALUE_A ))); \
                } \
        };
    KL_REPEAT( 9, KL_FUNCTION_OBJECT_METHOD_WRAPPER );

#define KL_WRAP_FUNCTION( _N ) \
    template< class _R KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )) > \
        function_object_function_wrapper_##_N \
        < _R, _R (*)( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) KL_COMMA( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) > \
        wrap_function( _R (*f)( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) ) \
        { \
            return new \
                function_object_function_wrapper_##_N \
                < _R, _R (*)( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) KL_COMMA( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) > \
                (f); \
        }
    KL_REPEAT( 10, KL_WRAP_FUNCTION );

#define KL_WRAP_METHOD( _N ) \
    template< class _R, class _C KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_DECL_TEMPLATE_CLASS_A )) > \
        function_object_method_wrapper_##_N \
        < _R, _R (_C::*)( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )), _C KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) > \
        wrap_method( _R (_C::*m)( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) ) \
        { \
            return new \
                function_object_method_wrapper_##_N \
                < _R, _R (_C::*)( KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )), _C KL_COMMA( _N, KL_COMMA_REPEAT( _N, KL_TEMPLATE_CLASS_A )) > \
                (m); \
        }
    KL_REPEAT( 9, KL_WRAP_METHOD );

};//KL

