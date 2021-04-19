#ifndef __INCLUDED__USELESS_HINT_MAN_H__
#define __INCLUDED__USELESS_HINT_MAN_H__

namespace Useless {

//USES
    class Widget;

/*! \ingroup gDisplay
 *  HintMan is widget hint manager, used by all ActiveWidgets to show/hide hints.
 */
class USELESS_API HintMan
{
public:
    virtual ~HintMan() {}
    virtual void ShowHint( Widget *hint_owner ) = 0;
    virtual void HideHint( Widget *hint_owner ) = 0;
    virtual bool HasHint ( Widget *hint_owner ) = 0;
	
};

};//namespace Useless
#endif//__INCLUDED__USELESS_HINT_MAN_H__
