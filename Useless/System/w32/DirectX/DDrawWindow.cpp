#include "UselessPch.h"

#include "Useless/System/w32/DirectX/DDrawWindow.h"

#include "Useless/ErrorConfig.h"
#define   DDRAWWINDOW_ERROR(e)   APP_CRITICAL(e, "DDrawWindow")
#define   DDRAWWINDOW_WARNING(e) APP_WARNING (e, "DDrawWindow")

namespace Useless {

#pragma warning(disable:4355)

DDrawWindow::DDrawWindow(const char *title) : Window(title), _input(0), _keyboard(0), _mouse(0)
{
    Tie2Signals();
    AllowClear(false);
}


DDrawWindow::DDrawWindow(const char *t, Window *p, int w, int h, int x, int y, bool titlebar) throw() 
    : Window(t,p,
        w+GetSystemMetrics(SM_CXSIZEFRAME)*2,
        h+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYCAPTION),
        x,y), _input(0), _keyboard(0), _mouse(0)
{
    if( FAILED( _display.CreateWindowed( Window::GetHandle(), w, h ) ) )
    {
        DDRAWWINDOW_ERROR(Useless::Error("Window %ix%ix couldn't be used for a display",w,h) );
    }
    Tie2Signals();
    AllowClear(false);
}

DDrawWindow::DDrawWindow(const char *t, int w, int h, int bpp ) throw() : Window(t,0,w,h,0,0),
        _input(0), _keyboard(0), _mouse(0)
{
    if( FAILED( _display.CreateFullScreen( Window::GetHandle(), w, h, bpp ) ) )
    {
        DDRAWWINDOW_ERROR(Useless::Error("Mode %ix%ix%i is not supported",w,h,bpp) );
    }
    Tie2Signals();
    AllowClear(false);
}

void DDrawWindow::InitDInput( bool fg, bool intercept)
{
    if (!_input)
    {
        try { _input    = new DirectInput( Application::GetInstance() ); } catch(...)  {_input = 0; }
    }
    if (!_keyboard)
    {
        try 
        { 
            _keyboard = new DIKeyboard( *_input, Window::GetHandle() ); 
            _keyboard_update = Useless::Tie2Signal( Application::OnUpdate, _keyboard, &DIKeyboard::Update );
        }
        catch(...) { _keyboard=0; }
    }
    if (!_mouse)    
    {
        try
        { 
            _mouse = new DIMouse( *_input, this, fg, intercept ); 
            _mouse_update    = Useless::Tie2Signal( Application::OnUpdate,    _mouse, &DIMouse::Update );
        } 
        catch(...) { _mouse=0; }
    }
}

void DDrawWindow::ReleaseDInput()
{
    _mouse_update.Untie();
    _keyboard_update.Untie();
    delete _keyboard; _keyboard=0;
    try { delete _mouse; } catch(...){} _mouse=0; //Workaround unknown error: damage after normal block
    delete _input;    _input=0;
}

DDrawWindow::~DDrawWindow()
{
    ReleaseDInput();
}

void DDrawWindow::Update()
{
    if ( FAILED( _display.UpdateBounds() ) )
    {
        DDRAWWINDOW_WARNING(Useless::Error("DDrawWindow::UpdateBounds(): Error"));
    }
}

void DDrawWindow::Tie2Signals()
{
    Useless::Tie2Signal( OnMove, this, &DDrawWindow::Update );
    Useless::Tie2Signal( OnResize, this, &DDrawWindow::Update );
}

};// namespace Useless
