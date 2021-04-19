#ifndef __INCLUDED_USELESS_EQUATION_HOLDER_H__
#define __INCLUDED_USELESS_EQUATION_HOLDER_H__

#include "Useless/Functional/Holder.h"

namespace Useless {

/*! \ingroup Functors
 *  \defgroup EquationHolders Equation Holders
 */

/*! @name Packing Equations */
/*@{*/

/*! \ingroup EquationHolders
 *  Evaluates a*b + c
 */
template< class TNum, class T1, class T2, class T3 >
class PolynomialHolder  
{
    public:
        PolynomialHolder() {}
        PolynomialHolder(const T1& a, const T2& b, const T3& c): _a(a), _b(b), _c(c) {}
        operator TNum() {return ((TNum)_a)*((TNum)_b) + ((TNum)_c); }

    private:
        T1 _a;
        T2 _b;
        T3 _c;
};

/*! \ingroup EquationHolders
 *  PolynomialHolder calculates a*b+c. EqPackPoly function creates instance.
 */
template< class TNum, class T1, class T2, class T3 >
PolynomialHolder<TNum,T1,T2,T3> EqPackPoly(const T1& a, const T2& b, const T3& c)
{
    return PolynomialHolder<TNum,T1,T2,T3>( a, b, c);
}

/*! \ingroup EquationHolders
 *  Evaluates -a
 */
template< class TNum, class T1 >
class SignHolder                                     // calculates -a
{
    public:
        SignHolder() {}
        SignHolder(const T1& a): _a(a) {}
        operator TNum() { return -((TNum)_a); }

    private:
        T1 _a;
};

/*! \ingroup EquationHolders
 *  SignHolder calculates -a. EqPackSign function creates instance.
 */
template< class TNum, class T1 >
SignHolder<TNum,T1> EqPackSign(const T1& a)
{
    return SignHolder<TNum,T1>(a);
}

/*! \ingroup EquationHolders
 *  Evaluates 1-a. If TNum=bool (1-a) is the same as (!a).
 */
template< class TNum, class T1 >
class RestHolder                                    // calculates 1-a
{
    public:
        RestHolder() {}
        RestHolder(const T1& a): _a(a) {}
        operator TNum() { return 1.0-((TNum)_a); }

    private:
        T1 _a;
};

/*! \ingroup EquationHolders
 *  RestHolder calculates 1-a. EqPackRest function creates instance.
 */
template< class TNum, class T1 >
RestHolder<TNum,T1> EqPackRest(const T1& a)
{
    return RestHolder<TNum,T1>(a);
}

/*! \ingroup EquationHolders
 *  Evaluates ~a. \warning Dont even try TNum=bool !!!
 */
template< class TNum, class T1 >
class NegationHolder                               // calculates ~a
{
    public:
        NegationHolder() {}
        NegationHolder(const T1& a): _a(a) {}
        operator TNum() { return ~((TNum)_a); }

    private:
        T1 _a;
};

/*! \ingroup EquationHolders
 *  NegationHolder calculates ~a. EqPackNeg function creates instance.
 */
template< class TNum, class T1 >
NegationHolder<TNum,T1> EqPackNeg(const T1& a)
{
    return NegationHolder<TNum,T1>(a);
}

/*! \ingroup EquationHolders
 *  Evaluates 1/a.
 */
template< class TNum, class T1 >
class ReciprocalHolder                             // calculates 1/a
{
    public:
        ReciprocalHolder() {}
        ReciprocalHolder(const T1& a): _a(a) {}
        operator TNum() { return 1.0/((TNum)_a); }

    private:
        T1 _a;
};

/*! \ingroup EquationHolders
 *  ReciprocalHolder calculates 1/a. EqPackReci function creates instance.
 */
template< class TNum, class T1 >
ReciprocalHolder<TNum,T1> EqPackReci(const T1& a)
{
    return ReciprocalHolder<TNum,T1>(a);
}

/*@}*/

};//namespace Useless

#endif __INCLUDED_USELESS_EQUATION_HOLDER_H__
