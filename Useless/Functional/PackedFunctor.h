#ifndef __INCLUDED_USELESS_PACKEDFUNCTOR_H__
#define __INCLUDED_USELESS_PACKEDFUNCTOR_H__

/*
**  $Id: PackedFunctor.h,v 1.9 2003/01/20 20:40:58 koolas Exp $
**
**  NAME
**      PackedFunctor
**
**  PURPOSE
**      Universal asynchronious callbacks
**
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**      Andreas Huber (andreas@huber.net)
**
**
**  USAGE
**      Useless::PkFunc::Ptr    Useless::PackFunctor( Any_Functor_Type, T1..T4 )
**      Useless::PkFuncR::Ptr    Useless::PackFunctorR( Any_FunctorR_Type, T1..T4, (RT)0 )
**    
**  EXAMPLE
**      //    Create UnaryCall "func"
**      Useless::FUnary<float>::Ptr func = Useless::UnaryCall(&subject,&class::method);
**
**      //    Pack this with constant argument (unsigned int)15 into PackedFunc "my_package"
**      Useless::PkFunc::Ptr my_package = Useless::PackFunctor( func, (unsigned int)15 );
**
*/

#include "Useless/Util/SPointer.h"
#include "Functor.h"

namespace Useless
{

/*! \ingroup Functors
 *  Packed Functor
 */
class PackedFunc
{
    public:    
        virtual ~PackedFunc() {}
        virtual void operator()() throw() = 0;    
};

/*! \ingroup Functors
 *  Packed Functor returning value
 */
template< class ReturnType >
class PackedFuncR
{
    public:
        virtual ~PackedFuncR() {}
        virtual ReturnType operator()() throw() = 0;
        operator PackedFunc& () { return (*(PackedFunc*)this) ;}    
};

template< class FunctorPtr  >
class PackedVoidFunc : public PackedFunc
{
    public:
        PackedVoidFunc( const FunctorPtr& action) : _action(action) {}    
        virtual void operator()() throw() 
        {
            (*_action)();    
        }
    private:
        FunctorPtr _action;
};

template< class FunctorPtr, class ReturnType  >
class PackedVoidFuncR : public PackedFuncR <ReturnType>
{
    public:
        PackedVoidFuncR( const FunctorPtr& action) : _action(action) {}    
        virtual ReturnType operator()() throw()
        {
            return (*_action)();    
        }
    private:
        FunctorPtr _action;
};

template< class FunctorPtr, class Arg_1_Type  >
class PackedUnaryFunc : public PackedFunc
{
    public:
        PackedUnaryFunc( const FunctorPtr& action, const Arg_1_Type arg1) :
            _action(action), _arg1(arg1) {}    
        virtual void operator()() throw() 
        {
            (*_action)(_arg1); 
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
};

template< class FunctorPtr, class Arg_1_Type, class ReturnType  >
class PackedUnaryFuncR : public PackedFuncR <ReturnType>
{
    public:
        PackedUnaryFuncR( const FunctorPtr& action, const Arg_1_Type arg1) :
            _action(action), _arg1(arg1) {}    
        virtual ReturnType operator()() throw() 
        {
            return (*_action)(_arg1);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
};

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type  >
class PackedBinaryFunc : public PackedFunc
{
    public:
        PackedBinaryFunc( const FunctorPtr& action, const Arg_1_Type& arg1, const Arg_2_Type& arg2 ) :
            _action(action), _arg1(arg1), _arg2(arg2) {}    
        virtual void operator()() throw() 
        {
            (*_action)(_arg1,_arg2);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
        Arg_2_Type _arg2;    
};

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class ReturnType  >
class PackedBinaryFuncR : public PackedFuncR <ReturnType>
{
    public:
        PackedBinaryFuncR( const FunctorPtr& action, const Arg_1_Type& arg1, const Arg_2_Type& arg2 ) :
            _action(action), _arg1(arg1), _arg2(arg2) {}    
        virtual ReturnType operator()() throw() 
        {
            return (*_action)(_arg1,_arg2);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
        Arg_2_Type _arg2;    
};

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type  >
class PackedTernaryFunc : public PackedFunc
{
    public:
        PackedTernaryFunc( const FunctorPtr& action,
                              const Arg_1_Type& arg1,
                           const Arg_2_Type& arg2, 
                           const Arg_3_Type& arg3) :
            _action(action), _arg1(arg1), _arg2(arg2), _arg3(arg3) {}    
        virtual void operator()() throw() 
        {
            (*_action)(_arg1,_arg2,_arg3);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
        Arg_2_Type _arg2;    
        Arg_3_Type _arg3;    
};

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class ReturnType  >
class PackedTernaryFuncR : public PackedFuncR <ReturnType>
{
    public:
        PackedTernaryFuncR(const FunctorPtr& action,
                              const Arg_1_Type& arg1,
                           const Arg_2_Type& arg2, 
                           const Arg_3_Type& arg3) :
            _action(action), _arg1(arg1), _arg2(arg2), _arg3(arg3) {}    
        virtual ReturnType operator()() throw() 
        {
            return (*_action)(_arg1,_arg2,_arg3);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
        Arg_2_Type _arg2;    
        Arg_3_Type _arg3;    
};

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type  >
class PackedTetraryFunc : public PackedFunc
{
    public:
        PackedTetraryFunc( const FunctorPtr& action,
                              const Arg_1_Type& arg1,
                           const Arg_2_Type& arg2, 
                           const Arg_3_Type& arg3,
                           const Arg_4_Type& arg4) :
            _action(action), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4) {}    
        virtual void operator()() throw() 
        {
            (*_action)(_arg1,_arg2,_arg3,_arg4);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
        Arg_2_Type _arg2;    
        Arg_3_Type _arg3;    
        Arg_4_Type _arg4;    
};


template< class FunctorPtr, 
          class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type,
          class ReturnType  >
class PackedTetraryFuncR : public PackedFuncR <ReturnType>
{
    public:
        PackedTetraryFuncR(const FunctorPtr& action,
                              const Arg_1_Type& arg1,
                           const Arg_2_Type& arg2, 
                           const Arg_3_Type& arg3,
                           const Arg_4_Type& arg4) :
            _action(action), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4) {}    
        virtual ReturnType operator()() throw() 
        {
            return (*_action)(_arg1,_arg2,_arg3,_arg4);    
        }
    private:
        FunctorPtr _action;
        Arg_1_Type _arg1;    
        Arg_2_Type _arg2;    
        Arg_3_Type _arg3;    
        Arg_4_Type _arg4;    
};

/////////////////////////////////////////////////////////////////////////
//    Define smart-pointer types 
//
//    General 
//    Asynchronious calls:
//        Useless::PkFunc       Without returned value
//        Useless::PkFuncR<R>   With returned value

class PkFunc
{
public:
    typedef SPointer< PackedFunc > Ptr;

