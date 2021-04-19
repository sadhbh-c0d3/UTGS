#ifndef __INCLUDED_USELESS_FUNCTOR_H__
#define __INCLUDED_USELESS_FUNCTOR_H__

/*
 *  $Id: Functor.h,v 1.6 2003/01/20 20:40:58 koolas Exp $
 *
 *  NAME
 *      Functor
 *
 *  PURPOSE
 *      Universal callbacks
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *      Andrei Alexandrescu
 *      Andreas Huber (andreas@huber.net)
 *
 *  USAGE
 *      Useless::FVoid::Ptr                         Useless::VoidCall       ( Callee *, (Callee::*)())
 *      Useless::FVoidR<RT>::Ptr                    Useless::VoidRCall      ( Callee *, (Callee::*)(), RT&)
 *      Useless::FUnary<T>::Ptr                     Useless::UnaryCall      ( Callee *, (Callee::*)(T))
 *      Useless::FUnaryR<T,RT>::Ptr                 Useless::UnaryRCall     ( Callee *, (Callee::*)(T), RT&)
 *      Useless::FBinary<T1,T2>::Ptr                Useless::BinaryCall     ( Callee *, (Callee::*)(T1,T2))
 *      Useless::FBinaryR<T1,T2,RT>::Ptr            Useless::BinaryRCall    ( Callee *, (Callee::*)(T1,T2), RT&)
 *      Useless::FTernary<T1,T2,T3>::Ptr            Useless::TernaryCall    ( Callee *, (Callee::*)(T1,T2,T3))
 *      Useless::FTernaryR<T1,T2,T3,RT>::Ptr        Useless::TernaryRCall   ( Callee *, (Callee::*)(T1,T2,T3), RT&)
 *      Useless::FTetrary<T1,T2,T3,T4>::Ptr         Useless::TetraryCall    ( Callee *, (Callee::*)(T1,T2,T3,T4))
 *      Useless::FTetraryR<T1,T2,T3,T4,RT>::Ptr     Useless::TetraryRCall   ( Callee *, (Callee::*)(T1,T2,T3,T4), RT&)
 */

#include "Useless/Util/SPointer.h"
#include "Useless/Util/StaticCheck.h"

namespace Useless
{
/////////////////////////////////////////////////////////////////
//    Define main functor types
//    
//    Defined: void operator(T1,T2,...,Tn)
//    ===================================        
//    Functor_0         
//    Functor_1 <Type>
//    Functor_2 <T1,T2>             
//    Functor_3 <T1,T2,T3>        
//    Functor_4 <T1,T2,T3,T4>
//
//    Defined: RT operator(T1,T2,...,Tn)
//    ===================================        
//    Functor_0R <ReturnType>        
//    Functor_1R <Type>
//    Functor_2R <T1,T2,RT>             
//    Functor_3R <T1,T2,T3,RT>        
//    Functor_4R <T1,T2,T3,T4,RT>
//

/*! \defgroup Functors Functors
 *  \ingroup Util
 *  They are adaptable for any kind of member function.
 */

/*! \ingroup Functors
 */
/*@{*/

template< class _Type > _Type * GetPointer( _Type *ptr )
{
	return ptr;
}

template< class _Type > _Type * GetPointer( SPointer< _Type > &ptr )
{
	return ptr.get();
}


/*! Base Functor*/
class Functor {};

/*! Noarg Functor */
 class Functor_0 : public Functor
{
    public:
        virtual ~Functor_0() {}
        virtual void operator()() throw() = 0;    
};

/*! Noarg Functor returning value */
template< class ReturnType >
class Functor_0R : public Functor
{
    public:
        virtual ~Functor_0R() {}
        virtual ReturnType operator()() throw() = 0;
};

/*! Unary Functor*/
template< class Arg_1_Type >
class Functor_1 : public Functor
{
    public:
        virtual ~Functor_1() {}
        virtual void operator()(Arg_1_Type) throw() = 0;    
};

/*! Unary Functor returning value*/
template< class Arg_1_Type, class ReturnType >
class Functor_1R : public Functor
{
    public:
        virtual ~Functor_1R() {}
        virtual ReturnType operator()(Arg_1_Type) throw() = 0;
};

/*! Binary Functor*/
template< class Arg_1_Type, class Arg_2_Type >
class Functor_2 : public Functor
{
    public:
        virtual ~Functor_2() {}
        virtual void operator()(Arg_1_Type, Arg_2_Type) throw() = 0;    
};

/*! Binary Functor returning value*/
template< class Arg_1_Type, class Arg_2_Type, class ReturnType >
class Functor_2R : public Functor
{
    public:
        virtual ~Functor_2R() {}
        virtual ReturnType operator()(Arg_1_Type, Arg_2_Type) throw() = 0;
};

/*! Ternary Functor*/
template< class Arg_1_Type, class Arg_2_Type, class Arg_3_Type >
class Functor_3 : public Functor
{
    public:
        virtual ~Functor_3() {}
        virtual void operator()(Arg_1_Type, Arg_2_Type, Arg_3_Type) throw() = 0;    
};

/*! Ternary Functor returning value*/
template< class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class ReturnType >
class Functor_3R : public Functor
{
    public:
        virtual ~Functor_3R() {}
        virtual ReturnType operator()(Arg_1_Type, Arg_2_Type, Arg_3_Type) throw() = 0;
};

/*! Tetrary Functor*/
template< class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type >
class Functor_4 : public Functor
{
    public:
        virtual ~Functor_4() {}
        virtual void operator()(Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type) throw() = 0;    
};

/*! Tetrary Functor returning value*/
template< class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type, class ReturnType >
class Functor_4R : public Functor
{
    public:
        virtual ~Functor_4R() {}
        virtual ReturnType operator()(Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type) throw() = 0;
};



///////////////////////////////////////////////////////////////
//    Define specialization of functors depending on pointers
//    to subject and its method
//
template< class CalleePtr, class MemFunPtr >
class Functor_0_Tpl : public Functor_0
{
    public:    
        Functor_0_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual void operator()() throw() 
        {
            ((*const_cast<CalleePtr&>(_subject_p)).*_method_p)();    
        }    

