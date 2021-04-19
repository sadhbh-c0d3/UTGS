#ifndef __INCLUDED__USELESS__ARRAY_SKIN_H__
#define __INCLUDED__USELESS__ARRAY_SKIN_H__

#include "Useless/GUI/Skins/Skin.h"
#include "Useless/Util/Storage/SortedVector.h"

namespace Useless {

/*! ArraySkin is abstract Skin which is divided into cells.
 *  It may be divided horizontaly and/or verticaly.
 */
class USELESS_API ArraySkin : virtual public Skin
{
public:
    //! Set horizontal dividers (separators between rows)
    virtual void SetHDividers( const SortedVector<int> &h_dividers ) = 0;
    //! Set vertical dividers (separators between columns)
    virtual void SetVDividers( const SortedVector<int> &v_dividers ) = 0;
    //! Tell if skin may separate rows
    virtual bool IsHDividable() const { return false; }
    //! Tell if skin may separate columns
    virtual bool IsVDividable() const { return false; }
    //! Paint backdrop tile
    virtual void PaintBackdrop( const Painter& painter, const Pos& = Pos() ) = 0;
    //! Paint grid built of frame and dividers
    virtual void PaintGrid( const Painter& painter, const Pos& = Pos() ) = 0;

    //! Every ArraySkin is also a Skin
    DEFINE_CYCLIC_VISITABLE(Skin);

    //! Skin::Paint is overloaded here
    virtual void Paint( const Painter& painter, const Pos &p = Pos() )
    {
        PaintBackdrop(painter,p);
        PaintGrid(painter,p);
    }
};

};//namespace Useless
#endif//__INCLUDED__USELESS__ARRAY_SKIN_H__
