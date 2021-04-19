#ifndef __INCLUDED_USELESS_WIDGET_TABLE_WIDGET_H__
#define __INCLUDED_USELESS_WIDGET_TABLE_WIDGET_H__

#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Widgets/TableWidget.h"


namespace Useless {

class USELESS_API WidgetTableWidget : virtual private HubWidget, virtual public TableWidget
{
public:
    WidgetTableWidget( const ArraySkin &skin );
    WidgetTableWidget( const TableStyle &style);
    virtual ~WidgetTableWidget();

    virtual  void  Repaint( const WidgetPainter &painter );
    virtual  void  Resize( int w, int h);
    virtual  Rect  GetClientRect( const Widget *sender=NULL ) const;

       bool  Insert ( int column, int row, Widget *client,
                      int = H_CENTER|V_CENTER );//!< Alignment is defined in <Skin.h> or <Rect.h>

     Widget* Retrieve( int column, int row);
       bool  Remove  ( int column, int row);

       bool  Retrieve( Widget *client);
       bool  Remove  ( Widget *client);

       bool  Settled( int column, int row) const;
     Widget* Settler( int column, int row) const;

	virtual void RemoveColumns( int column, int num_columns );
    virtual void UpdateTable();
    
    HubWidget::Accept;
    DEFINE_CYCLIC_VISITABLE(Widget);

protected:
    void  RepaintImpl( const WidgetPainter &painter );
    void  ResizeImpl( int w, int h) {}
    void  UpdateTableImpl();

private:
    typedef struct __Cell
    {
        Widget          *_settler;
        Rect::Alignment  _alignment;

    } Cell;

    typedef AssocVector< Pos, Cell>     SettlerMap;
    typedef AssocVector< Widget*, Pos>  PlaceMap;
    
    SettlerMap  _settlers;
    PlaceMap    _placement;
};


};//namespace Useless

#endif//__INCLUDED_USELESS_WIDGET_TABLE_WIDGET_H__
