#ifndef __INCLUDED_GUI_BINDER_H__
#define __INCLUDED_GUI_BINDER_H__

#include "Useless/XML/Resources/SignalMap.h"

namespace Useless {

struct USELESS_RESOURCES_API GUIBinder
{
    struct BindDef
    {
        std::string _eventToRaise;
        std::string _stringToPass;
    };
    typedef std::multimap< std::string, BindDef > BindDefinitions;
    BindDefinitions _defs;

    virtual void BindTo( ActiveWidget *source );
};

};//Useless

#endif//__INCLUDED_GUI_BINDER_H__