    private:
    CalleePtr    _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class ReturnType >
class Functor_0R_Tpl : public Functor_0R <ReturnType>
{
    public:    
        Functor_0R_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual ReturnType operator()() throw()
        {
            return ((*_subject_p).*_method_p)();    
        }    

    private:
    CalleePtr    _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class Arg_1_Type >
class Functor_1_Tpl : public Functor_1< Arg_1_Type >
{
    public:    
        Functor_1_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual void operator()(Arg_1_Type arg) throw()
        {
            ((*const_cast<CalleePtr&>(_subject_p)).*_method_p)(arg);//GCC W/A ?? 
        }    

    private:
    CalleePtr _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class Arg_1_Type, class ReturnType >
class Functor_1R_Tpl : public Functor_1R< Arg_1_Type, ReturnType >
{
    public:    
        Functor_1R_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual ReturnType operator()(Arg_1_Type arg) throw()
        {
            return ((*_subject_p).*_method_p)(arg);    
        }    

    private:
    CalleePtr    _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class Arg_1_Type, class Arg_2_Type >
class Functor_2_Tpl : public Functor_2< Arg_1_Type, Arg_2_Type >
{
    public:    
        Functor_2_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual void operator()(Arg_1_Type arg1, Arg_2_Type arg2) throw()
        {
            (*(_subject_p).*_method_p)(arg1, arg2);    
        }    

    private:
    CalleePtr _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class Arg_1_Type, class Arg_2_Type, class ReturnType >
class Functor_2R_Tpl : public Functor_2R< Arg_1_Type, Arg_2_Type, ReturnType >
{
    public:    
        Functor_2R_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual ReturnType operator()(Arg_1_Type arg1, Arg_2_Type arg2) throw()
        {
            return ((*_subject_p).*_method_p)(arg1, arg2);    
        }    

    private:
    CalleePtr    _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type >
class Functor_3_Tpl : public Functor_3< Arg_1_Type, Arg_2_Type, Arg_3_Type >
{
    public:    
        Functor_3_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual void operator()(Arg_1_Type arg1, Arg_2_Type arg2, Arg_3_Type arg3) throw()
        {
            ((*_subject_p).*_method_p)(arg1, arg2, arg3);    
        }    

    private:
    const CalleePtr    _subject_p;
    const MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class ReturnType >
class Functor_3R_Tpl : public Functor_3R< Arg_1_Type, Arg_2_Type, Arg_3_Type, ReturnType >
{
    public:    
        Functor_3R_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual ReturnType operator()(Arg_1_Type arg1, Arg_2_Type arg2, Arg_3_Type arg3) throw()
        {
            return ((*_subject_p).*_method_p)(arg1, arg2, arg3);    
        }    

    private:
    CalleePtr    _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, 
          class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, 
          class Arg_4_Type >
class Functor_4_Tpl : public Functor_4< Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type >
{
    public:    
        Functor_4_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual void operator()(Arg_1_Type arg1, Arg_2_Type arg2, Arg_3_Type arg3, Arg_4_Type arg4) throw()
        {
            ((*_subject_p).*_method_p)(arg1, arg2, arg3, arg4);    
        }    

