#include "UselessPch.h"

#include "directinput.h"

namespace Useless {

DirectInput::DirectInput (HINSTANCE hInst)
{
    HRESULT hr;

    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (void**)&_lpDI, NULL); 

    if FAILED(hr) MessageBox(NULL,_T("Can't create DirectInput device."),_T("ERROR"),MB_OK);
}

DirectInput::DirectInput(const DirectInput &di): _lpDI(di._lpDI)
{
    _lpDI->AddRef();
}

DirectInput::~DirectInput ()
{
    if ( _lpDI != NULL ) 
    {
        _lpDI->Release();
        _lpDI = NULL;
    }
}

LPDIRECTINPUT8 DirectInput::GetlpDI()
{
    return _lpDI;
}

};//namespace Useless



