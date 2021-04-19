#ifndef __INCLUDED__USELESS_PARM_LIST_H__
#define __INCLUDED__USELESS_PARM_LIST_H__
/*
**  $Id: ParmList.h,v 1.17 2003/01/20 20:40:58 koolas Exp $
**
**  NAME
**      ParmList_(n)
**
**  PURPOSE
**      Static Lists for Parameters
**
**  USEAGE
**      _PL( T1, T2, ...) template functions create n-sized ParmList_(n)'s
**
**      T x = InitParm<T>( L, D ); this will init x of type T using value from list L
**                                 or in case there isn't any uses list D (defaults)
**                                 if neither was matched compiler error NullType assigment appears
**
**  NOTE
**      Parameter types are used as index values.
**
**      !!! It is impossibile to use two or more same types. !!!
**
**      One may change type using struct CTC::Rename<T,int>
**      For example:
**                   typedef Rename<int,0> px; 
**
**      'px' can be understood as name of parameter (here x position attribute)
**      ParmList may contain only one 'px' type
**
**      To assign value of parameter 'px' on parm list do:
**      parm_list[ type<px>() ] = x;
**
**  SEE ALSO
**      If you are interested in creating functions that uses ParmLists
**      then take a look at section 'Initialiser macros' (few screen below)
**
**  AUTHOR(S)
**      CodeZilla - Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/Util/TemplateDefines.h"
#include "Useless/Util/CTC.h"
#include "Useless/Util/StaticCheck.h"

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'


namespace Useless
{

/* ParmList Ctor initializer lists:
 *
 * Synopsis:
 *      ParmList_k( const ParmList_n& ): a1( l[ type<T1>() ] ),
 *                                       a2( l[ type<T2>() ] ),
 *                                       .....................,
 *                                       ak( l[ type<Tk>() ] )
 */
#define __LST_INIT_N__(l,n) a##n ( l[CTC::type< T##n >()] )

#define _LST_INIT_1(l) __LST_INIT_N__(l,1)
#define _LST_INIT_2(l) __LST_INIT_N__(l,2), _LST_INIT_1(l)
#define _LST_INIT_3(l) __LST_INIT_N__(l,3), _LST_INIT_2(l)
#define _LST_INIT_4(l) __LST_INIT_N__(l,4), _LST_INIT_3(l)
#define _LST_INIT_5(l) __LST_INIT_N__(l,5), _LST_INIT_4(l)
#define _LST_INIT_6(l) __LST_INIT_N__(l,6), _LST_INIT_5(l)
#define _LST_INIT_7(l) __LST_INIT_N__(l,7), _LST_INIT_6(l)
#define _LST_INIT_8(l) __LST_INIT_N__(l,8), _LST_INIT_7(l)
#define _LST_INIT_9(l) __LST_INIT_N__(l,9), _LST_INIT_8(l)
#define _LST_INIT_10(l) __LST_INIT_N__(l,10), _LST_INIT_9(l)
#define _LST_INIT_11(l) __LST_INIT_N__(l,11), _LST_INIT_10(l)
#define _LST_INIT_12(l) __LST_INIT_N__(l,12), _LST_INIT_11(l)
#define _LST_INIT_13(l) __LST_INIT_N__(l,13), _LST_INIT_12(l)
#define _LST_INIT_14(l) __LST_INIT_N__(l,14), _LST_INIT_13(l)
#define _LST_INIT_15(l) __LST_INIT_N__(l,15), _LST_INIT_14(l)
#define _LST_INIT_16(l) __LST_INIT_N__(l,16), _LST_INIT_15(l)
#define _LST_INIT_17(l) __LST_INIT_N__(l,17), _LST_INIT_16(l)
#define _LST_INIT_18(l) __LST_INIT_N__(l,18), _LST_INIT_17(l)
#define _LST_INIT_19(l) __LST_INIT_N__(l,19), _LST_INIT_18(l)
#define _LST_INIT_20(l) __LST_INIT_N__(l,20), _LST_INIT_19(l)

/* Operators: 
 *
 *    const T1&     operator [](CTC::type<T1>)
 *    const T2&     operator [](CTC::type<T2>)
 *    ........................................
 *    const Tn&     operator [](CTC::type<Tn>)
 *
 *    CTC::value<T> operator [](CTC::value<T>)
 *    CTC::NullType operator [](CTC::type<T> )
 *
 *    const Tn& operator [](CTC::arg<n>) const
 *    Tn& operator [](CTC::type<Tn>)
 *    Tn& operator [](CTC::arg<n>)
 *
 */
