#include "UselessPch.h"

#include "Useless/System/w32/DirectX/DirectInput.h"
#include "Useless/System/w32/DirectX/Mouse.h"

namespace Useless {

DIMouse::DIMouse (DirectInput &di, Window *parent, bool fg, bool intercept):
        _parent_window(parent), g_pMouse(NULL), _data(0)
{
    _hWnd = _parent_window->GetHandle();
    Init(di, fg, intercept);
}

DIMouse::DIMouse (const DIMouse &mouse): g_pMouse(mouse.g_pMouse), _parent_window(mouse._parent_window), _data(0)
{
    _hWnd = _parent_window->GetHandle();
    if (g_pMouse) g_pMouse->AddRef();
    ZeroMemory( &_button, 3*sizeof(int) );
}

DIMouse::~DIMouse()
{
    if (g_pMouse)
    {
        g_pMouse->Unacquire();
        g_pMouse->Release();
        g_pMouse=NULL;
    }
    delete[] _data;
}

void DIMouse::Init(DirectInput &di, bool foreground, bool intercept)
{
    if (g_pMouse)
    {
        g_pMouse->Unacquire();
        g_pMouse->Release();
        g_pMouse=NULL;
    }
    delete[] _data;

    hr = di.GetlpDI()->CreateDevice(GUID_SysMouse, &g_pMouse, NULL);
    if (FAILED(hr)) {
        
        throw Useless::Error("DIMouse::DIMouse(): Can't create device.","ERROR",MB_OK);

    }

    hr = g_pMouse->SetDataFormat(&c_dfDIMouse);
 
    if (FAILED(hr)) {
        
        throw Useless::Error("DIMouse::DIMouse(): Can't set data format","ERROR",MB_OK);
    }

    unsigned int flags=0;
    if (intercept)  flags |= DISCL_EXCLUSIVE;
    if (foreground) flags |= DISCL_FOREGROUND;
    hr = g_pMouse->SetCooperativeLevel(_hWnd, flags);
 
    if (FAILED(hr)) {
        
        throw Useless::Error("DIMouse::DIMouse(): Can't set CooperativeLevel","ERROR",MB_OK);

    }

    DIPROPDWORD dip;
    dip.diph.dwSize = sizeof(DIPROPDWORD);
    dip.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dip.diph.dwObj = 0;
    dip.diph.dwHow = DIPH_DEVICE;
    dip.dwData = BUFFER_SIZE;
    if ( g_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dip.diph ) != DI_OK )
        throw Useless::Error("DIMouse::DIMouse(): Buffer could not be set");
    _data = new DIDEVICEOBJECTDATA[ BUFFER_SIZE ];

    Rect wcr = _parent_window->GetClientRect();
    _x= wcr.GetW()/2.0;
    _y= wcr.GetH()/2.0;
    _z= 0.0;
    _button[0]=0;
    _button[1]=0;
    _button[2]=0;
    g_pMouse->Acquire();
}

void DIMouse::Update ()
{
    if( NULL == g_pMouse ) { return; }
  
    unsigned long num_enqueued;

    num_enqueued = BUFFER_SIZE;
    if ( g_pMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), _data, &num_enqueued, 0 ) != DI_OK )
    {
        while( g_pMouse->Acquire() == DIERR_INPUTLOST ) {}
    }

    while(num_enqueued--)
    {
        switch ( (*_data).dwOfs )
        {
        case DIMOFS_BUTTON0:
            {
                _button[0] = (((*_data).dwData & 0x80)&&1);
                _parent_window->OnMouseBtn.Send_2( _button[0]<<0, 1<<0 );
            }
            break;

        case DIMOFS_BUTTON1:
            {
                _button[1] = (((*_data).dwData & 0x80)&&1);
                _parent_window->OnMouseBtn.Send_2( _button[1]<<1, 1<<1 );
            }
            break;

        case DIMOFS_BUTTON2:
            {
                _button[2] = (((*_data).dwData & 0x80)&&1);
                _parent_window->OnMouseBtn.Send_2( _button[2]<<2, 1<<2 );
            }
            break;
        }
    }

    
    ZeroMemory( &dims2, sizeof(dims2) );
    hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims2 );

    if( FAILED(hr) ) 
    {
    
        hr = g_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pMouse->Acquire();

        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
                
        return; 
    }

    _dx= dims2.lX;
    _dy= dims2.lY;
    _dz= dims2.lZ;

    _x+= dims2.lX;
    _y+= dims2.lY;
    _z+= dims2.lZ;

    Rect wcr = _parent_window->GetClientRect();
    if (_x < 0 ) _x = wcr.GetX1();
    if (_y < 0 ) _y = wcr.GetY1();
    if (_x > wcr.GetW() ) _x = wcr.GetW();
    if (_y > wcr.GetH() ) _y = wcr.GetH();
   
    unsigned int changed_buttons_mask = 0;
    unsigned int buttons_status = 0;
    for (int i=0; i!=3l; ++i)
    {
        if (dims2.rgbButtons[i]&0x80) 
            { 
                buttons_status = (1<<i); 
            }
        if ( (dims2.rgbButtons[i]&0x80) && !_button[i] || !(dims2.rgbButtons[i]&0x80) && _button[i] )
        { 
            _button[i] = (dims2.rgbButtons[i]&0x80); 
            changed_buttons_mask |= (1<<i);
        }
    }
    if (changed_buttons_mask) 
    {
        _parent_window->OnMouseBtn.Send_2( buttons_status, changed_buttons_mask );
    }

    if ( _dx || _dy )
    {
        _parent_window->OnMouseMove.Send_2( Pos(_x, _y), Pos(_dx, _dy) ); 
    }

    if ( _dz )
    {
        _parent_window->OnMouseWheel.Send_2( _z, _dz ); 
    }
}

long DIMouse :: GetX() const
{
    return _x;
}

long DIMouse :: GetY() const
{
    return _y;
}

long DIMouse :: GetZ() const
{
    return _z;
}

bool DIMouse :: Button1() const
{
    return dims2.rgbButtons[0] & 0x80;
}

bool DIMouse :: Button2() const
{
    return dims2.rgbButtons[1] & 0x80;
}

bool DIMouse :: Button3() const
{
    return dims2.rgbButtons[3] & 0x80;
}

long DIMouse :: GetDX() const
{
    return _dx;
}

long DIMouse :: GetDY() const
{
    return _dy;
}

};//namespace Useless
