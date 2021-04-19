#ifndef __INCLUDED_USELESS_STATIC_CHECK_H__
#define __INCLUDED_USELESS_STATIC_CHECK_H__

namespace CTC {

template<bool> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

template<bool> struct CompileTimeChecker
{
    CompileTimeChecker(...);
};
template<> struct CompileTimeChecker<false>  {};

#define STATIC_CHECK(expr,msg) \
{ \
    class ERROR_##msg {} err; \
    (void)sizeof(CTC::CompileTimeChecker<(expr)>(err)); \
}

template<class To, class From>
To SAFE_CAST(From from)
{
    STATIC_CHECK( sizeof(From) <= sizeof(To), Destination_type_to_narrow );
    return (To)from;
}

template<class T, class U>
struct ConversionTraitsFcn
{
    typedef char Small;
    struct Big { char dummy[2]; };
    static Small Test(U);
    static Big Test(...);
    static T MakeT();
};
    
template<class T, class U>
struct Conversion
{
    typedef ConversionTraitsFcn<T,U> Fc;
    enum { exists = sizeof( Fc::Test( Fc::MakeT())) == sizeof(Fc::Small)};
    enum { is_safe = sizeof(T) >= sizeof(U) };
};

#define STATIC_WARNING(msg) \
{ \
    struct C_WARNING_##msg {}; \
    C_WARNING_##msg *WARNING_##msg;\
}

};// namespace CTC
#endif// __INCLUDED_USELESS_STATIC_CHECK_H__
