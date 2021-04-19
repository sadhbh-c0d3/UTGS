#if !defined (DIRECTINPUT_H)
#define DIRECTINPUT_H


#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#pragma comment (lib,"dinput8.lib")


namespace Useless {

class USELESS_API DirectInput
{
private:

    LPDIRECTINPUT8          _lpDI; 

public:
    DirectInput (HINSTANCE hInst);
    DirectInput(const DirectInput&);
    ~DirectInput ();
    LPDIRECTINPUT8 GetlpDI();

private:
    DirectInput operator=(const DirectInput&);

};


};//namespace Useless

#endif
