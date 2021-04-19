#ifndef __INCLUDED_USELESS_GUI_MATER_H__
#define __INCLUDED_USELESS_GUI_MATER_H__

/*
 *    $Id: GUIMaster.h,v 1.12 2003/01/20 22:00:30 koolas Exp $
 *
 *    NAME
 *        GUIMaster        
 *
 *    PURPOSE
 *        Root of GUI
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/WidgetEnvironment.h"

namespace Useless {

// REQUIRE:
    class Screen;
    class ScreenMan;

///////////////////////////////////////
//  CLASS: GUIMaster
//
/*! \defgroup gGUI GUI */

/*! \ingroup gGUI 
 *  Main GUI manager.
 *  GUI manager creates base WidgetEnvironment.
 *
 *  It creates ScreenManager, Workspace and GarbageCollector.
 *  It may also create default MouseCursor.
 *
 *  Workspace is entry point for \a Widget's.
 */
class USELESS_API GUIMaster
{
public:
    GUIMaster(Screen &screen, bool dont_attach = false );
    GUIMaster(Screen &screen, const IGraphics &mouse_cursor_shape, bool dont_attach = false );
    ~GUIMaster();

    Workspace&  GetWorkspace() { return *_workspace;  }
    ScreenMan&  GetScreenMan() { return *_screen_man; }
    const WidgetEnvironment& GetEnvironment() { return _environment; }
    
    void        SetHintMan(SPointer<HintMan> hints);

    void AttachToScreen();
    void DetachFromScreen();

private:
    ScreenMan         *_screen_man;
    Workspace         *_workspace;
    GarbageCollector   _trash_can;
    WidgetEnvironment  _environment;

    void Init(Screen &screen, bool dont_join );
};

};// namespace Useless

#endif //__INCLUDED_USELESS_GUI_MATER_H__
