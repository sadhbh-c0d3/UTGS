#ifndef __INCLUDED_USELESS_BAR_SKINNER_H__
#define __INCLUDED_USELESS_BAR_SKINNER_H__


#include "Useless/GUI/Skins/Skin.h"
#include "Useless/GUI/Skins/ImageDicer.h"

namespace Useless {

/*! \ingroup Skins
 *  \a BarWidget skin.
 */
class USELESS_API BarSkinner : public Skin, private ImageDicer
{
public:
    BarSkinner() {}
    BarSkinner( const RefImage& image, Orientation orient, int x1, int x2, int x3 );
    virtual ~BarSkinner() {}

    //obsolete
    BarSkinner( const RefImage& image, const Rect& boundry, Orientation orient, int x1, int x2, int x3 );

    DEFINE_CYCLIC_VISITABLE(Skin);

    enum Images { HEAD=1, BODY=2, HANDLE=3, TAIL=4, NUM_IMAGES };

    virtual  void  Paint    ( const Painter& painter, const Pos& = Pos() );
    virtual   int  GetWidth ()             const { return (_orient==HORIZONTAL)? _size : GetImage(HEAD).GetWidth();  }
    virtual   int  GetHeight()             const { return (_orient==VERTICAL)?   _size : GetImage(HEAD).GetHeight(); }
    virtual   Pos  GetDirection()          const { return (_orient==HORIZONTAL)? Pos(1,0) : Pos(0,1) ; }
    virtual  void  SetSize  ( const Dim2i& d)    { _size = X(d.x, d.y); }

    virtual   float       GetFrameRate() const { return GetSource().GetFrameRate();}
    virtual   bool        IsSolid()      const { return GetSource().IsSolid();}
    virtual   bool        IsColorKeyed() const { return GetSource().HasColorKey(); }
    virtual   NormalPixel GetColorKey()  const { return GetSource().GetColorKey(); }

             void  Think    ( PointList& points, RectList& rects, const Pos& = Pos() );
           IGraphics& GetSource()       { return ImageDicer::GetImage(SOURCE); }
     const IGraphics& GetSource() const { return ImageDicer::GetImage(SOURCE); }
             void GetMargins( int &frame_left, int &frame_top, int &frame_right, int &frame_bottom );

    virtual int GetDimension( int dimID ) const;

    ImageDicer::SetColorKey;
    ImageDicer::SetNoColorKey;

private:
    int         _size;
    Orientation _orient;

    void  BarDice(const Rect& boundry, Orientation orient, int x1, int x2, int x3 );

     Pos  P( int x );
     int  X( int x, int y );
    Rect  R( int x, int s );
};

////////////////////////////////////////////////////////////////

inline Pos  BarSkinner::P( int x )
{ 
    return (_orient==HORIZONTAL)? Pos(x,0) : Pos(0,x) ; 
}

inline int  BarSkinner::X( int x, int y ) 
{ 
    return (_orient==HORIZONTAL)? x : y ; 
}

inline Rect BarSkinner::R( int x, int s ) 
{ 
    s=(s<0)? 0:s;
    return (_orient==HORIZONTAL)? Rect(x,0,s,GetImage(HEAD).GetHeight()): Rect(0,x,GetImage(HEAD).GetWidth(),s); 
}

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,BarSkinner);

#endif//__INCLUDED_USELESS_BAR_SKINNER_H__
