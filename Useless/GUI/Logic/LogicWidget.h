#ifndef __INCLUDED_USELESS_LOGIC_WIDGET_H__
#define __INCLUDED_USELESS_LOGIC_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Misc/DisplayState.h"

namespace Useless {

class USELESS_API LogicWidget : virtual public Widget
{
public:
    virtual  int  GetState () const = 0;
    virtual bool  SetState (int state) = 0;
    virtual bool  HasState (int state) const = 0;


    //helper method to get DisplayState from state int
    DisplayState GetDP() const
    {
        return GetState();
    }
    //helper method to set only highlight and selection
    bool SetStateA( int state )
    {
        return SetState( GetDP().CpSetA(state) );
    }
    //helper method to get only highlight and selection info
    DisplayState GetStateA() const
    {
        return GetDP().CpGetA();
    }
};
    
};//namespace Useless

#endif//__INCLUDED_USELESS_LOGIC_WIDGET_H__
