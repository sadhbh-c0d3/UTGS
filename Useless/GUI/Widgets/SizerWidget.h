#ifndef __INCLUDED_USELESS_SIZER_WIDGET_H__
#define __INCLUDED_USELESS_SIZER_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Skins/ArraySkin.h"
#include "Useless/GUI/Widgets/detail/Belt.h"
#include "Useless/Util/Storage/cell_division_vector.h"
#include "Useless/Util/Storage/AssocVector.h"

#include "Useless/GUI/Skins/SizerSkin.h"

namespace Useless {

//USES
    class SizerStyle;

/*! \ingroup Widgets
 *  A SizerWidget is a frame divided to rows or columns.
 *  It stores and aligns widgets inside cells.
 */
class USELESS_API SizerWidget : private HubWidget, virtual public Widget, public IWSizer
{
public:
    SizerWidget( const ArraySkin &skin );
    SizerWidget( const SizerStyle &style);
    virtual ~SizerWidget();

    virtual SizerWidget& Sizer() { return *this; }

    virtual void RepaintAction(const WidgetPainter &painter);
    virtual void Resize( int w, int h);
    virtual Rect GetClientRect( const Widget *sender=NULL ) const;

////////////////////////////////////////
//  Sizer Methods
    void AddCell    ( int size, bool fixed=false );
    void ResizeCell ( int cell, int size, bool fixed=false );
    void RemoveCells( int cell, int num_cells );
     int GetNumCells()           const { return _cells.size(); }
     int GetCellSize( int cell ) const { return _cells.get_size(cell); }

    Rect CellRect  ( int field ) const;
    Rect ClientRect( int field ) const;

     int GetCell    ( int p ) const;
     int GetDivider ( int p ) const;
    bool OverDivider( const Pos &pos   ) const;

    Rect GetInside  ( const Rect &area ) const;
    Rect GetCrossing( const Rect &area ) const;

    virtual void SetupCells( const StupidVector<Belt> &cells, bool preserve_size=false );
    StupidVector<Belt> GetCells() const;
    void Sync( const TableWidget *p_table );

    virtual void Setup();
    virtual void UpdateSizer();
    bool IsHorizontal() const { return !!_sp_skin->Skin::GetDirMask().x; }

///////////////////////////////////////
//  WidgetSizer Methods
    bool Insert ( int field, Widget *client, int align = H_CENTER | V_CENTER );
    int  Insert ( Widget *client, int align = H_CENTER | V_CENTER );
    
 Widget* Retrieve( int field );
    bool Remove  ( int field );
    
    bool Retrieve( Widget *client);
    bool Remove  ( Widget *client);

    bool Settled ( int field ) const;
 Widget* Settler ( int field ) const;

    HubWidget::Accept;
    DEFINE_CYCLIC_VISITABLE(Widget);

private:
    SPointer<ArraySkin> _sp_skin;
    int _w,_h;

    cell_division_vector _cells;
    bool                 _need_update;

    typedef struct __Cell
    {
        Widget          *_settler;
        Rect::Alignment  _alignment;

    } Cell;

    AssocVector< int, Cell>      _settlers;
    AssocVector< Widget*, int>   _placement;

    int                 _frame_left;
    int                 _frame_top;
    int                 _frame_right;
    int                 _frame_bottom;
    int                 _horiz_divider;
    int                 _verti_divider;

    friend class TableWidget;

//helpers
    int& Parallel() { return IsHorizontal() ? _w:_h; }
    int& Ortho   () { return IsHorizontal() ? _h:_w; }
    int& Parallel( int &x, int &y) { return IsHorizontal() ? x:y ;}
    int& Ortho   ( int &x, int &y) { return IsHorizontal() ? y:x ;}

    int  Parallel( int x, int y) const { return IsHorizontal() ? x:y ;}
    int  Ortho   ( int x, int y) const { return IsHorizontal() ? y:x ;}
    int  Parallel() const { return IsHorizontal() ? _w:_h; }
    int  Ortho   () const { return IsHorizontal() ? _h:_w; }

};
//----------------------------------------------------------------------------------

/* Sizer style information
*/
class SizerStyle
{
public:
    SizerStyle( const ArraySkin &s, int l=0, int t=0, int r=0, int b=0, int d=0): 
    skin(s), frame_left(l), frame_top(t), frame_right(r), frame_bottom(b), divider(d) {}

    const ArraySkin &skin;
    int              frame_left;
    int              frame_top;
    int              frame_right;
    int              frame_bottom;
    int              divider;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_SIZER_WIDGET_H__
