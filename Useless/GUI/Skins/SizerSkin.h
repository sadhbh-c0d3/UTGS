#ifndef __INCLUDED_USELESS_SIZER_SKIN_H__
#define __INCLUDED_USELESS_SIZER_SKIN_H__

#pragma warning(disable:4250) // inherits via dominance
#include "Useless/GUI/Skins/ArraySkin.h"
#include "Useless/GUI/Skins/ImageDicer.h"
#include "Useless/GUI/Skins/FrameSkin.h"


namespace Useless {

/*! \ingroup Skins
 *  \a SizerWidget skin.
 */
class USELESS_API SizerSkin : public FrameSkin, public ArraySkin
{
public:
    SizerSkin(): _orient(HORIZONTAL) {}
     /*!  \a boundry - minimal rectangular area containing all sizer images 
      * 
      *   \a orient - choose: HORIZONTAL or VERTICAL                        
      *
      *   \a p1, \a p2, \a p3 - parallel direction cuts                     
      *
      *   \a o1, \a o2 - orthogonal direction cuts                          
      *
      *   \a no_interrior - if true, no interrior shall be painted          
      *
      *   \a hilite_outer, \a hilite_inner - two parameters used to construct FrameSkinner for hilites 
      */
     //@{
    SizerSkin( const RefImage& image, const Rect& boundry, Orientation orient, int p1, int p2, int p3, int o1, int o2, bool no_interrior=false, const Rect &hilite_outer=Rect(), const Rect &hilite_inner=Rect() );
     //@}
    virtual ~SizerSkin();

    DEFINE_CYCLIC_VISITABLE(ArraySkin);

    enum Images { DIV=1<<8 };

    //Skin overloads
    virtual void Paint( const Painter& painter, const Pos &p = Pos() ) { ArraySkin::Paint(painter,p); }
    virtual  int GetDimension( int dimID ) const;

    virtual SPointer<Skin> CreateSubSkin( int skinID ) const;

    //ArraySkin oveloads
    virtual void SetHDividers( const SortedVector<int> &h_dividers );
    virtual void SetVDividers( const SortedVector<int> &v_dividers );
    virtual bool IsHDividable() const { return _orient==HORIZONTAL; }
    virtual bool IsVDividable() const { return _orient==VERTICAL; }
    virtual void PaintBackdrop( const Painter& painter, const Pos& = Pos() );
    virtual void PaintGrid    ( const Painter& painter, const Pos& = Pos() );

    //Sizer methods
    void ThinkGrid( PointList& points, RectList& rects, const Pos& = Pos() );

private:
    SortedVector<int>    _dividers;
    Orientation          _orient;

    Rect                 _hilite_bound;
    Rect                 _hilite_interior;

    void  SizerDice( const Rect& boundry, Orientation orient, int x1, int x2, int x3, int y1, int y2, bool no_interrior=false);
    Rect  T( const Rect& r);
};

////////////////////////////////////////////////////////////////

inline Rect SizerSkin::T( const Rect& r)
{
    return (_orient==HORIZONTAL)? r : Rect( r.GetY1(), r.GetX1(), r.GetH(), r.GetW() );
}

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,SizerSkin);
REGISTER_COPIER(Useless,ArraySkin,SizerSkin);

#endif//__INCLUDED_USELESS_SIZER_SKIN_H__
