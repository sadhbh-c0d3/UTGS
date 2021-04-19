#ifndef __INCLUDED_WIDGET_VISITOR___
#define __INCLUDED_WIDGET_VISITOR___

/* Debug GUI event logging macros
 *
 * In order to use GUI logging, in ErrorConfig.h at least __ERROR_LEVEL_4__ must be defined.
 *
 * Logged events depending on choosen level:
 *
 * HIGH: changes of dirty areas
 * MEDIUM: pointer movement
 * LOW: widget creation, destruction, insertion, retrieval, button press and release
 *
 * Note:
 *  Any change in this file, causes only several widgets to recompile.
 */

//! Choose one here, or in ErrorConfig.h

//#define __GUI_DEBUG_LEVEL_HIGH__
//#define __GUI_DEBUG_LEVEL_MEDIUM__
//#define __GUI_DEBUG_LEVEL_LOW__
//#define __GUI_DEBUG_LEVEL_NONE__


#if !defined(NDEBUG) & !defined(__GUI_DEBUG_LEVEL_NONE__)
#   define __GUI_DEBUG_LEVEL_NONE__
#endif

#if !defined(__GUI_DEBUG_LEVEL_NONE__)
#   include "Useless/Error/AppError.h"
#   include "Useless/GUI/Widgets/Widget.h"
#   include "Useless/GUI/WidgetEnvironment.h"
#   ifdef WIN32
#       include <typeinfo.h>
#   else
#       include <typeinfo>
#   endif

namespace Useless {

template<class T> const char* DBG_Name(T& ref)
{
    return typeid(ref).name();
}

class DebugWidgetEventLogger
{
public:
    DebugWidgetEventLogger(Widget const* w) 
    {
        if (w)
        {
            d_dur      = w->_p_environment ? w->_p_environment->GetDirtyDuration() : -1;
            input_egr  = w->_input_eager;
            input_itc  = w->_input_intercept;
            parent     = w->_parent;
            id         = w->_this_widget_id;
            parent_id  = (parent)? parent->_this_widget_id : 0;
            name  = DBG_Name(*w);
        }
        else
        {
            d_dur      = 0;
            d_tim      = 0;
            input_egr  = 0;
            input_itc  = 0;
            parent     = 0;
            id         = 0;
            parent_id  = 0;
            name  = "none";
        }
    }

