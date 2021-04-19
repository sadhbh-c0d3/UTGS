#ifndef __INCLUDED_CTC_H__
#define __INCLUDED_CTC_H__

#include "Useless/TemplateConfig.h"
#include "Useless/Util/StaticCheck.h"

/*! Useless Type Check
 */
namespace CTC {

namespace intimate {

    struct SelectThen 
    {       
        template<typename Then, typename Else>
        struct Result
        {       
        typedef Then type;
        };
    };

    struct SelectElse
    {
        template<typename Then, typename Else>
        struct Result
        { 
        typedef Else type;
        };
    };

    template<bool Condition>
    struct Selector
    {
        typedef SelectThen type;
    };

    template<>
    struct Selector<false>
    {
        typedef SelectElse type;
    };
} // end namespace intimate 

template<bool Condition, typename Then, typename Else>
struct IF
{
    typedef typename intimate::Selector<Condition>::type select;
    typedef typename select::template Result<Then,Else>::type type;
};

template< class T >
struct TypeFcn
{
    static T& MakeT();
};

typedef char (&CTRUE )[1];
typedef char (&CFALSE)[2];

template< typename T1, typename T2 >
struct TypeCompareFcn
{
    template<typename T>  struct tc_type  {};

    static CFALSE Compare( ... );
    static CTRUE  Compare( tc_type<T1>, tc_type<T1> );

    static CFALSE Casts(...);
    static CTRUE  Casts( const T1& );
};

template< typename T1, typename T2 >
struct TypeCompare
{
    typedef TypeCompareFcn< T1, T2> Fc;
    enum 
    { 
        equal = sizeof( Fc::Compare( Fc::tc_type<T1>(), Fc::tc_type<T2>() ) ) == sizeof(CTRUE),
        casts = sizeof( Fc::Casts( T2() ) ) == sizeof(CTRUE),
    };
};

template< class T, class T1 > T& Assign( T &variable, const T1 &value ) 
{
    STATIC_CHECK( (Conversion<T1,T>::exists), NO_DEFAULT_CONVERTION );
    return variable = (const T&)value; 
}


typedef struct{} NullType;
template< int I > struct arg {};
template< typename T> struct type {};
template< typename T>
struct value
{
    value() {}
    value( const T &x): x(x) {}
    operator T& () const { return x; }
    mutable T x;
};

template< int I > struct numbered { enum { index = I }; };
template< class T > type<T> getType( const T &x ) { return type<T>(); }

struct Null_T_Fcn
{
    static CFALSE IsNull(...);
    static CTRUE  IsNull( const NullType & );
};

template< class T > struct Null_Traits
{
    enum { is_null = sizeof(Null_T_Fcn::IsNull( TypeFcn<T>::MakeT()))==sizeof(CTRUE) };
};

#define CTC_IsNullType(t) \
    (sizeof( CTC::Null_T_Fcn::IsNull(t) ) == sizeof( CTC::CTRUE ))


    
//! Special const traits used to make type T a 'const T'.
//@{
// WIN32 version
#ifdef WIN32
    namespace imp {
        template< class T > struct MakeConst
        {
            template< const bool Y > struct Choose
            {
                typedef T Result;
            };

            template<> struct Choose< true >
            {
                typedef const T Result;
            };
        };
    };//imp

    template< const bool Y, class T >
    struct MakeConst
    {
        typedef typename imp::MakeConst<T>::Choose<Y>::Result Result;
    };
// GCC version
#else
    template< const bool Y, class T >
    struct MakeConst
    {
        typedef T Result;
    };

    template< class T >
    struct MakeConst< true, T>
    {
        typedef const T Result;
    };
#endif
//@}


// W/A for zero sized arrays (msvc bug)
template< const int S > struct ArraySize { enum { Result = S }; };
USELESS_SPECIALIZATION struct ArraySize<0> { enum { Result = 1 }; };



// Exact Type Detector
template< class _T > struct Type
{
    typedef _T T;
};


// Array NumElements and ElementSize
template< class _T >
struct ArrayTraits
{
    template< const int _N > struct Size
    {
        typedef int Unit;
        Unit _s[_N];
    };

    template< class _T0, const int _N > static Size< _N > TestSize( Type< _T0 [_N] > );

    enum { 
        NumElements =
            (sizeof( TestSize( Type< _T >() )) -
             ( sizeof( Size<1> ) - sizeof( Size<1>::Unit )))
            /sizeof( Size<1>::Unit ),

        ElementSize =
            sizeof( _T ) / NumElements
    };
};


// If member is in array, athen GetNthMember gets offset of nth member.
template< class _T, class _C > _T _C::* GetNthMember( _T _C::*x, int n )
{
    _T _C::*y = x;
    (char *&)y += n * ArrayTraits< _T >::ElementSize;
    return y;
};

/*
template< class _T >
struct SPointerTestFcn
{
    template< class _X >
        CTRUE test( SPointer< _X > & );
    CFALSE test(...);
};


template< class _T > struct SPointerTraits
{
    template< const bool _F > struct element_type_traits { typedef typename _T::element_type result; };
    struct element_type_traits< false > { typedef CFALSE result; };

    enum {
        is_spointer = sizeof( SPointerTestFcn::test( TypeFcn<_T>::MakeT() )) == sizeof(CTRUE);
    };
    
    typedef typename IF< is_spointer
        , typename element_type_traits< is_spointer >::result
        , _T >::Result elemet_type;
};
*/


};//namespace CTC
#endif//__INCLUDED_CTC_H__
