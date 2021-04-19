#if !defined (KEYBOARD_H)
#define KEYBOARD_H

#include "directinput.h"

namespace Useless {
    
class USELESS_API DIKeyboard
{
private:

    LPDIRECTINPUTDEVICE8    g_pKeyboard; 
    HRESULT                 hr;
    HWND                    _hWnd;
    char                    _buffer[256];

public:
    DIKeyboard (DirectInput &di , HWND hWnd);
    DIKeyboard (const DIKeyboard &kbd);
    virtual ~DIKeyboard ();
    HRESULT Update ();
    bool IsPressed (int key);
};

};//namespace Useless

#endif
