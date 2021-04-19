#ifndef __INCLUDED__USELESS__TABLE_LISTED_ITEMS__H__
#define __INCLUDED__USELESS__TABLE_LISTED_ITEMS__H__

#include "Useless/GUI/Logic/MultiItemLogic.h"
#include "Useless/GUI/Widgets/DataTableWidget.h"
#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/Util/Storage/StupidVector.h"

namespace Useless {

class USELESS_API TableListedItems : virtual public DataTableWidget, virtual public MultiItemLogic
{
public:
             TableListedItems( const ArraySkin &table_skin, const SkinMan &state_skins );
             TableListedItems( const TableStyle &style, const SkinMan &state_skins );
    virtual ~TableListedItems();

    virtual  int  GetState( int item_no )         const;
    virtual bool  SetState( int item_no, int state_no );
    virtual bool  HasState( int item_no, int state_no );

    virtual  int  ItemAtPosition( const Pos &pos) const;
    virtual  int  GetNumItems()                   const { return TableWidget::GetNumRows(); }
    virtual Rect  GetItemBound( int item_no  )    const;

    virtual void  PaintClients ( const WidgetPainter &painter );
    virtual DisplayState GetPaintState( int col, int row) const { return GetState(row); }

    virtual void  UpdateTable();

    enum { GEN_FOCUSSED=1, GEN_HIGHLIGHT=2 };
    void SetupSkin( int flags );

    DEFINE_CYCLIC_VISITABLE(DataTableWidget);

protected:
    void  RepaintImpl( const WidgetPainter &painter ) {}
    void  ResizeImpl( int w, int h) {}
    void  UpdateTableImpl();

private:
    SkinSelector _skins;

    struct State
    {
        State(): i(0) {}
        State(int i): i(i) {}
        operator int() const { return i; }
        int i;
    };
    StupidVector<State>  _states;

};

};//namespace Useless
#endif//__INCLUDED__USELESS__TABLE_LISTED_ITEMS__H__
