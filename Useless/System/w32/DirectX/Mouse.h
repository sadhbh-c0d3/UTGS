#if !defined (MOUSE_H)
#define MOUSE_H

#include "directinput.h"
#include "Useless/System/w32/Window.h"

namespace Useless {
    
class USELESS_API DIMouse
{
private:
    enum { BUFFER_SIZE = 16 };

    LPDIRECTINPUTDEVICE8    g_pMouse; 
    DIDEVICEOBJECTDATA     *_data;
    HANDLE                  g_hMouseEvent;
    HRESULT                 hr;
    HWND                    _hWnd;
    DIMOUSESTATE2           dims2;
    Window                 *_parent_window;

    long _x,_y,_z,_dx,_dy,_dz;
    bool _button[3];

public:
    DIMouse (DirectInput &di, Window *parent, bool fg=true, bool intercept=true);
    DIMouse (const DIMouse &mouse);
    virtual ~DIMouse();

    void Init(DirectInput &di, bool foreground, bool intercept);
    void Update ();
    void Acquire() { if (g_pMouse) g_pMouse->Acquire(); }
    
    long GetX () const;
    long GetY () const;
    long GetZ () const;
    long GetDX () const;
    long GetDY () const;
    bool Button1 () const;
    bool Button2 () const;
    bool Button3 () const;
};

};// namespace Useless

#endif
