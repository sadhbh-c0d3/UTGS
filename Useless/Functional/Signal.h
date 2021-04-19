#ifndef __INCLUDED_USELESS_SIGNAL_H__
#define __INCLUDED_USELESS_SIGNAL_H__

/*    
 * $Id: Signal.h,v 1.24 2004/06/02 20:25:08 koolas Exp $
 *
 *  NAME
 *      Signal
 *
 *  PURPOSE
 *      Communication between objects of classes
 *
 * AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 */

/*! \ingroup Functors
 *  \defgroup Signals Signals
 */
/*! \ingroup Signals 
 *  \class Useless::Signal
 *  \par
 *  \verbatim
 *         
 * USAGE
 *      Tie2Signal( sig_ref, object_ptr, method_ptr ) default tie. This one will use 
 *                                                    most first signal arguments 
 *                                                    (i.e. if method gets 2 params 
 *                                                    and signal sends 4 then sending signal 
 *                                                    will cause execution of method with 
 *                                                    first two of this 4). THIS FUNCTION
 *                                                    AUTODETECTS ALL TYPES
 *                                                       
 *      Signal_1< int > usig - declare unary signal with argument of int type
 *      Signal_2< char, int > - declare binary signal
 *      Signal_3< int, char*, char* > - declare ternary signal
 *      Signal_4< int, int ,char*, float > qsig - declare tetrary signal
 *
 *      sig.TieVoid( object_ptr, method_ptr)
 *      sig.TieUnary( object_ptr, method_ptr, parameter_1 )
 *      sig.TieBinary( object_ptr, method_ptr, parm1, parm2 )
 *      sig.TieTernary( object_ptr, method_ptr, parm1, parm2, parm3 )
 *      sig.TieTetrary( object_ptr, method_ptr, parm1, parm2, parm3, parm4 )
 *
 *      Arg_N_Type sig.GetArgN() - method returns by value Nth its argument (N = 1..4)
 *                                 for example: Arg_1_Type x = sig.GetArg1();
 *      VariableHolder<Arg_N_Type> sig.GetArg1() - method returns by first its argument
 *                                 by VariableHolder
 *      Signal_N& sig.SetArgN(Arg_N_Type&) - method sets Nth parameter of signals execution
 *      
 *      Clear() - clear signal callback queue. Til next Tie no operation will occure on Send
 *      bool Empty() - check if nothing is tied to signal
 *      int Size() - return number of tied methods
 *      bool Disabled() - check if signal is disabled
 *      void Disable() - disable signal
 *      void Enable() - enable signal
 *
 *
 *      Sending Signal
 *      --------------
 *      Send() - method executes callbacks (all or several it depends on logic)
 *               if returned value is 'false' then signal was empty
 *
 *
 *   \endverbatim      
 *         
*/
#pragma warning(disable:4786)
#include <vector>
#include "Useless/Functional/PackedFunctor.h"
#include "Useless/Functional/Holder.h"
#include "Useless/Util/StaticCheck.h"
#include "Useless/Util/CommonReference.h"

namespace Useless {

class USELESS_API Signal 
{
public:
    Signal();
    virtual ~Signal();

    bool Send();   
    bool Schedule();

    void Clear()    { _packed_functors.clear(); }
    bool Empty()    { return ( _packed_functors.empty() )? true : false ;}
    size_t Size()     { return _packed_functors.size(); }

    bool IsDisabled() { return _is_disabled; }
    void Disable()    { _is_disabled = true; } 
    void Enable()     { _is_disabled = false; }

    Signal( const Signal &other );
    Signal& operator = ( const Signal &other );


    /////////////////////////////////////////////////////////////////////////
    //  Functor ting on functions
    //
    class FuncID
    {
    public:
        FuncID(): _id(0) {}
        void Untie()     { if(_sigref) _sigref->Untie(_id); _sigref=CommonReference<Signal>(); }
        operator bool () { return  _sigref.GetTarget(); }
        operator int  () { return  _id; }

