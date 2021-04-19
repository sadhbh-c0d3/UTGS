#ifndef __INCLUDED_USELESS_DUTY_LIST_H__
#define __INCLUDED_USELESS_DUTY_LIST_H__

#include "Useless/Functional/PseudoFunctor.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Functional/Holder.h"
#include <vector>

namespace Useless {

/*! \ingroup Util 
 *  List of activities to be executed later.
 *  Effect depends on passed class instance.
 */

//! \ingroup Signals
template< class BaseT >
class DutyList
{
public:
    void Execute( BaseT& instance )
    {
        int size=_duties.size();
        for (int i=0; i!=size; ++i)
        {
            (*_duties[i])(instance);
        }
    }

    bool Empty() { return _duties.empty(); }
    void Clear() { _duties.clear(); }

    template< class Callee, class RT >
        void AddDuty( RT (Callee::*mptr)() )
        {
            _duties.push_back( new PseudoFunctor_0<BaseT, RT (Callee::*)()>(mptr) );
        }

    template< class Callee, class RT, class T1, class A1>
        void AddDuty( RT (Callee::*mptr)(T1), const A1& a1 )
        {
            _duties.push_back( new PseudoFunctor_1<BaseT, RT (Callee::*)(T1),A1>(mptr,a1) );
        }

    template< class Callee, class RT, class T1, class T2, class A1, class A2>
        void AddDuty( RT (Callee::*mptr)(T1,T2), const A1& a1, const A2& a2 )
        {
            _duties.push_back( new PseudoFunctor_2<BaseT, RT (Callee::*)(T1,T2),A1,A2>(mptr,a1,a2) );
        }

    template< class Callee, class RT, class T1, class T2, class T3, class A1, class A2, class A3>
        void AddDuty( RT (Callee::*mptr)(T1,T2,T3), const A1& a1, const A2& a2, const A3& a3 )
        {
            _duties.push_back( new PseudoFunctor_3<BaseT, RT (Callee::*)(T1,T2,T3),A1,A2,A3>(mptr,a1,a2,a3) );
        }

    template< class Callee, class RT, class T1, class T2, class T3, class T4, class A1, class A2, class A3, class A4>
        void AddDuty( RT (Callee::*mptr)(T1,T2,T3,T4), const A1& a1, const A2& a2, const A3& a3, const A4& a4 )
        {
            _duties.push_back( new PseudoFunctor_4<BaseT, RT (Callee::*)(T1,T2,T3,T4),A1,A2,A3,A4>(mptr,a1,a2,a3,a4) );
        }


private:
    typedef PseudoFunctor<BaseT> __Duty;
    typedef SPointer< __Duty > _SP_Duty;
    typedef std::vector< _SP_Duty > __DutyList;
    __DutyList _duties;
};


};//namespace Useless

#endif//__INCLUDED_USELESS_DUTY_LIST_H__