#define _LST_DEFAULT_OPS \
    template< class T > \
        CTC::value<T> operator[](const CTC::value<T> &x) { return x; } \
    template< class T > \
        CTC::NullType operator[](const CTC::type<T> &) const { return CTC::NullType(); }\
    template< class T > \
        static CTC::CFALSE Exists(CTC::type<T>);


#define __LST_OPERATORS__(n) const T##n & operator[](CTC::type< T##n >) const { return a##n; }\
                             const T##n & operator[](CTC::arg< n >  ) const { return a##n; }\
                             T##n & operator[](CTC::type< T##n >) { return a##n; }\
                             T##n & operator[](CTC::arg< n >  ) { return a##n; }

#define _LST_OP_1 __LST_OPERATORS__(1)
#define _LST_OP_2 __LST_OPERATORS__(2); _LST_OP_1
#define _LST_OP_3 __LST_OPERATORS__(3); _LST_OP_2
#define _LST_OP_4 __LST_OPERATORS__(4); _LST_OP_3
#define _LST_OP_5 __LST_OPERATORS__(5); _LST_OP_4
#define _LST_OP_6 __LST_OPERATORS__(6); _LST_OP_5
#define _LST_OP_7 __LST_OPERATORS__(7); _LST_OP_6
#define _LST_OP_8 __LST_OPERATORS__(8); _LST_OP_7
#define _LST_OP_9 __LST_OPERATORS__(9); _LST_OP_8
#define _LST_OP_10 __LST_OPERATORS__(10); _LST_OP_9
#define _LST_OP_11 __LST_OPERATORS__(11); _LST_OP_10
#define _LST_OP_12 __LST_OPERATORS__(12); _LST_OP_11
#define _LST_OP_13 __LST_OPERATORS__(13); _LST_OP_12
#define _LST_OP_14 __LST_OPERATORS__(14); _LST_OP_13
#define _LST_OP_15 __LST_OPERATORS__(15); _LST_OP_14
#define _LST_OP_16 __LST_OPERATORS__(16); _LST_OP_15
#define _LST_OP_17 __LST_OPERATORS__(17); _LST_OP_16
#define _LST_OP_18 __LST_OPERATORS__(18); _LST_OP_17
#define _LST_OP_19 __LST_OPERATORS__(19); _LST_OP_18
#define _LST_OP_20 __LST_OPERATORS__(20); _LST_OP_19

//static CTC::CTRUE Exists(CTC::type<T1>);
//? why copiler says that function ^^^ was allready declared ?

/* ParmList_n
 *
 *  struct ParmList_n
 *  {
 *      ParmList_n();
 *      ParmList_n( const T1 &, const T2 &, ..., const Tn & );
 *      ParmList_n( const ParmList_k & );
 *
 *      const T1& operator[] ( CTC::type<T1> );
 *      const T2& operator[] ( CTC::type<T2> );
 *      ......................................;
 *      const Tn& operator[] ( CTC::type<Tn> );
 *
 *      CTC::NullType operator[] ( CTC::type<T> );
 *      CTC::value<T> operator[] ( CTC::value<T>);
 *
 *      T1 a1; T2 a2;... ; Tn an;
 *  };
 */
_TPL_1 struct ParmList_1
{
    ParmList_1() {}
    ParmList_1( _TPL_ARGS_1 ): _TPL_ARGS_INIT_1 {}
    //template< class Lst > ParmList_1( const Lst &l ): _LST_INIT_1(l) {}
    
    _LST_OP_1   
    _LST_DEFAULT_OPS
    _TPL_PARMS_1;
};

#define __LST_PARM_LIST__(n) \
    _TPL_##n struct ParmList_##n \
    { \
        ParmList_##n() {} \
        ParmList_##n( _TPL_ARGS_##n ): _TPL_ARGS_INIT_##n {} \
        template< class Lst > ParmList_##n( const Lst &l ): _LST_INIT_##n(l) {} \
        _LST_OP_##n \
        _LST_DEFAULT_OPS \
        _TPL_PARMS_##n; \
    };

                      
__LST_PARM_LIST__(2)
__LST_PARM_LIST__(3)
__LST_PARM_LIST__(4)
__LST_PARM_LIST__(5)
__LST_PARM_LIST__(6)
__LST_PARM_LIST__(7)
__LST_PARM_LIST__(8)
__LST_PARM_LIST__(9)
__LST_PARM_LIST__(10)
__LST_PARM_LIST__(11)
__LST_PARM_LIST__(12)
__LST_PARM_LIST__(13)
__LST_PARM_LIST__(14)
__LST_PARM_LIST__(15)
__LST_PARM_LIST__(16)
__LST_PARM_LIST__(17)
__LST_PARM_LIST__(18)
__LST_PARM_LIST__(19)
__LST_PARM_LIST__(20)


