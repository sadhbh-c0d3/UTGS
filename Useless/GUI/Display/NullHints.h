#ifndef __INCLUDED__USELESS_NULL_HINTS_H__
#define __INCLUDED__USELESS_NULL_HINTS_H__

#include "Useless/GUI/Display/HintMan.h"

namespace Useless {

class USELESS_API NullHints : public HintMan
{
public:
    virtual ~NullHints() {}
    virtual void ShowHint( Widget *hint_owner ) {}
    virtual void HideHint( Widget *hint_owner ) {}
    virtual bool HasHint ( Widget *hint_owner ) { return false; }
};


};//namespace Useless
#endif//__INCLUDED__USELESS_NULL_HINTS_H__
