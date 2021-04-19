#include "UselessPch.h"

#include "keyboard.h"
#include "DirectInput.h"

namespace Useless {

DIKeyboard :: DIKeyboard (DirectInput &di , HWND hWnd): _hWnd(hWnd)
{
    hr = di.GetlpDI()->CreateDevice(GUID_SysKeyboard, &g_pKeyboard, NULL); 
    if FAILED(hr) {
        MessageBox(NULL,_T("Can't create device."),_T("ERROR"),MB_OK);
    }

    hr = g_pKeyboard->SetDataFormat(&c_dfDIKeyboard); 
 
    if FAILED(hr) {
        MessageBox(NULL,_T("Can't set data format."),_T("ERROR"),MB_OK);
    }

    hr = g_pKeyboard->SetCooperativeLevel(_hWnd, 
                   DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 

    if FAILED(hr) {
        MessageBox(NULL,_T("Can't set cooperative level."),_T("ERROR"),MB_OK);
    }

    hr = g_pKeyboard->Acquire();
/*
    if FAILED(hr) {
        MessageBox(NULL,"Can't acquire.","ERROR",MB_OK);
    }
*/
} 

DIKeyboard::DIKeyboard (const DIKeyboard &kbd): g_pKeyboard(kbd.g_pKeyboard), _hWnd(kbd._hWnd)
{
    g_pKeyboard->AddRef();
}

DIKeyboard :: ~DIKeyboard ()
{
    if (g_pKeyboard!=NULL) 
    {
        g_pKeyboard->Unacquire();

        g_pKeyboard->Release();
        g_pKeyboard = NULL;
    }
}

HRESULT DIKeyboard :: Update ()
{
    if( NULL == g_pKeyboard ) 
    {
        return E_FAIL;
    }
    
    ZeroMemory( &_buffer, sizeof(_buffer) );
    hr = g_pKeyboard->GetDeviceState(sizeof(_buffer),(LPVOID)&_buffer); 

    if( FAILED(hr) ) 
    {
    
        hr = g_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pKeyboard->Acquire();

        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
                
        return hr; 
    }

    return S_OK;

}

bool DIKeyboard :: IsPressed (int key)
{
    return (_buffer[key] & 0x80) ? 1 : 0;
}

}; //Useless
