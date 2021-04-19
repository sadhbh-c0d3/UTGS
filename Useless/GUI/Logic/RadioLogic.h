#ifndef __INCLDUED__USELESS_RADIO_LOGIC_H__
#define __INCLDUED__USELESS_RADIO_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"

namespace Useless {

class USELESS_API RadioLogic : virtual public ActiveWidget, virtual public EnumWidget, virtual private LogicWidget
{
public:
    RadioLogic();
    
    bool IsDisabled() const;

    virtual  int GetSelected() const;
    virtual void SetSelected(int s);

    virtual bool IsClickPossibile( const Pos &p, int btn_no ) const;

private:
    void Press();
    void Highlite();
    void Unhighlite();
    void Enable(bool t);

    bool IsRadio() const;
};



};// namespace Useless

#endif//__INCLDUED__USELESS_RADIO_LOGIC_H__