    private:
    CalleePtr    _subject_p;
    MemFunPtr _method_p;
};

template< class CalleePtr, class MemFunPtr, 
          class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type, 
            class ReturnType >
class Functor_4R_Tpl : public Functor_4R< Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type, ReturnType >
{
    public:    
        Functor_4R_Tpl
            (
             CalleePtr subject_p,
             MemFunPtr method_p
             ) :
            _subject_p(subject_p),
            _method_p(method_p)
        {
        }
            
        virtual ReturnType operator()(Arg_1_Type arg1, Arg_2_Type arg2, Arg_3_Type arg3, Arg_4_Type arg4) throw()
        {
            return ((*_subject_p).*_method_p)(arg1, arg2, arg3, arg4);    
        }    

    private:
    CalleePtr _subject_p;
    MemFunPtr _method_p;
};

/////////////////////////////////////////////////////////////////////////
//    Define smart-pointer types 
//
//    Calls without retrun
//        Useless::FVoid::Ptr
//        Useless::FUnary<T>::Ptr
//        Useless::FBinary<T1,T2>::Ptr
//        Useless::FTernary<T1,T2,T3>::Ptr
//        Useless::FTetrary<T1,T2,T3,T4>::Ptr
//
//    Calls with return types
//        Useless::FVoidR<RT>::Ptr 
//        Useless::FUnaryR<T,RT>::Ptr 
//        Useless::FBinaryR<T1,T2,RT>::Ptr
//        Useless::FTernaryR<T1,T2,T3>::Ptr
//        Useless::FTetraryR<T1,T2,T3,T4>::Ptr
//

struct FVoid{ typedef Useless::SPointer < Useless::Functor_0 > Ptr ; };

template < class TRType >
struct FVoidR{ typedef Useless::SPointer < Useless::Functor_0R < TRType > > Ptr ; };

template < class TType >
struct FUnary{ typedef Useless::SPointer < Useless::Functor_1 < TType > > Ptr ; };

template < class TType, class TRType >
struct FUnaryR{ typedef Useless::SPointer < Useless::Functor_1R < TType, TRType > > Ptr ; };

template < class TType1, class TType2 >
struct FBinary{ typedef Useless::SPointer < Useless::Functor_2 < TType1, TType2 > > Ptr ; };

template < class TType1, class TType2, class TRType >
struct FBinaryR{ typedef Useless::SPointer < Useless::Functor_2R < TType1, TType2, TRType > > Ptr ; };

template < class TType1, class TType2, class TType3 >
struct FTernary{ typedef Useless::SPointer < Useless::Functor_3 < TType1, TType2, TType3 > > Ptr ; };

template < class TType1, class TType2, class TType3, class TRType >
struct FTernaryR{ typedef Useless::SPointer < Useless::Functor_3R < TType1, TType2, TType3, TRType > > Ptr ; };

template < class TType1, class TType2, class TType3, class TType4 >
struct FTetrary{ typedef Useless::SPointer < Useless::Functor_4 < TType1, TType2, TType3, TType4 > > Ptr ; };

template < class TType1, class TType2, class TType3, class TType4, class TRType >
struct FTetraryR{ typedef Useless::SPointer < Useless::Functor_4R < TType1, TType2, TType3, TType4, TRType > > Ptr ; };


/////////////////////////////////////////////////////////////////////////
//    Define creators
//    
//    Compiler will deduce appriopriate template arguments
//

template < class CalleePtr, class Callee, class Ret >
inline FVoid::Ptr VoidCall( const CalleePtr& subject, Ret (Callee::*method)() )
{
    return new Functor_0_Tpl< CalleePtr, Ret (Callee::*)() >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret >
inline typename FVoid::Ptr VoidCall( const CalleePtr& subject, Ret (Callee::*method)() const )
{
    return new Functor_0_Tpl< CalleePtr, Ret (Callee::*)() const>( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class RetType >
inline typename FVoidR<RetType>::Ptr VoidRCall( const CalleePtr& subject, Ret (Callee::*method)(), const RetType& )
{
    return new Functor_0R_Tpl< CalleePtr, Ret (Callee::*)(), RetType >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class RetType >
inline typename FVoidR<RetType>::Ptr VoidRCall( const CalleePtr& subject, Ret (Callee::*method)() const, const RetType& )
{
    return new Functor_0R_Tpl< CalleePtr, Ret (Callee::*)() const, RetType >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type >
inline typename FUnary<Arg_1_Type>::Ptr UnaryCall( const CalleePtr& subject, Ret (Callee::*method)(Arg_1_Type) )
{
    return new Functor_1_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type), Arg_1_Type >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type >
inline typename FUnary<Arg_1_Type>::Ptr UnaryCall( const CalleePtr& subject, Ret (Callee::*method)(Arg_1_Type) const )
{
    return new Functor_1_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type) const, Arg_1_Type >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class RetType >
inline typename FUnaryR<Arg_1_Type,RetType>::Ptr UnaryRCall( const CalleePtr& subject, Ret (Callee::*method)(Arg_1_Type), const RetType& )
{
    return new Functor_1R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type), Arg_1_Type, RetType >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class RetType >
inline typename FUnaryR<Arg_1_Type,RetType>::Ptr UnaryRCall( const CalleePtr& subject, Ret (Callee::*method)(Arg_1_Type) const, const RetType& )
{
    return new Functor_1R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type) const, Arg_1_Type, RetType >( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type>
inline typename FBinary<Arg_1_Type, Arg_2_Type>::Ptr BinaryCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type) 
    )
{
    return new 
        Functor_2_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type), Arg_1_Type, Arg_2_Type >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type>
inline typename FBinary<Arg_1_Type, Arg_2_Type>::Ptr BinaryCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type) const
    )
{
    return new 
        Functor_2_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type) const, Arg_1_Type, Arg_2_Type >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class RetType>
inline typename FBinaryR<Arg_1_Type, Arg_2_Type, RetType>::Ptr BinaryRCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type),
     const RetType& 
    )
{
    return new 
        Functor_2R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type), Arg_1_Type, Arg_2_Type, RetType >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class RetType>
inline typename FBinaryR<Arg_1_Type, Arg_2_Type, RetType>::Ptr BinaryRCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type) const,
     const RetType& 
    )
{
    return new 
        Functor_2R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type) const, Arg_1_Type, Arg_2_Type, RetType >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type>
inline typename FTernary<Arg_1_Type, Arg_2_Type, Arg_3_Type>::Ptr TernaryCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type) 
    )
{
    return new 
        Functor_3_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type), Arg_1_Type, Arg_2_Type, Arg_3_Type >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type>
inline typename FTernary<Arg_1_Type, Arg_2_Type, Arg_3_Type>::Ptr TernaryCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type) const
    )
{
    return new 
        Functor_3_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type) const, Arg_1_Type, Arg_2_Type, Arg_3_Type >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class RetType>
inline typename FTernaryR<Arg_1_Type, Arg_2_Type, Arg_3_Type, RetType>::Ptr TernaryRCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type),
     const RetType& 
    )
{
    return new 
        Functor_3R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type), Arg_1_Type, Arg_2_Type, Arg_3_Type, RetType >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class RetType>
inline typename FTernaryR<Arg_1_Type, Arg_2_Type, Arg_3_Type, RetType>::Ptr TernaryRCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type) const,
     const RetType& 
    )
{
    return new 
        Functor_3R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type) const, Arg_1_Type, Arg_2_Type, Arg_3_Type, RetType >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type>
