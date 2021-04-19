#ifndef __INCLUDED__USELESS_LIST_MENU_H__
#define __INCLUDED__USELESS_LIST_MENU_H__

#include "Useless/GUI/Widgets/DropDown.h"

namespace Useless {

//USES
    class AdvListStyle2;

class USELESS_API ListMenu : public DropDown, public IWList
{
public:
    ListMenu( const CaptionStyle &style, IWList *p_items, bool items_above = false );
    ListMenu( const CaptionStyle &style, const AdvListStyle2 &list_style, bool list_above = false );
    virtual ~ListMenu();

//#   include "ListMenu.inl" - ParmList Ctor - unsupported

    virtual DataPointer GetSelectedData() const;
    virtual void SetMenuHeight  ( int h);

    virtual ListWidget&   Items()   { return _p_items->Items(); }

private:
    IWList *_p_items;
    int     _alignment;
};

};
#endif//__INCLUDED__USELESS_LIST_MENU_H__
