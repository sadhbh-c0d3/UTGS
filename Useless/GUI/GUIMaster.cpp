#include "UselessPch.h"

#include "Useless/GUI/GUIMaster.h"
#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/Graphic/Device/Screen.h"
#include "Useless/GUI/Display/ScreenMan.h"
#include "Useless/GUI/Display/NullHints.h"

#include "Useless/ErrorConfig.h"
#define   BIGLOG(e) HUGE_LOG(e, "GUIMaster")
#define   GUILOG(e) SMALL_LOG(e, "GUIMaster")

namespace Useless {

GUIMaster::GUIMaster(Screen &screen, bool dont_attach )
{
    Init(screen,dont_attach);
}
    
GUIMaster::GUIMaster(Screen &screen, const IGraphics &mcimg, bool dont_attach )
{
    Init(screen,dont_attach);
    _screen_man->CreateDefaultCursor(mcimg);
    _screen_man->SetDefaultCursor();
}

GUIMaster::~GUIMaster()
{
    _workspace->SetEnvironment(NULL);
    _trash_can.Purify();
    delete _workspace;
    delete _screen_man;
}

void GUIMaster::Init(Screen &screen, bool dont_attach)
{
    _workspace   = new Workspace(*this);
    _screen_man  = new ScreenMan(screen,*this);

    _environment.p_TrashCan = &_trash_can;
    _environment.n_DirtyTime = _screen_man->GetDirtyCounter();
    _environment.p_Workspace = _workspace;
    _environment.p_Form = _workspace; // @todo
    _environment.p_Screen = &screen;
    _environment.sp_HintSource = new NullHints();

    _workspace->Resize( screen.GetWidth(), screen.GetHeight() );
    _workspace->SetEnvironment( &_environment );

    if (!dont_attach) _screen_man->Attach();
}

void GUIMaster::AttachToScreen()
{
    _screen_man->Attach();
}

void GUIMaster::DetachFromScreen()
{
    _screen_man->Detach();
}

void GUIMaster::SetHintMan( SPointer<HintMan> hints )
{
    _environment.sp_HintSource = hints;
    _workspace->SetEnvironment( &_environment );
}


};// namespace Useless
