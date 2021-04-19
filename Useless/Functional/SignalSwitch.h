#ifndef __INCLUDED_USELESS_SIGNAL_SWITCH_H__
#define __INCLUDED_USELESS_SIGNAL_SWITCH_H__

#include "Useless/Functional/PackedFunctor.h"
#include "Useless/Util/Storage/AssocVector.h"

namespace Useless {

/*! \ingroup Signals
 *  Selects callback funcor associated with id. Can be used when signal
 *  sends one argument, and choice of function to be called depends on it.
 */
template< class T_ID >
class SignalSwitch
{
public:
    SignalSwitch() {}
    ~SignalSwitch() {}

    typedef SPointer< SignalSwitch<T_ID> >  Ptr;

    struct InitPtr : public Ptr
    {
        InitPtr(): Ptr(new SignalSwitch) {}
    };

    void Switch(const T_ID &arg);

    void Untie(const T_ID &arg)
    {
        _slots.Remove(arg);
    }

    void Tie( const T_ID &id, const PkFunc &pfunp) 
        { 
            _slots.Insert( id, pfunp );
        }
    
    template< class CalleePtr, class Callee, class Ret  >
    void TieVoid( const T_ID &id, const CalleePtr & calleePt , Ret(Callee::*method)() )
        {
            Tie( id, PackFunctor( VoidCall(calleePt, method) ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret, class Arg1, class Type1 >
    void TieUnary(const T_ID &id, const CalleePtr & calleePt , Ret(Callee::*method)(Arg1), const Type1 & arg )
        {
            Tie( id, PackFunctor( UnaryCall(calleePt, method), arg ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret,
                  class Arg1, class Arg2,
              class Type1, class Type2 >
    void TieBinary(const T_ID &id, 
                    const CalleePtr & calleePt , Ret(Callee::*method)(Arg1,Arg2),
                    const Type1 & arg1, const Type2 & arg2 )
        {
            Tie( id, PackFunctor( BinaryCall(calleePt, method), arg1, arg2 ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret,
                  class Arg1, class Arg2, class Arg3,
              class Type1, class Type2, class Type3 >
    void TieTernary(const T_ID &id, 
                    const CalleePtr & calleePt , Ret(Callee::*method)(Arg1,Arg2,Arg3),
                    const Type1 & arg1, const Type2 & arg2, const Type3 & arg3 )
        {
            Tie( id, PackFunctor( TernaryCall(calleePt, method), arg1, arg2, arg3 ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret,
                  class Arg1, class Arg2, class Arg3, class Arg4,
              class Type1, class Type2, class Type3, class Type4 >
    void TieTetrary(const T_ID &id, 
                    const CalleePtr & calleePt , Ret(Callee::*method)(Arg1,Arg2,Arg3,Arg4),
                    const Type1 & arg1, const Type2 & arg2,
                    const Type3 & arg3, const Type4 & arg4 )
        {
            Tie( id, PackFunctor( TetraryCall(calleePt, method), arg1, arg2, arg3, arg4 ) );    
        }

private:
    typedef AssocVector< T_ID, PkFunc > __Slots;
    __Slots _slots;
};


/* Implementation
-----------------------------------------------------------------------*/

template< class T_ID >

inline void SignalSwitch<T_ID>::Switch(const T_ID &arg)
{
    __Slots::ConstIterator iter;
    
    for ( _slots.Find(arg, &iter); !!iter && iter.Key()==arg; ++iter )
    { 
        (* ((PkFunc&)iter.Value()) )(); //little 'const' workaround
    }
}



};//namespace Useless
#endif//__INCLUDED_USELESS_SIGNAL_SWITCH_H__
