#ifndef __INCLUDED__USELESS_CONDITIONAL_FUNCTOR_H__
#define __INCLUDED__USELESS_CONDITIONAL_FUNCTOR_H__

#include "Useless/Functional/PackedFunctor.h"
#include "Useless/Util/TemplateDefines.h"

namespace Useless {

template< class Pred, typename FunctorPtr >
class ConditionalFunctor : public PackedFunc
{
public:
	ConditionalFunctor( const Pred &pred, const FunctorPtr functor ):
	_pred(pred), _functor(functor) {}

    virtual void operator()() throw()
	{
		if ( _pred() ) { (*_functor)(); }
	}

private:
	Pred		_pred;
	FunctorPtr  _functor;
};

template< class P, class CP, class C, class R >
PkFunc::Ptr MakeCondFunc( const P &p, const CP &cp, R (C::*f)() )
{
	return new ConditionalFunctor< P, PkFunc >( p, PackFunctor( VoidCall(cp,f) ) );
}

template< class P, class CP, class C, class R, _TPL_1_, class A1 >
PkFunc::Ptr
MakeCondFunc( const P &p, const CP &cp, R (C::*f)(A1), _TPL_ARGS_1 )
{
	return new ConditionalFunctor< P, PkFunc >( p, PackFunctor( UnaryCall(cp,f), _TPL_PASS_ARGS_1 ) );
}

template< class P, class CP, class C, class R, _TPL_2_, class A1, class A2 >
PkFunc::Ptr
MakeCondFunc( const P &p, const CP &cp, R (C::*f)(A1,A2), _TPL_ARGS_2 )
{
	return new ConditionalFunctor< P, PkFunc >( p, PackFunctor( BinaryCall(cp,f), _TPL_PASS_ARGS_2 ) );
}

template< class P, class CP, class C, class R, _TPL_3_, class A1, class A2, class A3 >
PkFunc::Ptr
MakeCondFunc( const P &p, const CP &cp, R (C::*f)(A1,A2,A3), _TPL_ARGS_3 )
{
	return new ConditionalFunctor< P, PkFunc >( p, PackFunctor( TernaryCall(cp,f), _TPL_PASS_ARGS_3 ) );
}

template< class P, class CP, class C, class R, _TPL_4_, class A1, class A2, class A3, class A4 >
PkFunc::Ptr
MakeCondFunc( const P &p, const CP &cp, R (C::*f)(A1,A2,A3,A4), _TPL_ARGS_4 )
{
	return new ConditionalFunctor< P, PkFunc >( p, PackFunctor( TetraryCall(cp,f), _TPL_PASS_ARGS_4 ) );
}


};//namespace Useless
#endif//__INCLUDED__USELESS_CONDITIONAL_FUNCTOR_H__
