#ifndef __INCLUDED_USELESS_POPUP_CYCLER_H__
#define __INCLUDED_USELESS_POPUP_CYCLER_H__

#include "Useless/GUI/Widgets/Popup.h"
#include <list>

namespace Useless {

class USELESS_API PopupCycler
{
public:
    PopupCycler();
    ~PopupCycler();

    void Activate( Popup* );
    void Deactivate( Popup* );
    void VisibleNotify( Popup * );
    void ReactivatePopup();

private:
    typedef std::list< Popup * >::iterator Iterator;
    std::list< Popup * > _opened_popups;
    int _lock;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_POPUP_CYCLER_H__
