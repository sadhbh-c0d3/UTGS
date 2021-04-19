#ifndef __INCLDUED__USELESS_BUTTON_LOGIC_H__
#define __INCLDUED__USELESS_BUTTON_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"

namespace Useless {

class USELESS_API ButtonLogic : virtual public ActiveWidget, virtual private LogicWidget
{
public:
    ButtonLogic();
    
    bool IsDisabled() const;

private:
    void SetHighliten();
    void SetPressed  ();
    void SetNormal   ();
    void SetDisabled ();
    void Enable( bool t);
};



};// namespace Useless

#endif//__INCLDUED__USELESS_BUTTON_LOGIC_H__
