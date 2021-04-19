#ifndef __INCLUDED_USELESS_PSEUDO_FUNCTOR_H__
#define __INCLUDED_USELESS_PSEUDO_FUNCTOR_H__

namespace Useless {

//! \ingroup Functors
template< class BaseT >
class PseudoFunctor
{
public:
    virtual ~PseudoFunctor() {}
    virtual void operator()( BaseT& ) = 0;    
};

//! \ingroup Functors
template< class BaseT, class MetPt >
class PseudoFunctor_0 : public PseudoFunctor<BaseT>
{
public:
    PseudoFunctor_0( MetPt mpt ): _mpt(mpt) {}
    virtual void operator()( BaseT& instance)
    {
        (instance.*_mpt)();
    }

private:
    MetPt _mpt;
};

//! \ingroup Functors
template< class BaseT, class MetPt, class Arg1 >
class PseudoFunctor_1 : public PseudoFunctor<BaseT>
{
public:
    PseudoFunctor_1( MetPt mpt, const Arg1& arg1 ): _mpt(mpt), _arg1(arg1) {}
    virtual void operator()( BaseT& instance)
    {
        (instance.*_mpt)(_arg1);
    }

private:
    MetPt _mpt;
    Arg1  _arg1;
};

//! \ingroup Functors
template< class BaseT, class MetPt, class Arg1, class Arg2 >
class PseudoFunctor_2 : public PseudoFunctor<BaseT>
{
public:
    PseudoFunctor_2( MetPt mpt, const Arg1& arg1, const Arg2& arg2 ): _mpt(mpt), _arg1(arg1), _arg2(arg2) {}
    void operator()( BaseT& instance)
    {
        (instance.*_mpt)(_arg1,_arg2);
    }

private:
    MetPt _mpt;
    Arg1 _arg1;
    Arg2 _arg2;
};

//! \ingroup Functors
template< class BaseT, class MetPt, class Arg1, class Arg2, class Arg3 >
class PseudoFunctor_3 : public PseudoFunctor<BaseT>
{
public:
    PseudoFunctor_3( MetPt mpt, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3 ): _mpt(mpt), _arg1(arg1), _arg2(arg2), _arg3(arg3) {}
    void operator()( BaseT& instance)
    {
        (instance.*_mpt)(_arg1,_arg2,_arg3);
    }

private:
    MetPt _mpt;
    Arg1 _arg1;
    Arg2 _arg2;
    Arg3 _arg3;
};

//! \ingroup Functors
template< class BaseT, class MetPt, class Arg1, class Arg2, class Arg3, class Arg4 >
class PseudoFunctor_4 : public PseudoFunctor<BaseT>
{
public:
    PseudoFunctor_4( MetPt mpt, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4): 
        _mpt(mpt), _arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4) {}
    void operator()( BaseT& instance)
    {
        (instance.*_mpt)(_arg1,_arg2,_arg3,_arg4);
    }

private:
    MetPt _mpt;
    Arg1 _arg1;
    Arg2 _arg2;
    Arg3 _arg3;
    Arg4 _arg4;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_PSEUDO_FUNCTOR_H__
