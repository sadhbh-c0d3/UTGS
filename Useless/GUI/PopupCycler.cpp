#include "UselessPch.h"

#include "PopupCycler.h"

namespace Useless {

PopupCycler::PopupCycler(): _lock(0) {}

PopupCycler::~PopupCycler()
{
}

void PopupCycler::Activate( Popup *popup )
{
    bool t = (_lock==0) ? true : false;
    ++_lock;

    /* Disallow activation of nonmodal popup while modal is active */
    if ( !_opened_popups.empty() && !popup->_modal && 
       ( *_opened_popups.begin() )->IsVisible() && 
       ( *_opened_popups.begin() )->_modal ) 
       { return; }

    Iterator i = std::find( _opened_popups.begin(), _opened_popups.end(), popup );
    if ( i!= _opened_popups.begin() || _opened_popups.empty() || !t)
    {
        if ( i!= _opened_popups.end() )  {  _opened_popups.erase(i); }
        if ( t && !_opened_popups.empty() )   
        { 
            Deactivate( *_opened_popups.begin() );
        }
        _opened_popups.push_front( popup );
        popup->ActivatePopupImp();
    }
    --_lock;
}

void PopupCycler::Deactivate( Popup *popup )
{
    bool t = (_lock==0) ? true : false;
    ++_lock;

    Iterator i = _opened_popups.begin();
    if ( (*i)==popup  )
    {
        popup->DeactivatePopupImp();
        
        if (t) 
        { 
            _opened_popups.erase(i);
            if ( !_opened_popups.empty() )
            {
               Activate( *_opened_popups.begin() );
            }
        }
    }

    --_lock;
}

void PopupCycler::VisibleNotify( Popup *popup )
{
    if ( popup->IsVisible() )
    {
        Activate( popup );
    }
    else
    {
        Deactivate( popup );
    }
}

void PopupCycler::ReactivatePopup()
{
    if ( ! _opened_popups.empty() )
    {
        (*_opened_popups.begin())->Show();
    }
}


};//namespace Useless
