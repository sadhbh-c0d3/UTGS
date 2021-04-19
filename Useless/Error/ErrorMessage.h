#ifndef __INCLIDED_USELESS_ERROR_MESSAGE_H__
#define __INCLIDED_USELESS_ERROR_MESSAGE_H__

#include "Useless/System/Assert.h"
#include "Useless/Error/Error.h"
#include "Useless/TemplateConfig.h"
#include <typeinfo>

namespace Useless {

template< class _T, const int _I> struct BinaryClassIntSpecialization
{
    typedef _T Type;
    enum { Int = _I };
};

template< class _K >
struct WarnEnabledAttr
{
    enum { IsEnabled = true };
};

struct USELESS_API RTTIWarnEnabledAttr
{
    static bool
    IsEnabled( const std::type_info &tp, int id );
};
    

// only int, float, char, void_pointer my be formated
template< class T > struct Format {};

USELESS_SPECIALIZATION struct USELESS_API Format<int>    { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<float>  { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<char *> { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<void *> { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<const int>    { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<const float>  { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<const char *> { static char x[3]; };
USELESS_SPECIALIZATION struct USELESS_API Format<const void *> { static char x[3]; };

template< class _K>
struct ErrorMessage
{
    static Error Cat( const Error &e) { return e;}
};

template< class _Base >
struct VaErrorMessage
{
    static Error
    Message()
    {
        return _Base::Cat( Error("<no info>"));
    }

    template<
        class A1
        > static Error
    Message( A1 a1)
    {
        return Error( Format<A1>::x, a1 );
    }

    template<
        class A1,
        class A2
        > static Error
    Message( A1 a1, A2 a2)
    {
        Error e("%s %s", Format<A1>::x, Format<A2>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2 ));
    }

    template<
        class A1,
        class A2,
        class A3
        > static Error
    Message( A1 a1, A2 a2, A3 a3)
    {
        Error e("%s %s %s", Format<A1>::x, Format<A2>::x, Format<A3>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2, a3 ));
    }

    template<
        class A1,
        class A2,
        class A3,
        class A4
        > static Error
    Message( A1 a1, A2 a2, A3 a3, A4 a4)
    {
        Error e("%s %s %s %s", Format<A1>::x, Format<A2>::x, Format<A3>::x, Format<A4>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2, a3, a4 ));
    }
    
    template<
        class A1,
        class A2,
        class A3,
        class A4,
        class A5
        > static Error
    Message( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
    {
        Error e("%s %s %s %s %s", Format<A1>::x, Format<A2>::x, Format<A3>::x, Format<A4>::x,
                Format<A5>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2, a3, a4, a5 ));
    }
    
    template<
        class A1,
        class A2,
        class A3,
        class A4,
        class A5,
        class A6
        > static Error
    Message( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
    {
        Error e("%s %s %s %s %s %s", Format<A1>::x, Format<A2>::x, Format<A3>::x, Format<A4>::x,
                Format<A5>::x, Format<A6>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2, a3, a4, a5, a6 ));
    }
    
    template<
        class A1,
        class A2,
        class A3,
        class A4,
        class A5,
        class A6,
        class A7
        > static Error
    Message( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
    {
        Error e("%s %s %s %s %s %s", Format<A1>::x, Format<A2>::x, Format<A3>::x, Format<A4>::x,
                Format<A5>::x, Format<A6>::x, Format<A7>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2, a3, a4, a5, a6, a7 ));
    }
    
    template<
        class A1,
        class A2,
        class A3,
        class A4,
        class A5,
        class A6,
        class A7,
        class A8
        > static Error
    Message( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
    {
        Error e("%s %s %s %s %s %s", Format<A1>::x, Format<A2>::x, Format<A3>::x, Format<A4>::x,
                Format<A5>::x, Format<A6>::x, Format<A7>::x, Format<A8>::x);
        return _Base::Cat( Error( e.GetError(), a1, a2, a3, a4, a5, a6, a7, a8 ));
    }
};

};//namespace Useless

#define DISABLE_WARNING( _class_id, _id)\
namespace Useless {\
    struct WarnEnabledAttr< BinaryClassIntSpecialization< _class_id, _id> >\
    {\
        enum { IsEnabled = false };\
    };\
};//namespace Useless

#define SET_ERROR_MESSAGE( _class_id, _id, _text)\
namespace Useless {\
    USELESS_SPECIALIZATION struct ErrorMessage< BinaryClassIntSpecialization< _class_id, _id> >\
    {\
        static Error Cat( const Error &e) { return Error("\n[Message]\t%s\n[State]\t\t%s", #_text, e.GetError());}\
    };\
};//namespace Useless

#define C_IsEnabled( _class_id, _id)\
    ( Useless::WarnEnabledAttr< Useless::BinaryClassIntSpecialization< _class_id, _id> >::IsEnabled && \
        Useless::RTTIWarnEnabledAttr::IsEnabled( typeid( _class_id ), _id ))\

#define C_MakeMessage( _class_id, _id, _text)\
    Useless::VaErrorMessage< \
        Useless::ErrorMessage< Useless::BinaryClassIntSpecialization< _class_id, _id> >\
            >::Message _text .GetError()

            
#define C_Warn( _class_id, _id, _text)\
    if C_IsEnabled( _class_id, _id)\
    { C_warning( _text && "class" #_class_id "code" #_id);}\
    

#define C_message( _class_id, _id, _text)\
    if C_IsEnabled( _class_id, _id)\
    {\
        Useless::SendError( Useless::AssertPriority::Message, #_class_id,\
            C_MakeMessage( _class_id, _id, _text));\
    }


// Some better checking macros   

#   define C_assertf( _class_id, _id, _expr, _text)\
    C_msg_assert( _expr || !C_IsEnabled( _class_id, _id), \
            C_MakeMessage( _class_id, _id, _text));

#   define C_verifyf( _class_id, _id, _expr, _text)\
    if C_IsEnabled( _class_id, _id)\
        { C_msg_verify( _expr, C_MakeMessage( _class_id, _id, _text)); }\
    else \
        { C_verify( _expr ); }

#   define C_warnf( _class_id, _id, _expr, _text)\
    if C_IsEnabled( _class_id, _id)\
        { C_msg_warning( _expr, C_MakeMessage( _class_id, _id, _text)); }\
    else \
        { _expr; }

#   define C_xlogf( _class_id, _id, _expr, _text)\
    C_msg_log_until( _expr || !C_IsEnabled( _class_id, _id), \
            C_MakeMessage( _class_id, _id, _text));


#endif//__INCLIDED_USELESS_ERROR_MESSAGE_H__
