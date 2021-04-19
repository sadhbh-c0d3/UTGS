#ifndef __INCLUDED_USELESS_VARIABLE_LINK_H__
#define __INCLUDED_USELESS_VARIABLE_LINK_H__

#include "Useless/Functional/Functor.h"
#include "Useless/TemplateConfig.h"

namespace Useless {

/*! \ingroup ObjSync
 *  Link of two variables in compound with translators.
 *  \a Sync method looks for any changes and synchronize values
 *  of linked variables using translator \a Functor.
 */
template< class T1, class T2 >
class VariableLink
{
public:
    VariableLink(T1& var1, T2& var2, USELESS_TYPENAME FUnaryR<T2,T1>::Ptr T2toT1, USELESS_TYPENAME FUnaryR<T1,T2>::Ptr T1toT2): 
                 _var1(var1), _var2(var2), _T2toT1(T2toT1), _T1toT2(T1toT2) {}
    virtual ~VariableLink() {}

    void Sync()
    {
        if ( !_sp_val1() || _var1!=(*_sp_val1) )
        {
            _sp_val1 = new T1(_var1);
            _var2 = (*_T1toT2)(_var1);
            Sync();
        }
        else if ( !_sp_val2() || _var2!=(*_sp_val2) )
        {
            _sp_val2 = new T1(_var2);
            _var1 = (*_T2toT1)(_var2);
            Sync();
        }
    }


private:
    USELESS_TYPENAME FUnaryR<T2,T1>::Ptr _T2toT1;
    USELESS_TYPENAME FUnaryR<T1,T2>::Ptr _T1toT2;
    T1                  &_var1;
    T2                  &_var2;
    SPointer<T1>         _sp_val1;
    SPointer<T2>         _sp_val2;
};


};//namespace Useless

#endif//__INCLUDED_USELESS_VARIABLE_LINK_H__