    PkFunc() {}
    PkFunc( PackedFunc *x): _spf(x) {}
    PkFunc( const Ptr &spf ): _spf(spf) {}

    void operator()() { (*_spf)(); }

    Useless::PackedFunc& operator *() { return *_spf; }

    Ptr _spf;
};

template< typename R >
struct PkFuncR
{
public:
    typedef R RetType;
    typedef Useless::SPointer< Useless::PackedFuncR<R> > Ptr;

    PkFuncR() {}
    PkFuncR( PackedFuncR<R> *x): _spf(x) {}
    PkFuncR( const Ptr &spf ): _spf(spf) {}

    RetType operator()() { return (*_spf)(); }

    Useless::PackedFuncR<R>& operator *() { return *_spf; }

    Ptr _spf;
};


////////////////////////////////////////////////////////////////////////////
//    Define PackedFunctor creators
//

/*! \ingroup Functors
 */
/*! @name Packing Functors with default arguments */
/*@{*/
template< class FunctorPtr >  
inline PkFunc PackFunctor( const FunctorPtr& action)
{
    return new PackedVoidFunc<FunctorPtr>(action);    
}

template< class FunctorPtr, class ReturnType >  
inline PkFuncR<ReturnType> PackFunctorR( const FunctorPtr& action, const ReturnType&)
{
    return new PackedVoidFuncR<FunctorPtr, ReturnType>(action);    
}

template< class FunctorPtr, class Arg_1_Type >  
inline PkFunc PackFunctor( const FunctorPtr& action, const Arg_1_Type& arg1)
{
    return new PackedUnaryFunc<FunctorPtr, Arg_1_Type>(action,arg1);    
}

template< class FunctorPtr, class Arg_1_Type, class ReturnType >  
inline PkFuncR<ReturnType> PackFunctorR
( const FunctorPtr& action, const Arg_1_Type& arg1, const ReturnType&)
{
    return new PackedUnaryFuncR
            <FunctorPtr, Arg_1_Type, ReturnType>(action,arg1);    
}

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type >  
inline PkFunc PackFunctor( const FunctorPtr& action, const Arg_1_Type& arg1, const Arg_2_Type& arg2)
{
    return new PackedBinaryFunc<FunctorPtr, Arg_1_Type, Arg_2_Type>(action,arg1,arg2);    
}

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class ReturnType >  
inline PkFuncR<ReturnType> PackFunctorR
( const FunctorPtr& action, const Arg_1_Type& arg1, const Arg_2_Type& arg2, const ReturnType&)
{
    return new PackedBinaryFuncR
            <FunctorPtr, Arg_1_Type, Arg_2_Type, ReturnType>(action,arg1,arg2);    
}

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type >  
inline PkFunc PackFunctor( const FunctorPtr& action,
                         const Arg_1_Type& arg1, const Arg_2_Type& arg2,
                         const Arg_3_Type& arg3)
{
    return new PackedTernaryFunc<FunctorPtr, Arg_1_Type, Arg_2_Type, Arg_3_Type>
            (action,arg1,arg2,arg3);    
}

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class ReturnType >  
inline PkFuncR<ReturnType> PackFunctorR
                           (const FunctorPtr& action,
                         const Arg_1_Type& arg1, const Arg_2_Type& arg2,
                         const Arg_3_Type& arg3, const ReturnType&)
{
    return new PackedTernaryFuncR<FunctorPtr, Arg_1_Type, Arg_2_Type, Arg_3_Type, ReturnType>
            (action,arg1,arg2,arg3);    
}

template< class FunctorPtr, class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type >  
inline PkFunc PackFunctor( const FunctorPtr& action,
                         const Arg_1_Type& arg1, const Arg_2_Type& arg2,
                         const Arg_3_Type& arg3, const Arg_4_Type& arg4)
{
    return new PackedTetraryFunc<FunctorPtr, Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type>
            (action,arg1,arg2,arg3,arg4);    
}

template< class FunctorPtr, 
    class Arg_1_Type, class Arg_2_Type, class Arg_3_Type, class Arg_4_Type,
    class ReturnType >  
inline PkFuncR<ReturnType> PackFunctorR
                        (const FunctorPtr& action,
                         const Arg_1_Type& arg1, const Arg_2_Type& arg2,
                         const Arg_3_Type& arg3, const Arg_4_Type& arg4,
                         const ReturnType&)
{
    return new PackedTetraryFuncR
            <FunctorPtr, Arg_1_Type, Arg_2_Type, Arg_3_Type, Arg_4_Type, ReturnType>
            (action,arg1,arg2,arg3,arg4);    
}
/*@}*/

};
#endif //__INCLUDED_USELESS_PACKEDFUNCTOR_H__
