#ifndef __INCLUDED_DDRAW_WINDOW_H__
#define __INCLUDED_DDRAW_WINDOW_H__

/*
 * $Id: DDrawWindow.h,v 1.6 2002/09/26 18:32:55 koolas Exp $
 *
 *  NAME
 *      DDrawWindow 
 *
 *  PURPOSE
 *      Direct Draw specialization of Window class
 *
 *  AUTHOR(S)
 *      Sadhbh Code (https://github.com/sadhbh-c0d3)
 *
 *
*/ 

#include "Useless/System/w32/Window.h"

// Add DirectInput functionality
#include "Useless/System/w32/DirectX/DirectInput.h"
#include "Useless/System/w32/DirectX/Keyboard.h"
#include "Useless/System/w32/DirectX/Mouse.h"
#include "Useless/Graphic/Device/DDraw/DDSurface.h"

namespace Useless {
/*! \ingroup gApplication
 *  Window specialized in DirectDraw.
 *  It also gives DirectInput features.
 */
class USELESS_API DDrawWindow : virtual public Window
{
public:
    DDrawWindow(const char *title="IAV Game");
    DDrawWindow(const char *title, int width, int height, int bpp) throw();
    DDrawWindow(const char* title, Window* parent, int width, int height, int x, int y, bool titlebar=true) throw();
    virtual ~DDrawWindow();

//protected:
          DDCreator  &GetDisplay()        { return _display; }
    const DDCreator  &GetDisplay()  const { return _display; }

    const DIKeyboard &GetKeyboard() const { return *_keyboard; }
    const DIMouse    &GetMouse()    const { return *_mouse; }
             void     InitDInput( bool foreground, bool intercept);
             void     ReleaseDInput();

private:
    DDCreator   _display;
    DirectInput *_input;
    DIKeyboard  *_keyboard;
    DIMouse     *_mouse;

    Signal::FuncID _mouse_update;
    Signal::FuncID _keyboard_update;

    void Update();
    void Tie2Signals();
    friend class DIMouse;
    friend class DIKeyboard;
};

};// namespace Useless
#endif __INCLUDED_DDRAW_WINDOW_H__
