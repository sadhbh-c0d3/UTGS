#ifndef __INCLUDED__USELESS_CONDITIONAL_SIGNAL_H__
#define __INCLUDED__USELESS_CONDITIONAL_SIGNAL_H__

#include "Useless/Functional/Signal.h"
#include "Useless/Functional/ConditionalFunctor.h"

namespace Useless {

template< class Pred, class CPtr, class C, class R >
inline Signal::FuncID
ConditionalTie( const Pred &p, Signal &sig, const CPtr &cp, R(C::*m)() )
{
	return sig.Tie( MakeCondFunc( p, cp, m) );
}

template< class Pred, class CPtr, class C, class R, _TPL_1_, class A1 >
inline Signal::FuncID
ConditionalTie( const Pred &p, Signal_1<_TPL_TYPES_1> &sig, const CPtr &cp, R(C::*m)(A1) )
{
	return sig.Tie( MakeCondFunc( p, cp, m, sig.GetVar1()) );
}

template< class Pred, class CPtr, class C, class R, _TPL_2_, class A1, class A2 >
inline Signal::FuncID
ConditionalTie( const Pred &p, Signal_2<_TPL_TYPES_2> &sig, const CPtr &cp, R(C::*m)(A1,A2) )
{
	return sig.Tie( MakeCondFunc( p, cp, m, sig.GetVar1(), sig.GetVar2()) );
}

template< class Pred, class CPtr, class C, class R, _TPL_3_, class A1, class A2, class A3 >
inline Signal::FuncID
ConditionalTie( const Pred &p, Signal_3<_TPL_TYPES_3> &sig, const CPtr &cp, R(C::*m)(A1,A2,A3) )
{
	return sig.Tie( MakeCondFunc( p, cp, m, sig.GetVar1(), sig.GetVar2(), sig.GetVar3()) );
}

template< class Pred, class CPtr, class C, class R, _TPL_4_, class A1, class A2, class A3, class A4 >
inline Signal::FuncID
ConditionalTie( const Pred &p, Signal_4<_TPL_TYPES_4> &sig, const CPtr &cp, R(C::*m)(A1,A2,A3,A4) )
{
	return sig.Tie( MakeCondFunc( p, cp, m, sig.GetVar1(), sig.GetVar2(), sig.GetVar3(), sig.GetVar4()) );
}

};//namespace Useless
#endif//__INCLUDED__USELESS_CONDITIONAL_SIGNAL_H__