//ParmList(n) template wrapper
template< typename AnyParmList > struct ParmList : public AnyParmList
{
    ParmList( const AnyParmList &p ): AnyParmList(p) {} 
};

};//namespace Useless



/*! Useless Type Check
 */
namespace CTC {

struct PL_T_Fcn
{
    _TPL_1 static CTRUE IsList( const Useless::ParmList<T1> &);

#define __PL_T_Fcn_isList__(n) \
    _TPL_##n static CTRUE IsList( const Useless::ParmList_##n<_TPL_TYPES_##n> &);

__PL_T_Fcn_isList__(1)
__PL_T_Fcn_isList__(2)
__PL_T_Fcn_isList__(3)
__PL_T_Fcn_isList__(4)
__PL_T_Fcn_isList__(5)
__PL_T_Fcn_isList__(6)
__PL_T_Fcn_isList__(7)
__PL_T_Fcn_isList__(8)
__PL_T_Fcn_isList__(9)
__PL_T_Fcn_isList__(10)
__PL_T_Fcn_isList__(11)
__PL_T_Fcn_isList__(12)
__PL_T_Fcn_isList__(13)
__PL_T_Fcn_isList__(14)
__PL_T_Fcn_isList__(15)
__PL_T_Fcn_isList__(16)
__PL_T_Fcn_isList__(17)
__PL_T_Fcn_isList__(18)
__PL_T_Fcn_isList__(19)
__PL_T_Fcn_isList__(20)

            static CFALSE IsList(...);
};

template< typename T > struct PL_Traits
{
    enum { is_list = sizeof( PL_T_Fcn::IsList( TypeFcn<T>::MakeT()))==sizeof(CTRUE) };
};

#define CTC_IsParmList(t) \
    (sizeof( CTC::PL_T_Fcn::IsList(t) ) == sizeof( CTC::CTRUE ))


#define CTC_PL_Exists(L,T) !CTC_IsNullType( L()[type<T>()] )



template< class T, class L, class D >
struct ParmInit
{
    static const T& Get( const D &d, NullType   ) { return d[type<T>()]; }
    static const T& Get( const D &d, const T &x ) { return x; }
};

template< class T, class L, class D >
struct ParmGet
{
    static const D& Get( const D &d, NullType   ) { return d; }
    static const T& Get( const D &d, const T &x ) { return x; }
};


template< class T > struct Creator
{
    template< class Parms > static T* Create( const Parms &p)
    {
        STATIC_CHECK( CTC::PL_Traits<Parms>::is_list, ARG_1_IS_NOT_PARM_LIST );

        return new T( use_parms, p);
    }
};

};//namespace CTC


/* Useless root
 */
