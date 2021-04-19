#ifndef __INCLUDED__USELESS_COMPOSITE_PREDICATE_H__
#define __INCLUDED__USELESS_COMPOSITE_PREDICATE_H__

#include "Useless/Functional/SmallComposite.h"

namespace Useless {

namespace { template< class P > struct get_Type { typedef typename P::Type Type; }; };
    
/* Generic Composite Predicate
---------------------------------------------*/
template< class Pr, class Tp = get_Type<Pr>::Type >
class CompositePredicate
{
public:
	typedef Pr P;
	typedef Tp Type;
    typedef bool RetType;

	CompositePredicate( const SmallComposite<Type> c ): _c(c) {}
	CompositePredicate( Type x ) { _c._value=x; }
	
	template< class Parms > bool operator()( const Parms &parms )
	{
		return _c.ApplyPred( P(), parms ); 
	}

	SmallComposite<Type> _c;
};

template< class P1, class P2, class T > 
CompositePredicate< Pred_AND<P1, P2>, T >
/* Operator & (AND)
---------------------------------------------------------------------------------*/
operator & ( const CompositePredicate<P1,T> &p1, const CompositePredicate<P2,T> &p2 )
{
	return SmallComposite<T>::Concatenate( p1._c, p2._c);
}

template< class P1, class P2, class T > 
CompositePredicate< Pred_OR<P1, P2>, T >
/* Operator | (OR)
---------------------------------------------------------------------------------*/
operator | ( const CompositePredicate<P1,T> &p1, const CompositePredicate<P2,T> &p2 )
{
	return SmallComposite<T>::Concatenate( p1._c, p2._c);
}

template< class P1, class P2, class T > 
CompositePredicate< Pred_XOR<P1, P2>, T >
/* Operator ^ (EXOR)
---------------------------------------------------------------------------------*/
operator ^ ( const CompositePredicate<P1,T> &p1, const CompositePredicate<P2,T> &p2 )
{
	return SmallComposite<T>::Concatenate( p1._c, p2._c);
}

};//namespace Useless
#endif//__INCLUDED__USELESS_COMPOSITE_PREDICATE_H__
