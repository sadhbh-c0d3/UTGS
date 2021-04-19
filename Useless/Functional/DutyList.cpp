#include "UselessPch.h"

#include "Useless/Functional/DutyList.h"

namespace Useless{

/* Only for testing
class Z 
{ 
public: 
    void F(){}
    void W(int) {}
    void V(int, int) {}
    void H(int, int, int) {}
    void G(int, int, int, int) {}
};

void foo()
{   
    PseudoFunctor_0<Z, void (Z::*)() > psf(&Z::F);
    PseudoFunctor_1<Z, void (Z::*)(int), float> psw(&Z::W, 5.5f);
    PseudoFunctor_2<Z, void (Z::*)(int,int), float, float > psv(&Z::V, 5.5f, 7.3f);

    DutyList<Z> dutl;
    dutl.AddDuty(&Z::F);
    dutl.AddDuty(&Z::W, 7.2f);
    dutl.AddDuty(&Z::V, 7.2f, 6.3f);
    dutl.AddDuty(&Z::H, 7.2f, 6.3f, 5);
    dutl.AddDuty(&Z::G, 7.2f, 6.3f, 2, 4);
    Z z;
    dutl.Execute( z );
}
*/

};//namespace Useless
