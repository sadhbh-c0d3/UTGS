#ifndef __INLCLUDED__USELESS__KEY_BUTTON_LOGIC_H__
#define __INLCLUDED__USELESS__KEY_BUTTON_LOGIC_H__

#include "Useless/System/KeyCodes.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Logic/LogicWidget.h"

namespace Useless {

/*! KeyButtonLogic may be used to expand any Button, Mark, Swtich
 *  with keyboard control feature.
 *  This logic creates OnPress[0], OnRelease[0] and Click[0] events.
 *  Combine it with any other logic, f.e ButtonLogic
 */
class USELESS_API KeyButtonLogic : virtual public ActiveWidget, virtual public LogicWidget
{
public:
    KeyButtonLogic( int key_code=Keys::SPACE );
    ~KeyButtonLogic();
    
    void EnableFocussing( int key_code=Keys::SPACE );
    void DisableFocussing();

private:
    void SetFocussed();
    void SetNormal();

    void LMBPress( const Pos &where );
    void FocLost();
    void KbdPress( int code );
    void KbdHold( int code );
    void KbdRelease( int code );
    
    int _code;
    int _state;
    std::vector<Signal::FuncID> _fids;
};

};//namespace Useless
#endif//__INLCLUDED__USELESS__KEY_BUTTON_LOGIC_H__