    private:
        FuncID( const CommonReference<Signal> &s, unsigned long id): _sigref(s), _id(id) {}
        CommonReference<Signal> _sigref;
        unsigned long           _id;
        friend class Signal;
    };

    class UntieOnDestruction
    {
    public:
        UntieOnDestruction( const FuncID &fid ): fid(fid) {}
        ~UntieOnDestruction() { fid.Untie(); }

    private:
        FuncID fid;
        UntieOnDestruction( const UntieOnDestruction&);
        UntieOnDestruction& operator =( const UntieOnDestruction&);
    };

    void Untie(unsigned long id)
    {
        size_t i;
        size_t n = _packed_functors.size();
        for (i=0; i!= n; ++i)
        {
            if ( _packed_functors[i].first == id) break;
        }
        while( ++i < n )
        {
            _packed_functors[i-1] = _packed_functors[i];
        }
        _packed_functors.resize(n-1);
    }

    FuncID Tie( PkFunc pfunp);
    
    template< class CalleePtr, class Callee, class Ret  >
    FuncID TieVoid(const CalleePtr & calleePt , Ret(Callee::*method)() )
        {
            return Tie( PackFunctor( VoidCall(calleePt, method) ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret, class Arg1, class Type1 >
    FuncID TieUnary(const CalleePtr & calleePt , Ret(Callee::*method)(Arg1), const Type1 & arg )
        {
            return Tie( PackFunctor( UnaryCall(calleePt, method), arg ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret,
                  class Arg1, class Arg2,
              class Type1, class Type2 >
    FuncID TieBinary(const CalleePtr & calleePt ,
                    Ret(Callee::*method)(Arg1,Arg2),
                    const Type1 & arg1,
                    const Type2 & arg2 )
        {
            return Tie( PackFunctor( BinaryCall(calleePt, method), arg1, arg2 ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret,
                  class Arg1, class Arg2, class Arg3,
              class Type1, class Type2, class Type3 >
    FuncID TieTernary(const CalleePtr & calleePt ,
                    Ret(Callee::*method)(Arg1,Arg2,Arg3),
                    const Type1 & arg1,
                    const Type2 & arg2,
                      const Type3 & arg3 )
        {
            return Tie( PackFunctor( TernaryCall(calleePt, method), arg1, arg2, arg3 ) );    
        }
    
    template< class CalleePtr, class Callee, class Ret,
                  class Arg1, class Arg2, class Arg3, class Arg4,
              class Type1, class Type2, class Type3, class Type4 >
    FuncID TieTetrary(const CalleePtr & calleePt ,
                    Ret(Callee::*method)(Arg1,Arg2,Arg3,Arg4),
                    const Type1 & arg1,
                    const Type2 & arg2,
                      const Type3 & arg3,
                      const Type4 & arg4 )
        {
            return Tie( PackFunctor( TetraryCall(calleePt, method), arg1, arg2, arg3, arg4 ) );    
        }

    bool WasScheduled() const;

private:
    typedef std::pair<int, PkFunc> __Slot;    // ID + PackedFunctor
    typedef std::vector< __Slot > FunctorList;

    FunctorList             _packed_functors;
    CommonReference<Signal> _self_pointer;
    bool                    _is_disabled;

    /*-- Global Signal Shedule ( enqueued all unsent signals )--*/
    typedef std::vector< CommonReference<Signal> > ScheduleMap; 
    static ScheduleMap _sig_schedule;

public:
    /*-- Disppatch all unsent enqueued signals --*/
    static void Dispatch();

};

/*! \ingroup Util
 * Unary Signal
 */
template< class T >
class Signal_1 : public Signal
{
    public:
    T& GetArg1() { return _arg1; }
    VariableHolder<T> GetVar1() { return PackVariable2Holder(_arg1); }
    Signal_1& SetArg1(const T & val) { _arg1 = val; return (*this); }
    typedef VariableHolder<T> Var1Type;
    typedef T Arg1Type;
    bool Send_1(T arg_1)
    {
        SetArg1(arg_1);
        return Signal::Send();
    }
    bool Schedule_1(T arg_1)
    {
        SetArg1(arg_1);
        return Signal::Schedule();
    }
    
    private:        
    T _arg1;
};

/*! \ingroup Util
 * Binary Signal
 */
template< class T1, class T2 >
class Signal_2 : public Signal_1<T1>
{
    public:
    T2& GetArg2() { return _arg2; }
    VariableHolder<T2> GetVar2() { return PackVariable2Holder(_arg2); }
    Signal_2& SetArg2(const T2 & val) { _arg2 = val; return (*this); }
    typedef VariableHolder<T2> Var2Type;
    typedef T2 Arg2Type;
    bool Send_2(T1 arg_1, T2 arg_2)
    {
        SetArg1(arg_1);
        SetArg2(arg_2);
        return Signal::Send();
    }
    bool Schedule_2(T1 arg_1, T2 arg_2)
    {
        SetArg1(arg_1);
        SetArg2(arg_2);
        return Signal::Schedule();
    }
    
    
    private:        
    T2 _arg2;
};

/*! \ingroup Util
 * Ternary Signal
 */
template< class T1, class T2, class T3 >
class Signal_3 : public Signal_2<T1,T2>
{
    public:
    T3& GetArg3() { return _arg3; }
    VariableHolder<T3> GetVar3(){ return PackVariable2Holder(_arg3); }
    Signal_3& SetArg3(const T3 & val) { _arg3 = val; return (*this); }
    typedef VariableHolder<T3> Var3Type;
    typedef T3 Arg3Type;
    bool Send_3(T1 arg_1, T2 arg_2, T3 arg_3)
    {
        SetArg1(arg_1);
        SetArg2(arg_2);
        SetArg3(arg_3);
        return Signal::Send();
    }
    bool Schedule_3(T1 arg_1, T2 arg_2, T3 arg_3)
    {
        SetArg1(arg_1);
        SetArg2(arg_2);
        SetArg3(arg_3);
        return Signal::Schedule();
    }
    
    private:        
    T3 _arg3;
};

/*! \ingroup Util
 * Tetrary Signal
 */
template< class T1, class T2, class T3, class T4 >
class Signal_4 : public Signal_3<T1,T2,T3>
{
    public:
    T4& GetArg4() { return _arg4; }
    VariableHolder<T4> GetVar4() { return PackVariable2Holder(_arg4); }
    Signal_4& SetArg4(const T4 & val) { _arg4 = val; return (*this); }
    typedef VariableHolder<T4> Var4Type;
    typedef T4 Arg4Type;
    bool Send_4(T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4)
    {
        SetArg1(arg_1);
        SetArg2(arg_2);
        SetArg3(arg_3);
        SetArg4(arg_4);
        return Signal::Send();
    }
    bool Schedule_4(T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4)
    {
        SetArg1(arg_1);
        SetArg2(arg_2);
        SetArg3(arg_3);
        SetArg4(arg_4);
        return Signal::Schedule();
    }
    
    private:        
    T4 _arg4;
};

template< class CalleePtr, class Callee, class Ret >
inline Signal::FuncID Tie2Signal( Signal &sig, const CalleePtr & callee, Ret(Callee::*met)() )
{
    return sig.TieVoid( callee, met );    
}

template< class CalleePtr, class Callee, class Ret, class Arg1, class T1 >
inline Signal::FuncID Tie2Signal( Signal_1<T1> &sig, const CalleePtr & callee,
                 Ret(Callee::*met)(Arg1) )
{
    STATIC_CHECK( (const bool)(CTC::Conversion<Arg1, T1>::exists),
                  __SIGNAL_ARGS_DO_NOT_CAST_TO_FUNCTION_ARGUMENTS);

    return sig.TieUnary( callee, met, sig.GetVar1() );    
}
    
template< class CalleePtr, class Callee, class Ret, class Arg1, class Arg2, class T1, class T2 >
inline Signal::FuncID Tie2Signal( Signal_2<T1,T2> &sig, const CalleePtr & callee,
                 Ret(Callee::*met)(Arg1,Arg2) )
{
    STATIC_CHECK( (const bool)(CTC::Conversion<Arg1, T1>::exists) && 
                  (const bool)(CTC::Conversion<Arg2, T2>::exists),
                  __SIGNAL_ARGS_DO_NOT_CAST_TO_FUNCTION_ARGUMENTS);

    return sig.TieBinary( callee, met, sig.GetVar1(), sig.GetVar2() );    
}
    
template< class CalleePtr, class Callee, class Ret, class Arg1, class Arg2, class Arg3,
          class T1, class T2, class T3 >
inline Signal::FuncID Tie2Signal( Signal_3<T1,T2,T3> &sig, const CalleePtr & callee,
                 Ret(Callee::*met)(Arg1,Arg2,Arg3) )
{
    STATIC_CHECK( (const bool)(CTC::Conversion<Arg1, T1>::exists) && 
                  (const bool)(CTC::Conversion<Arg2, T2>::exists) &&
                  (const bool)(CTC::Conversion<Arg3, T3>::exists),
                  __SIGNAL_ARGS_DO_NOT_CAST_TO_FUNCTION_ARGUMENTS);

    return sig.TieTernary( callee, met, sig.GetVar1(), sig.GetVar2(), sig.GetVar3() );    
}
    
template< class CalleePtr, class Callee, class Ret, 
          class Arg1, class Arg2, class Arg3, class Arg4, class T1, class T2, class T3, class T4>
inline Signal::FuncID Tie2Signal( Signal_4<T1,T2,T3,T4> &sig, const CalleePtr & callee,
                 Ret(Callee::*met)(Arg1,Arg2,Arg3,Arg4) )
{
    STATIC_CHECK( (const bool)(CTC::Conversion<Arg1, T1>::exists) && 
                  (const bool)(CTC::Conversion<Arg2, T2>::exists) &&
                  (const bool)(CTC::Conversion<Arg3, T3>::exists) && 
                  (const bool)(CTC::Conversion<Arg4, T4>::exists),
                  __SIGNAL_ARGS_DO_NOT_CAST_TO_FUNCTION_ARGUMENTS);

    return sig.TieTetrary( callee, met, sig.GetVar1(), sig.GetVar2(),
                    sig.GetVar3(), sig.GetVar4() );    
}
    

/////////////////////////////////////////////////////////////////////////////////////////////
//  Throw COMPILATION ERROR
//

template< class CP, class C, class R, class A1 >
inline void Tie2Signal( Signal&, CP, R(C::*)(A1) ) { STATIC_CHECK( false, TOO_FEW_ARGS_IN_SIGNAL ); }
template< class CP, class C, class R, class A1, class A2 >
inline void Tie2Signal( Signal&, CP, R(C::*)(A1,A2) ) { STATIC_CHECK( false, TOO_FEW_ARGS_IN_SIGNAL ); }
template< class CP, class C, class R, class A1, class A2, class A3>
inline void Tie2Signal( Signal&, CP, R(C::*)(A1,A2,A3) ) { STATIC_CHECK( false, TOO_FEW_ARGS_IN_SIGNAL ); }
template< class CP, class C, class R, class A1, class A2, class A3, class A4>
inline void Tie2Signal( Signal&, CP, R(C::*)(A1,A2,A3,A4) ) { STATIC_CHECK( false, TOO_FEW_ARGS_IN_SIGNAL ); }
};

#endif //__INCLUDED_USELESS_SIGNAL_H__
