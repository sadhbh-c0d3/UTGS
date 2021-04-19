#ifndef __INCLUDED_USELESS_TALBE_LIST_WIDGET_H__
#define __INCLUDED_USELESS_TALBE_LIST_WIDGET_H__

#include "Useless/GUI/Widgets/TableListedItems.h"
#include "Useless/GUI/Logic/ListLogic.h"

namespace Useless {

//USES
    class TableListStyle;


/*! \ingroup Widgets
 *  TableListWidget is combined from TableListedItems and ListLogic.
 *  Compatibility with version 1.0 preserved.
 *  v. 2.0
 */
class USELESS_API TableListWidget : virtual public TableListedItems, virtual public ListLogic
{
public:
    TableListWidget( const ArraySkin &skin, const SkinMan &state_skins );
    TableListWidget( const TableListStyle &style);
    virtual ~TableListWidget();


    /* Obsolete ctor
    ---------------------------------------------------------------------------*/
    TableListWidget( const ArraySkin &skin );
    TableListWidget( const TableStyle &style);

    virtual void Setup() { TableListedItems::Setup(); ListLogic::Setup(); }
};



/*! Extended table style which holds additional skins for item states
 */
class USELESS_API TableListStyle : public TableStyle
{
public:
    TableListStyle( const ArraySkin &grid_skin,
                    const SkinMan   &state_skins,
                    int l=0, int t=0, int r=0, int b=0, int hd=0, int vd=0 )

                    :TableStyle(grid_skin,l,t,r,b,hd,vd), state_skins(state_skins) {}


    const SkinMan &state_skins;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_TALBE_LIST_WIDGET_H__