namespace Useless {

/*! Create ParmList:
 *          _PL( const T1 &a1, const T2 &a2,... ,const Tn &an );
 */
#define __LST_PARM_LIST_CREATOR(n) \
_TPL_##n ParmList< ParmList_##n< _TPL_TYPES_##n > > _PL( _TPL_ARGS_##n ) \
{\
    return ParmList_##n<_TPL_TYPES_##n>(_TPL_PASS_ARGS_##n);\
}

__LST_PARM_LIST_CREATOR(1)
__LST_PARM_LIST_CREATOR(2)
__LST_PARM_LIST_CREATOR(3)
__LST_PARM_LIST_CREATOR(4)
__LST_PARM_LIST_CREATOR(5)
__LST_PARM_LIST_CREATOR(6)
__LST_PARM_LIST_CREATOR(7)
__LST_PARM_LIST_CREATOR(8)
__LST_PARM_LIST_CREATOR(9)
__LST_PARM_LIST_CREATOR(10)
__LST_PARM_LIST_CREATOR(11)
__LST_PARM_LIST_CREATOR(12)
__LST_PARM_LIST_CREATOR(13)
__LST_PARM_LIST_CREATOR(14)
__LST_PARM_LIST_CREATOR(15)
__LST_PARM_LIST_CREATOR(16)
__LST_PARM_LIST_CREATOR(17)
__LST_PARM_LIST_CREATOR(18)
__LST_PARM_LIST_CREATOR(19)
__LST_PARM_LIST_CREATOR(20)


/* ParmList parameter list:
 *          pl[arg<1>()], pl[arg<2>()],.... ,pl[arg<n>()]
 */
#define _LST_PARM_LIST_PARMS_1_ pl[CTC::arg<1>()]
#define _LST_PARM_LIST_PARMS_2_ _LST_PARM_LIST_PARMS_1_, pl[CTC::arg<2>()]
#define _LST_PARM_LIST_PARMS_3_ _LST_PARM_LIST_PARMS_2_, pl[CTC::arg<3>()]
#define _LST_PARM_LIST_PARMS_4_ _LST_PARM_LIST_PARMS_3_, pl[CTC::arg<4>()]
#define _LST_PARM_LIST_PARMS_5_ _LST_PARM_LIST_PARMS_4_, pl[CTC::arg<5>()]
#define _LST_PARM_LIST_PARMS_6_ _LST_PARM_LIST_PARMS_5_, pl[CTC::arg<6>()]
#define _LST_PARM_LIST_PARMS_7_ _LST_PARM_LIST_PARMS_6_, pl[CTC::arg<7>()]
#define _LST_PARM_LIST_PARMS_8_ _LST_PARM_LIST_PARMS_7_, pl[CTC::arg<8>()]
#define _LST_PARM_LIST_PARMS_9_ _LST_PARM_LIST_PARMS_8_, pl[CTC::arg<9>()]
#define _LST_PARM_LIST_PARMS_10_ _LST_PARM_LIST_PARMS_9_, pl[CTC::arg<10>()]
#define _LST_PARM_LIST_PARMS_11_ _LST_PARM_LIST_PARMS_10_, pl[CTC::arg<11>()]
#define _LST_PARM_LIST_PARMS_12_ _LST_PARM_LIST_PARMS_11_, pl[CTC::arg<12>()]
#define _LST_PARM_LIST_PARMS_13_ _LST_PARM_LIST_PARMS_12_, pl[CTC::arg<13>()]
#define _LST_PARM_LIST_PARMS_14_ _LST_PARM_LIST_PARMS_13_, pl[CTC::arg<14>()]
#define _LST_PARM_LIST_PARMS_15_ _LST_PARM_LIST_PARMS_14_, pl[CTC::arg<15>()]
#define _LST_PARM_LIST_PARMS_16_ _LST_PARM_LIST_PARMS_15_, pl[CTC::arg<16>()]
#define _LST_PARM_LIST_PARMS_17_ _LST_PARM_LIST_PARMS_16_, pl[CTC::arg<17>()]
#define _LST_PARM_LIST_PARMS_18_ _LST_PARM_LIST_PARMS_17_, pl[CTC::arg<18>()]
#define _LST_PARM_LIST_PARMS_19_ _LST_PARM_LIST_PARMS_18_, pl[CTC::arg<19>()]
#define _LST_PARM_LIST_PARMS_20_ _LST_PARM_LIST_PARMS_19_, pl[CTC::arg<20>()]

/*! Expand ParmList
 *          _PLX( const ParmList &pl, const T &x );
 */
#define __LST_PARM_LIST_EXPAND(m,n) \
_TPL_##n ParmList< ParmList_##n<_TPL_TYPES_##n> > _PLX( const ParmList_##m<_TPL_TYPES_##m> &pl, const T##n &x )\
{\
    return ParmList_##n<_TPL_TYPES_##n>( _LST_PARM_LIST_PARMS_##m##_, x);\
}

__LST_PARM_LIST_EXPAND(1,2)
__LST_PARM_LIST_EXPAND(2,3)
__LST_PARM_LIST_EXPAND(3,4)
__LST_PARM_LIST_EXPAND(4,5)
__LST_PARM_LIST_EXPAND(5,6)
__LST_PARM_LIST_EXPAND(6,7)
__LST_PARM_LIST_EXPAND(7,8)
__LST_PARM_LIST_EXPAND(8,9)
__LST_PARM_LIST_EXPAND(9,10)
__LST_PARM_LIST_EXPAND(10,11)
__LST_PARM_LIST_EXPAND(11,12)
__LST_PARM_LIST_EXPAND(12,13)
__LST_PARM_LIST_EXPAND(13,14)
__LST_PARM_LIST_EXPAND(14,15)
__LST_PARM_LIST_EXPAND(15,16)
__LST_PARM_LIST_EXPAND(16,17)
__LST_PARM_LIST_EXPAND(17,18)
__LST_PARM_LIST_EXPAND(18,19)
__LST_PARM_LIST_EXPAND(19,20)

/* Initialize from ParmList
 */
template< class T, typename Params, typename Defaults >
const T& InitParm( CTC::type<T>, const Params &l, const Defaults &d )
{
    STATIC_CHECK( CTC::PL_Traits<Params>::is_list,   ARG_1_IS_NOT_PARM_LIST );
    STATIC_CHECK( CTC::PL_Traits<Defaults>::is_list, ARG_2_IS_NOT_PARM_LIST );

    return CTC::ParmInit<T,Params,Defaults>::Get( d, l[CTC::type<T>()] );
}

template< class T, typename Params, typename Defaults >
const T& InitParm( const T&, const Params &l, const Defaults &d )
{
    STATIC_CHECK( CTC::PL_Traits<Params>::is_list,   ARG_1_IS_NOT_PARM_LIST );
    STATIC_CHECK( CTC::PL_Traits<Defaults>::is_list, ARG_2_IS_NOT_PARM_LIST );

    return CTC::ParmInit<T,Params,Defaults>::Get( d, l[CTC::type<T>()] );
}

/* Get parameter value by parameter-type-name
 * If parameter does not exist then use default value d
 */
template< class T, typename Parms, typename D >
const D& GetParm( CTC::type<T>, const Parms &l, const D &d )
{
    STATIC_CHECK( CTC::PL_Traits<Parms>::is_list, ARG_1_IS_NOT_PARM_LIST );

    return CTC::ParmGet<T,Parms,D>::Get( d, l[CTC::type<T>()] );
}

template< class T, typename Parms, typename D >
const D& GetParm( const T&, const Parms &l, const D &d )
{
    STATIC_CHECK( CTC::PL_Traits<Parms>::is_list, ARG_1_IS_NOT_PARM_LIST );

    return CTC::ParmGet<T,Parms,D>::Get( d, l[CTC::type<T>()] );
}

/* Get parameter value by parameter-type-name
 * Parameter must exist or compile time error will occure
 */
template< class T, typename Parms >
const T& GetParmStrict( CTC::type<T>, const Parms &l )
{
    STATIC_CHECK( CTC::PL_Traits<Parms>::is_list, ARG_1_IS_NOT_PARM_LIST );
    STATIC_CHECK( !CTC_IsNullType( l[CTC::type<T>()] ), PARAMETER_NOT_FOUND );

    return l[CTC::type<T>()];
}

template< class T, typename Parms >
const T& GetParmStrict( const T&, const Parms &l )
{
    STATIC_CHECK( CTC::PL_Traits<Parms>::is_list, ARG_1_IS_NOT_PARM_LIST );
    STATIC_CHECK( !CTC_IsNullType( l[CTC::type<T>()] ), PARAMETER_NOT_FOUND );

    return l[CTC::type<T>()];
}


/* Create from ParmList methods (unused funcions - not tested)
 */
template< class T, class Parms > T* Create( const Parms &p)
{
    return CTC::Creator<T>::Create(p);
}

template< class T, class Parms > T* Create( CTC::type<T>, const Parms &p)
{
    return CTC::Creator<T>::Create(p);
}

template< class T, class A > T* CreateFromList( const ParmList_2<CTC::type<T>,A> &l )
{
    return CTC::Creator<T>::Create( l[CTC::arg<2>()] );
}


};//namespace Useless






