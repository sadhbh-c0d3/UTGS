#ifndef __INLCUDED__USELESS__STATIC_EXPRESSION_H__
#define __INLCUDED__USELESS__STATIC_EXPRESSION_H__

#include "Useless/TemplateConfig.h"
#include "Useless/Util/CTC.h"

namespace Useless {

/* Evaluator unary function unit
------------------------------------------------------*/
template< class T, class A, class UPType >
struct Evaluator
{
    typedef T type;
    typedef A arg_type;
    //typedef UPType upper_type;

    //-- type operator ()( const arg_type &a ) const; USER function
};

/* Expression binary operator unit
------------------------------------------------------*/
template< class T, class A, class UPType >
struct Accumulator
{
    typedef T type;
    typedef A arg_type;
    //typedef UPType upper_type;

    type operator ()() { return _x; }
    //-- bool operator +=( arg_type a ); USER function tells if evaluation should continue
    //-- Acumulator(): _x(...) {} USER initialization is nessesary

    type _x;
};

/* Expression binary unit
------------------------------------------------------*/
template< class LHS, class RHS, class Accum, class A >
struct BinaryNode : public Evaluator< typename Accum::arg_type, A, USELESS_STRUCT BinaryNode<LHS, RHS, Accum, A> >
{
    typedef LHS lhs_t;
    typedef RHS rhs_t;

    BinaryNode( const LHS &lhs, const RHS rhs ): _lhs(lhs), _rhs(rhs) {}

    type operator ()( const arg_type &a ) const
    {
        Accum accum;
        if ( accum += _lhs(a) )
        {
            accum += _rhs(a);
        }
        return accum();
    }

    lhs_t _lhs;
    rhs_t _rhs;
};

template< class Accum, class LHS, class RHS >
BinaryNode< LHS, RHS, Accum, typename LHS::arg_type >
/* Concatenate two expressions
-----------------------------------------------------------------------------------------*/
BinaryNode_Concatenate( const LHS &lhs, const RHS &rhs )
{
    using namespace CTC;
    STATIC_CHECK( (TypeCompare< typename LHS::type, typename Accum::arg_type >::equal), TYPES_MUST_BE_EQUAL );
    STATIC_CHECK( (TypeCompare< typename RHS::type, typename Accum::arg_type >::equal), TYPES_MUST_BE_EQUAL );
    STATIC_CHECK( (TypeCompare< typename LHS::type, typename RHS::type>::equal), TYPES_MUST_BE_EQUAL );
    STATIC_CHECK( (TypeCompare< typename LHS::arg_type, typename RHS::arg_type>::equal), TYPES_MUST_BE_EQUAL );

    return BinaryNode< LHS, RHS, Accum, typename LHS::arg_type >( lhs, rhs );
}

/* Standard logic operators
=======================================================================================*/

/* Standard logic AND accumulator
----------------------------------------------------------*/
struct Accum_AND : public Accumulator< bool, bool, Accum_AND >
{
    Accum_AND() { _x=1; }

    bool operator +=( arg_type a ) { return _x&=a; }
};

template< class T1, class T2, class T3, class T4 >
BinaryNode< T3, T4, Accum_AND, T2 >
//-- Use to create (x && y) like expression
operator && ( const Evaluator<T1,T2,T3> &lhs, const Evaluator<T1,T2,T4> &rhs )
{
    return BinaryNode_Concatenate<Accum_AND>( (T3&)lhs, (T4&)rhs );
}


/* Standard logic OR accumulator
----------------------------------------------------------*/
struct Accum_OR : public Accumulator< bool, bool, Accum_OR >
{
    Accum_OR() { _x=0; }

    bool operator +=( arg_type a ) { return !( _x|=a ); }
};

template< class T1, class T2, class T3, class T4 >
BinaryNode< T3, T4, Accum_OR, T2 >
//-- Use to create (x || y) like expression
operator || ( const Evaluator<T1,T2,T3> &lhs, const Evaluator<T1,T2,T4> &rhs )
{
    return BinaryNode_Concatenate<Accum_OR>( (T3&)lhs, (T4&)rhs );
}


/* Standard logic NOT evaluator
----------------------------------------------------------*/
struct Accum_NOT : public Accumulator< bool, bool, Accum_NOT >
{
    Accum_NOT() { _x=1; }

    bool operator +=( arg_type a ) { _x=!a; return false; }
};

/* Dummy no operation node
---------------------------------------------------------*/
template< class T, class A >
struct NopNode : public Evaluator< T, A, USELESS_STRUCT NopNode<T, A> >
{
    type operator ()( const arg_type &a ) const { return type(); }
};

template< class T1, class T2, class T3 >
BinaryNode< T3, NopNode<T1,T2>, Accum_NOT, T2 >
//-- Use to create (!x) like expression
operator !( const Evaluator<T1,T2,T3> &lhs )
{
    return BinaryNode_Concatenate<Accum_NOT>( (T3&)lhs, NopNode<T1,T2>() );
}


};//namespce Useless
#endif//__INLCUDED__USELESS__STATIC_EXPRESSION_H__
