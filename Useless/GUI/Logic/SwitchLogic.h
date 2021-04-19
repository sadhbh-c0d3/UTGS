#ifndef __INCLDUED__USELESS_SWITCH_LOGIC_H__
#define __INCLDUED__USELESS_SWITCH_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"

namespace Useless {

class USELESS_API SwitchLogic : virtual public ActiveWidget, virtual public EnumWidget, virtual private LogicWidget
{
public:
    SwitchLogic();
    
    bool IsDisabled() const;

    virtual  int GetSelected() const;
    virtual void SetSelected(int s);

private:
    void Press();
    void Highlite();
    void Unhighlite();
    void Enable(bool t);

    bool IsSwitched() const;
};



};// namespace Useless

#endif//__INCLDUED__USELESS_SWITCH_LOGIC_H__
