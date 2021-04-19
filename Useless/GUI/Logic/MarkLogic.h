#ifndef __INCLDUED__USELESS_MARK_LOGIC_H__
#define __INCLDUED__USELESS_MARK_LOGIC_H__

#include "Useless/GUI/Logic/LogicWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"

namespace Useless {

class USELESS_API MarkLogic : virtual public ActiveWidget, virtual public EnumWidget, virtual private LogicWidget
{
public:
    MarkLogic();
    
    bool IsDisabled() const;

    virtual  int GetSelected() const;
    virtual void SetSelected(int s);

private:
    void Press();
    void Change();
    void Release();
    void Highlite();
    void Unhighlite();

    bool IsMarked() const;
    void Enable(bool t);

    int _status;
};



};// namespace Useless

#endif//__INCLDUED__USELESS_MARK_LOGIC_H__
