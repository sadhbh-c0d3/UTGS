#ifndef __INCLDUED__USELESS_TAB_LOGIC_H__
#define __INCLDUED__USELESS_TAB_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"

namespace Useless {

class USELESS_API TabLogic : virtual public ActiveWidget, virtual public EnumWidget, virtual private LogicWidget
{
public:
    TabLogic();
    
    bool IsDisabled() const;

    virtual  int GetSelected() const;
    virtual void SetSelected(int s);

    virtual bool IsClickPossibile( const Pos &p, int btn_no ) const;

private:
    void Press();
    void Unpress();
    void Select();
    void Highlite();
    void Unhighlite();

    bool IsSelected() const;
    void Enable(bool t);
};



};// namespace Useless

#endif//__INCLDUED__USELESS_TAB_LOGIC_H__
