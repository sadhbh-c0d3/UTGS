#ifndef __INCLUDED_USELESS_SIGNAL_EVENT_H__
#define __INCLUDED_USELESS_SIGNAL_EVENT_H__

#include "Useless/Functional/Signal.h"

namespace Useless {

    struct SignalEvent
    {
        virtual ~SignalEvent() {}
        virtual void Raise( Signal *s, const std::string &arg ) = 0;
    };


}; //Useless

#endif//__INCLUDED_USELESS_SIGNAL_EVENT_H__
