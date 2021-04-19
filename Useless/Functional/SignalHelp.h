#ifndef __INCLUDED_USELESS_SIGNAL_HELP_H__
#define __INCLUDED_USELESS_SIGNAL_HELP_H__

#include "Useless/TemplateConfig.h"

namespace Useless {

#define USELESS_TEMPLATE_ARGUMENT( _T ) class _T
#define USELESS_FUNCTION_ARGUMENT( _T, _n ) const _T &_n


    template< class _A > struct ArgTie {
        template< USELESS_TEMPLATE_ARGUMENT(_S) >
		static _A apply( _S &s, const _A &a ) { return a; }
    };

    USELESS_SPECIALIZATION struct ArgTie< USELESS_TYPENAME CTC::arg<1> > {
        template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_A) >
		static typename _S::Var1Type apply( _S &s, const _A &a ) { return s.GetVar1(); }
    };

    USELESS_SPECIALIZATION struct ArgTie< USELESS_TYPENAME CTC::arg<2> > {
        template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_A) >
		static typename _S::Var2Type apply( _S &s, const _A &a ) { return s.GetVar2(); }
    };

    USELESS_SPECIALIZATION struct ArgTie< USELESS_TYPENAME CTC::arg<3> > {
        template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_A) >
		static typename _S::Var3Type apply( _S &s, const _A &a ) { return s.GetVar3(); }
    };

    USELESS_SPECIALIZATION struct ArgTie< USELESS_TYPENAME CTC::arg<4> > {
        template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_A) >
		static typename _S::Var4Type apply( _S &s, const _A &a ) { return s.GetVar4(); }
    };


    template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_P), USELESS_TEMPLATE_ARGUMENT(_R), USELESS_TEMPLATE_ARGUMENT(_C) > Signal::FuncID
        TieAt( _S &s, USELESS_FUNCTION_ARGUMENT(_P, p), _R (_C::*f)() )
        {
            return s.TieVoid( p, f );
        }

    template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_P), USELESS_TEMPLATE_ARGUMENT(_R), USELESS_TEMPLATE_ARGUMENT(_C), USELESS_TEMPLATE_ARGUMENT(A1), USELESS_TEMPLATE_ARGUMENT(B1) > Signal::FuncID
        TieAt( _S &s, USELESS_FUNCTION_ARGUMENT(_P, p), _R (_C::*f)(A1), USELESS_FUNCTION_ARGUMENT(B1,b1) )
        {
            return s.TieUnary( p, f, ArgTie<B1>::apply(s,b1) );
        }

    template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_P), USELESS_TEMPLATE_ARGUMENT(_R), USELESS_TEMPLATE_ARGUMENT(_C), USELESS_TEMPLATE_ARGUMENT(A1), USELESS_TEMPLATE_ARGUMENT(A2), USELESS_TEMPLATE_ARGUMENT(B1), USELESS_TEMPLATE_ARGUMENT(B2) > Signal::FuncID
        TieAt( _S &s, USELESS_FUNCTION_ARGUMENT(_P, p), _R (_C::*f)(A1,A2), USELESS_FUNCTION_ARGUMENT(B1,b1), USELESS_FUNCTION_ARGUMENT(B2,b2) )
        {
            return s.TieBinary( p, f, ArgTie<B1>::apply(s,b1), ArgTie<B2>::apply(s,b2) );
        }

    template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_P), USELESS_TEMPLATE_ARGUMENT(_R), USELESS_TEMPLATE_ARGUMENT(_C), USELESS_TEMPLATE_ARGUMENT(A1), USELESS_TEMPLATE_ARGUMENT(A2), USELESS_TEMPLATE_ARGUMENT(A3), USELESS_TEMPLATE_ARGUMENT(B1), USELESS_TEMPLATE_ARGUMENT(B2), USELESS_TEMPLATE_ARGUMENT(B3) > Signal::FuncID
        TieAt( _S &s, USELESS_FUNCTION_ARGUMENT(_P, p), _R (_C::*f)(A1,A2,A3), USELESS_FUNCTION_ARGUMENT(B1,b1), USELESS_FUNCTION_ARGUMENT(B2,b2), USELESS_FUNCTION_ARGUMENT(B3,b3) )
        {
            return s.TieTernary( p, f, ArgTie<B1>::apply(s,b1), ArgTie<B2>::apply(s,b2), ArgTie<B3>::apply(s,b3) );
        }

    template< USELESS_TEMPLATE_ARGUMENT(_S), USELESS_TEMPLATE_ARGUMENT(_P), USELESS_TEMPLATE_ARGUMENT(_R), USELESS_TEMPLATE_ARGUMENT(_C), USELESS_TEMPLATE_ARGUMENT(A1), USELESS_TEMPLATE_ARGUMENT(A2), USELESS_TEMPLATE_ARGUMENT(A3), USELESS_TEMPLATE_ARGUMENT(A4), USELESS_TEMPLATE_ARGUMENT(B1), USELESS_TEMPLATE_ARGUMENT(B2), USELESS_TEMPLATE_ARGUMENT(B3), USELESS_TEMPLATE_ARGUMENT(B4) > Signal::FuncID
        TieAt( _S &s, USELESS_FUNCTION_ARGUMENT(_P, p), _R (_C::*f)(A1,A2,A3,A4), USELESS_FUNCTION_ARGUMENT(B1,b1), USELESS_FUNCTION_ARGUMENT(B2,b2), USELESS_FUNCTION_ARGUMENT(B3,b3), USELESS_FUNCTION_ARGUMENT(B4,b4) )
        {
            return s.TieTetrary( p, f, ArgTie<B1>::apply(s,b1), ArgTie<B2>::apply(s,b2), ArgTie<B3>::apply(s,b3), ArgTie<B4>::apply(s,b4) );
        }

    static CTC::arg<1> _1_;
    static CTC::arg<2> _2_;
    static CTC::arg<3> _3_;
    static CTC::arg<4> _4_;
}; //Useless

#endif//__INCLUDED_USELESS_SIGNAL_HELP_H__
