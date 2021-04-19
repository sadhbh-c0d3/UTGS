#ifndef __INCLDUED__USELESS_HILITE_LOGIC_H__
#define __INCLDUED__USELESS_HILITE_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"

namespace Useless {

class USELESS_API HiliteLogic : virtual public ActiveWidget, virtual private LogicWidget
{
public:
    HiliteLogic();
    
    bool IsDisabled() const;

private:
    void Highlite();
    void Unhighlite();
    void Enable(bool t);

};



};// namespace Useless

#endif//__INCLDUED__USELESS_HILITE_LOGIC_H__
