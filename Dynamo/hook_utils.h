#ifndef __INCLUDED_DYNAMO_HOOK_UTILS_H__
#define __INCLUDED_DYNAMO_HOOK_UTILS_H__

#include "Dynamo/classes/CInterface.h"

namespace Dynamo {

template< class _H, class _F, class _A >
    struct HookImp_Function : CInterface, virtual _H
    {
        _F _f;
        
        HookImp_Function( _F f ): _f(f)
        {
        }

        void Apply( _A a )
        {
            _f(a);
        }
    };

template< class _H, class _P, class _F, class _A >
    struct HookImp_Method : CInterface, virtual _H
    {
        _P _p;
        _F _f;
        
        HookImp_Method( _P p, _F f ): _p(p), _f(f)
        {
        }

        void Apply( _A a )
        {
            ((*_p).*_f)(a);
        }
    };

template< class _H, class _A, class _FR, class _FA >
    HookImp_Function< _H, _FR(*)(_FA), _A > * make_hook_f( _FR(*f)(_FA) )
    {
        return new HookImp_Function< _H, _FR(*)(_FA), _A >(f);
    }

template< class _H, class _A, class _CP, class _C, class _FR, class _FA >
    HookImp_Method< _H, _CP, _FR(_C::*)(_FA), _A > * make_hook_mp( _CP p, _FR(_C::*f)(_FA) )
    {
        return new HookImp_Method< _H, _CP, _FR(_C::*)(_FA), _A >(p,f);
    }

};//Dynamo

#endif//__INCLUDED_DYNAMO_HOOK_UTILS_H__