/* Initialiser macros
 *---------------------
 * Use CTC_PL_InitParm(n) macros to ensure that requested parameter exists
 * or any replacement parameter exists
 *---------------------------------------
 * Example:
 *  Initialize hl-font. If there is no highlight font parameter on parm list
 *  then we want to check if there is normal font parameter. If nor isn't it,
 *  then compile time exception will occure with given message.
 *
 *  If we create such method:
 *
 *  template< class PL >
 *  C::Method( const CPT::Parms&, const PL &parm_list )
 *  {
 *      CTC_PL_InitParm2( _member_hl_font, parm_list, hilite, font, NO_FONT_SPECIFIED )
 *  }
 *
 *  What heppens when we call:
 *  1) c.Method( use_parms, _PL( hilite(my_font_2), font(my_font) ));
 *  2) c.Method( use_parms, _PL( font(my_font) ));
 *  3) c.Method( use_parms, _PL( skin(my_skin) ));
 *
 *  In case 1) all is ok, because we wanted hilite parm and we get it. Case 2)
 *  we didn't get hilite font, but there is parm replacement defined (font),
 *  and method will use it to initialize _member_hl_font member of C class.
 *  In case 3) compile time exception will occure saying:
 *  ERROR__NO_FONT_SPECIFIED
 *
 *  All macros initializes one given variable, from one given ParmList.
 *  CTC_PL_InitParm1, CTC_PL_InitParm2, CTC_PL_InitParm3, CTC_PL_InitParm4
 *  differ in number of replacement parameters.
 *
 *
-----------------------------------------------------------------------------*/
#define CTC_PL_InitParm1( variable, parm_list, type1, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
\
        STATIC_CHECK( (may_use_1), message ); \
