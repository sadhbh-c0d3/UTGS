#ifndef __INCLUDED_USELESS_WIDGET_ENVIRONMENT_H__
#define __INCLUDED_USELESS_WIDGET_ENVIRONMENT_H__

#include "Useless/Graphic/RectList.h"
#include "Useless/Functional/Functor.h"

#include "Useless/GUI/Display/HintMan.h" //- included since SPointer was used 

namespace Useless {

//USES
    class Widget;
    class Workspace;
    class Form;
    class Screen;
    class GarbageCollector;

/*! \ingroup Widgets
 *  WidgetEnvironment is very important module. Widgets are using it comunicate
 *  with their environment.
 *
 *  Class i used to send dirty rectangles outside widgets realm.
 *  The path which goes dirty rects splits then, and one copy of
 *  dirty-rect goes to parent widget, and one goes outsie widget's realm.
 *
 *  If widget decides to delete component, it's better sometimes to use
 *
 *  GetEnvironment()->GetTrashCan().Trash(something);
 *
 */
class USELESS_API WidgetEnvironment
{
public:
                 void  SendDirtyOutside( const Rect &rect, Widget *sender) const;
                  int  GetDirtyDuration()      const   { return  n_DirtyTime; }

     GarbageCollector& GetTrashCan()           const   { assert(p_TrashCan); return *p_TrashCan;  }
            Workspace& GetWorkspace()          const   { assert(p_Workspace);return *p_Workspace; }
                 Form& GetForm()               const   { assert(p_Form);     return *p_Form;      }
     SPointer<HintMan> GetHintMan()            const   { assert(!!sp_HintSource); return sp_HintSource; }
               Screen& GetScreen()             const   { assert(p_Screen); return *p_Screen; }
    
    /*! Method to be used to transfer of dirty region */
    mutable FUnary< const Rect& >::Ptr        p_SendDirtyOutside;
                                Widget       *p_WidgetAllowedToSend;
                                   int        n_DirtyTime;

    /*! A Place where the Trash is to be purged */
    mutable GarbageCollector                 *p_TrashCan;

    /*! Highest level root */
    mutable Workspace                        *p_Workspace;

    /*! Form to which belong this branch of widgets */
    mutable Form                             *p_Form;

    /*! Default Hint Manager */
    mutable SPointer<HintMan>                 sp_HintSource;

    /*! Screen */
    mutable Screen                           *p_Screen;
};

////////////////////////////////////////////////////////////////////////////////////
inline void  WidgetEnvironment::SendDirtyOutside( const Rect &rect, Widget *sender) const
{
    if ( sender==p_WidgetAllowedToSend) (*p_SendDirtyOutside)(rect); 
}

}; //Useless

#endif//__INCLUDED_USELESS_WIDGET_ENVIRONMENT_H__
