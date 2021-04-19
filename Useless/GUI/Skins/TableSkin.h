#ifndef __INCLUDED_USELESS_TABLE_SKIN_H__
#define __INCLUDED_USELESS_TABLE_SKIN_H__

#pragma warning(disable:4250) // inherits via dominance
#include "Useless/GUI/Skins/ArraySkin.h"
#include "Useless/GUI/Skins/ImageDicer.h"
#include "Useless/GUI/Skins/FrameSkin.h"

namespace Useless {

/*! \ingroup Skins
 *  \a TableWidget skin.
 */
class USELESS_API TableSkin : public FrameSkin, public ArraySkin
{
public:
    TableSkin() {}
    TableSkin( const RefImage &image, const Rect& boundry, int x1, int x2, int x3, int y1, int y2, int y3, bool no_interrior=false, const Rect& hilite_outer=Rect(), const Rect& hilite_inner=Rect() );
    virtual ~TableSkin();

    DEFINE_CYCLIC_VISITABLE(ArraySkin);

    enum Images { HDIV=1<<8, VDIV=2<<8, CROSSING=3<<8 };

    //Skin overloads
    virtual void Paint( const Painter& painter, const Pos &p = Pos() ) { ArraySkin::Paint(painter,p); }
    virtual  int GetDimension( int dimID ) const;

    virtual SPointer<Skin> CreateSubSkin( int skinID ) const;

    //ArraySkin oveloads
    virtual void SetHDividers( const SortedVector<int> &h_dividers );
    virtual void SetVDividers( const SortedVector<int> &v_dividers );
    virtual bool IsHDividable() const { return true; }
    virtual bool IsVDividable() const { return true; }
    virtual void PaintBackdrop( const Painter& painter, const Pos& = Pos() );
    virtual void PaintGrid    ( const Painter& painter, const Pos& = Pos() );

    //Table methods
    void  ThinkGrid( PointList& points, RectList& rects, const Pos& = Pos() );

private:
    SortedVector<int>    _x_dividers;
    SortedVector<int>    _y_dividers;

    Rect                 _hilite_bound;
    Rect                 _hilite_interior;

    void  TableDice( const Rect& boundry, int x1, int x2, int x3, int y1, int y2, int y3, bool no_interrior=false);
};


};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,TableSkin);
REGISTER_COPIER(Useless,ArraySkin,TableSkin);

#endif//__INCLUDED_USELESS_TABLE_SKIN_H__