\
        typedef CTC::IF< may_use_1, type1, \
                CTC::NullType >::type parm; \
\
        CTC::Assign(variable,parm_list[ CTC::type<parm>() ]);\
}

#define CTC_PL_InitParm2( variable, parm_list, type1, type2, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
        const bool may_use_2 = !CTC_IsNullType( parm_list[CTC::type<type2>()] ); \
\
        STATIC_CHECK( (may_use_1|may_use_2), message ); \
\
        typedef CTC::IF< may_use_1, type1, \
                CTC::IF< may_use_2, type2, \
                CTC::NullType >::type >::type parm; \
\
        CTC::Assign(variable,parm_list[ CTC::type<parm>() ]);\
}

#define CTC_PL_InitParm3( variable, parm_list, type1, type2, type3, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
        const bool may_use_2 = !CTC_IsNullType( parm_list[CTC::type<type2>()] ); \
        const bool may_use_3 = !CTC_IsNullType( parm_list[CTC::type<type3>()] ); \
\
        STATIC_CHECK( (may_use_1|may_use_2|may_use_3), message ); \
\
        typedef CTC::IF< may_use_1, type1, \
                CTC::IF< may_use_2, type2, \
                CTC::IF< may_use_3, type3, \
                CTC::NullType >::type >::type >::type parm; \
\
        CTC::Assign(variable,parm_list[ CTC::type<parm>() ]);\
}

#define CTC_PL_InitParm4( variable, parm_list, type1, type2, type3, type4, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
        const bool may_use_2 = !CTC_IsNullType( parm_list[CTC::type<type2>()] ); \
        const bool may_use_3 = !CTC_IsNullType( parm_list[CTC::type<type3>()] ); \
        const bool may_use_4 = !CTC_IsNullType( parm_list[CTC::type<type3>()] ); \
\
        STATIC_CHECK( (may_use_1|may_use_2|may_use_3|may_use_4), message ); \
\
        typedef CTC::IF< may_use_1, type1, \
                CTC::IF< may_use_2, type2, \
                CTC::IF< may_use_3, type3, \
                CTC::IF< may_use_4, type3, \
                CTC::NullType >::type >::type >::type >::type parm; \
\
        CTC::Assign(variable,parm_list[ CTC::type<parm>() ]);\
}



/* Check if parameter or replacement exists or not
 */
#define CTC_PL_CheckParm1( parm_list, type1, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
\
        STATIC_CHECK( (may_use_1), message ); \
}

#define CTC_PL_CheckParm2( parm_list, type1, type2, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
        const bool may_use_2 = !CTC_IsNullType( parm_list[CTC::type<type2>()] ); \
\
        STATIC_CHECK( (may_use_1|may_use_2), message ); \
}

#define CTC_PL_CheckParm3( parm_list, type1, type2, type3, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
        const bool may_use_2 = !CTC_IsNullType( parm_list[CTC::type<type2>()] ); \
        const bool may_use_3 = !CTC_IsNullType( parm_list[CTC::type<type3>()] ); \
\
        STATIC_CHECK( (may_use_1|may_use_2|may_use_3), message ); \
}

#define CTC_PL_CheckParm4( parm_list, type1, type2, type3, type4, message ) \
{ \
        STATIC_CHECK( CTC_IsParmList(parm_list), IS_NOT_PARM_LIST ); \
\
        const bool may_use_1 = !CTC_IsNullType( parm_list[CTC::type<type1>()] ); \
        const bool may_use_2 = !CTC_IsNullType( parm_list[CTC::type<type2>()] ); \
        const bool may_use_3 = !CTC_IsNullType( parm_list[CTC::type<type3>()] ); \
        const bool may_use_4 = !CTC_IsNullType( parm_list[CTC::type<type4>()] ); \
\
        STATIC_CHECK( (may_use_1|may_use_2|may_use_3|may_use_4), message ); \
}

#endif//__INCLUDED__USELESS_PARM_LIST_H__

