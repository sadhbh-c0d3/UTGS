#ifndef __INCLDUED__USELESS_DRAGGER_LOGIC_H__
#define __INCLDUED__USELESS_DRAGGER_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"

namespace Useless {

class USELESS_API DraggerLogic : virtual public ActiveWidget, virtual private LogicWidget
{
public:
    DraggerLogic();
    
    bool IsDisabled() const;

private:
    void SetHighliten();
    void SetPressed  ();
    void SetNormal   ();
    void SetDisabled ();
    void Enable( bool t);
};



};// namespace Useless

#endif//__INCLDUED__USELESS_DRAGGER_LOGIC_H__