    short            d_dur;
    short            d_tim;
    int              input_egr;
    int              input_itc;
    Widget const*    parent;
    int              id;
    int              parent_id;
    const char*      name;
};

#define DBG_LogWidgetEvent(E) SMALL_LOG(E,"Widgets");

inline double GetTime()
{
    return Useless::Timer()-Useless::Application::GetStartTime();
}

#define DBG_Info_2(M) \
    { \
        DebugWidgetEventLogger self(this); \
        DebugWidgetEventLogger him(sender); \
        DBG_LogWidgetEvent( Error("[%02.3f] ID:%02i P:%02i EC:%i IC:%i \t %s \t%s \t\tsender: %i \t%s", \
        GetTime(), self.id, self.parent_id, self.input_egr, self.input_itc, M, self.name, him.id, him.name ) );\
    }


#define DBG_Info_1(M) \
    { \
        DebugWidgetEventLogger self(this); \
        DBG_LogWidgetEvent( Error("[%02.3f] ID:%02i P:%02i EC:%i IC:%i \t %s \t%s", \
        GetTime(), self.id, self.parent_id, self.input_egr, self.input_itc, M, self.name ) );\
    }

#define DBG_Info_0(M) \
    { \
        DebugWidgetEventLogger self(this); \
        DBG_LogWidgetEvent( Error("[%02.3f] ID:%02i      \t\t\t%s", GetTime(), self.id, M ) );\
    }

#define DBG_Info_MEM(M) \
    { \
        DebugWidgetEventLogger self(this); \
        DBG_LogWidgetEvent( Error("[%02.3f] ID:%02i  ADDR:%0x\t\t\t%s", GetTime(), self.id, this, M ) );\
    }


#define DBG_Info_WGT(M, widget) \
    { \
        DebugWidgetEventLogger self(this); \
        DebugWidgetEventLogger him(widget); \
        DBG_LogWidgetEvent( Error("[%02.3f] ID:%02i P:%02i EC:%i IC:%i \t %s \t%s \t\tsender: %i \t%s", \
        GetTime(), self.id, self.parent_id, self.input_egr, self.input_itc, M, self.name, him.id, him.name ) );\
    }

#define DBG_Info_WGT1(widget) \
    { \
        DebugWidgetEventLogger self(widget); \
        DBG_LogWidgetEvent( Error("[%02.3f] ID:%02i P:%02i EC:%i IC:%i \t %s \t%s", \
        GetTime(), self.id, self.parent_id, self.input_egr, self.input_itc, M, self.name ) );\
    }


#ifdef  __GUI_DEBUG_LEVEL_HIGH__
#define __GUI_DEBUG_LEVEL_MEDIUM__
#define __GUI_DEBUG_LEVEL_LOW__
#define DBG_Dirty    DBG_Info_2("SetDirty");
#define DBG_DirtyP   DBG_Info_2("SetDirtyParent")
#define DBG_DirtyCh  DBG_Info_2("SetDirtyChilds")
#define DBG_Undirty  DBG_Info_1("UnDirty")
#define DBG_AcceptC  DBG_Info_0("AcceptCursor")
#endif __GUI_DEBUG_LEVEL_HIGH__

#ifdef  __GUI_DEBUG_LEVEL_MEDIUM__
#define __GUI_DEBUG_LEVEL_LOW__
#define DBG_IncEager DBG_Info_1("Input.Eager.Increase")
#define DBG_DecEager DBG_Info_1("Input.Eager.Decrease")
#define DBG_IntCur   DBG_Info_2("Input.Intercept")
#define DBG_RelCur   DBG_Info_2("Input.Release")
#endif  __GUI_DEBUG_LEVEL_MEDIUM__

#ifdef __GUI_DEBUG_LEVEL_LOW__
#define DBG_AcceptB  DBG_Info_0("AcceptButton")
#define DBG_Attach   DBG_Info_1("Attach")
#define DBG_Detach   DBG_Info_1("Detach")
#define DBG_Create   DBG_Info_MEM("Create")
#define DBG_Destroy  DBG_Info_MEM("Destroy")
#define DBG_Insert   DBG_Info_WGT("Insert",widget)
#define DBG_Remove   DBG_Info_WGT("Remove",widget)
#define DBG_Retrieve DBG_Info_WGT("Retrieve",widget)
#define DBG_Text(M)  DBG_Info_1(M)
#endif  __GUI_DEBUG_LEVEL_LOW__

#define DBG_Error    DBG_Info_1(" !!! ")

};//namespace Useless


#endif//!__GUI_DEBUG_LEVEL_NONE__

// Define all undefined debug macros as empty ones.

#ifndef DBG_AcceptC
    #define DBG_AcceptC {}
    #endif
#ifndef DBG_AcceptB
    #define DBG_AcceptB {}
    #endif
#ifndef DBG_IncEager 
    #define DBG_IncEager {}
    #endif
#ifndef DBG_DecEager
    #define DBG_DecEager {}
    #endif
#ifndef DBG_IntCur 
    #define DBG_IntCur {}
    #endif
#ifndef DBG_RelCur
    #define DBG_RelCur {}
    #endif
#ifndef DBG_Dirty  
    #define DBG_Dirty  {}
    #endif
#ifndef DBG_DirtyP
    #define DBG_DirtyP {}
    #endif
#ifndef DBG_DirtyCh 
    #define DBG_DirtyCh {}
    #endif
#ifndef DBG_Undirty
    #define DBG_Undirty {}
    #endif
#ifndef DBG_Attach 
    #define DBG_Attach {}
    #endif
#ifndef DBG_Detach
    #define DBG_Detach {}
    #endif
#ifndef DBG_Create
    #define DBG_Create {}
    #endif
#ifndef DBG_Destroy
    #define DBG_Destroy {}
    #endif
#ifndef DBG_Insert
    #define DBG_Insert {}
    #endif
#ifndef DBG_Remove
    #define DBG_Remove {}
    #endif
#ifndef DBG_Retrieve
    #define DBG_Retrieve {}
    #endif
#ifndef DBG_Error  
    #define DBG_Error  {}
    #endif
#ifndef DBG_Text
    #define DBG_Text(M) {}
    #endif
#ifndef DBG_Info_0
    #define DBG_Info_0(M) {}
    #endif

#endif//__INCLUDED_WIDGET_VISITOR___
