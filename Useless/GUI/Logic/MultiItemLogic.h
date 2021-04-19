#ifndef __INCLUDED__USELESS_MULTI_ITEM_LOGIC_H__
#define __INCLUDED__USELESS_MULTI_ITEM_LOGIC_H__

#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

/*! Multi Item Logical States Manager
---------------------------------------------------------*/
class USELESS_API MultiItemLogic : virtual public Widget
{
public:
    virtual  int GetState( int item_no )         const = 0;
    virtual bool SetState( int item_no, int state_no ) = 0;
    virtual bool HasState( int item_no, int state_no ) = 0;

    virtual  int ItemAtPosition( const Pos &pos) const = 0;
    virtual  int GetNumItems()                   const = 0;
            bool ItemExists ( int item_no )      const;

    virtual Rect GetItemBound ( int item_no  )   const = 0;

    virtual void OnItemRemove( int item_no ) {};
    virtual void OnItemsClear() {};
};


inline bool MultiItemLogic::ItemExists( int item_no ) const
{
    return item_no>=0 && item_no<GetNumItems();
}

};//namespace Useless

#endif//__INCLUDED__USELESS_MULTI_ITEM_LOGIC_H__
