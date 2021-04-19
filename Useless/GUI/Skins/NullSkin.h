#ifndef __INCLUDED_USELESS_NULLSKIN_H__
#define __INCLUDED_USELESS_NULLSKIN_H__

#include "Useless/GUI/Skins/ArraySkin.h"

namespace Useless {
/*! \ingroup Skins
 *  A Skin which responds for requests, but doesn't Paint anything.
 */
class USELESS_API NullSkin : public ArraySkin
{
public:
    NullSkin( int w=0, int h=0 ): _width(w), _height(h) {}
    NullSkin( const Pos &dir_mask ) { SetDirMask(dir_mask); }

    virtual ~NullSkin() {}
    
    virtual  void  Paint    ( const Painter& painter, const Pos& = Pos() ) {}

    virtual   int  GetWidth ()             const { return _width;  }
    virtual   int  GetHeight()             const { return _height; }
    virtual   Pos  GetDirection()          const { return Pos(1,1); }
    virtual  void  SetSize  ( const Dim2i& d)    { _width=d.x; _height=d.y; }

    virtual  bool  IsColorKeyed()          const { return false; }

    virtual  bool operator !() const { return true; }

    virtual void SetHDividers( const SortedVector<int> &h_dividers ) {}
    virtual void SetVDividers( const SortedVector<int> &v_dividers ) {}
    virtual bool IsHDividable() const { return false; }
    virtual bool IsVDividable() const { return false; }
    virtual void PaintBackdrop( const Painter& painter, const Pos& = Pos() ) {}
    virtual void PaintGrid( const Painter& painter, const Pos& = Pos() ) {}

private:
    int _width;
    int _height;
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,NullSkin);
REGISTER_COPIER(Useless,ArraySkin,NullSkin);

#endif//__INCLUDED_USELESS_NULLSKIN_H__
