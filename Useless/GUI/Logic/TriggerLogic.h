#ifndef __INCLDUED__USELESS_TRIGGER_LOGIC_H__
#define __INCLDUED__USELESS_TRIGGER_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"

namespace Useless {

class USELESS_API TriggerLogic : virtual public ActiveWidget, virtual public EnumWidget, virtual private LogicWidget
{
public:
    TriggerLogic();
    
    bool IsDisabled() const;

    virtual  int GetSelected() const;
    virtual void SetSelected(int s);

private:
    void Press();
    void Highlite();
    void Unhighlite();
    void Enable(bool t);

    bool IsSelected() const;
};



};// namespace Useless

#endif//__INCLDUED__USELESS_TRIGGER_LOGIC_H__