inline typename FTetrary<Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type>::Ptr TetraryCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type) 
    )
{
    return new 
        Functor_4_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type),
            Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type>
inline typename FTetrary<Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type>::Ptr TetraryCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type) const 
    )
{
    return new 
        Functor_4_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type) const,
            Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret,
           class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type, 
           class RetType>
inline typename FTetraryR<Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type, RetType>::Ptr TetraryRCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type),
     const RetType& 
    )
{
    return new 
        Functor_4R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type),
               Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type, RetType >
        ( subject, method );    
}

template < class CalleePtr, class Callee, class Ret,
           class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type, 
           class RetType>
inline typename FTetraryR<Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type, RetType>::Ptr TetraryRCall
    ( 
     const CalleePtr& subject, 
     Ret (Callee::*method)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type) const,
     const RetType& 
    )
{
    return new 
        Functor_4R_Tpl< CalleePtr, Ret (Callee::*)(Arg_1_Type,Arg_2_Type,Arg_3_Type,Arg_4_Type) const,
               Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type, RetType >
        ( subject, method );    
}

/*@}*/

/////////////////////////////////////////////////////////////////////////////////////////////
//  Throw COMPILATION ERROR
//
/*
template<class A> inline void* VoidCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* VoidRCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* UnaryCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* UnaryRCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* BinaryCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* BinaryRCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* TernaryCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* TernaryRCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* TetraryCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
template<class A> inline void* TetraryRCall(A,...) { STATIC_CHECK( false, WRONG_METHOD_PTR ); }
*/
};
#endif // __INCLUDED_USELESS_FUNCTOR_H__ 
