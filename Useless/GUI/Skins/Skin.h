#ifndef __INCLUDED_USELESS_SKIN_H__
#define __INCLUDED_USELESS_SKIN_H__

#include "Useless/Util/Visitor.h"
#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Util/CopyingFactory.h"

namespace Useless {
/*! \defgroup Skins Skins
 *  \ingroup gGUI
 */

/*! \ingroup Skins
 *  Interface for \a Widget skins.
 */
class USELESS_API Skin : public BaseVisitable<>
{
public:
             Skin();
    virtual ~Skin() {}

    DEFINE_VISITABLE();
    
    //! Each skin must know how to paint itself
    virtual void
    Paint       ( const Painter& painter, const Pos& = Pos() ) = 0;

    virtual int //! width  is property of any skin instance
    GetWidth    ()  const = 0;
    
    virtual int //! height is property of any skin instance
    GetHeight   ()  const = 0;
    
    virtual void//! may be unsafe for some skins (assertions)
    SetSize     ( const Dim2i &d) = 0;
    
    virtual Pos //! tell in which direction skin may be resized
    GetDirection()  const = 0;
    
    bool        //! tries to set desired size, but may not success
    SafeResize  ( const Dim2i &d );

    //! Mask is used to limit skin dimentionality
    //  ( from 2D to 1D or 0D )
    Pos
    GetDirMask  ()  const       { return _direction_mask; }
    
    void
    SetDirMask  ( const Pos &m) { _direction_mask = Pos(!!m.x,!!m.y); }

    Dim2i       //! Get skin dimentions
    GetSize     () const; 

    
    //! Length can be computed only if skin is 1D
    // ( direction * mask = Pos(x,0) or Pos(0,y) )
    int
    GetLength   () const;
    
    void
    SetLength   ( int l );

    //! Some skins may be computed faster if
    //  clip_rect is set to limit rendering region
    void
    SetClipRect ( const Rect &clip_rect)  { _clip_rect=clip_rect; }
    
    Rect
    GetClipRect ()  const   { return _clip_rect; }

    
    virtual bool //! Is skin solid filled (no transparent areas)
    IsSolid     () const { return IsColorKeyed();}
    
    virtual bool
    IsColorKeyed() const { return false; }
    
    virtual NormalPixel
    GetColorKey () const { return 0L; }

    virtual float
    GetFrameRate() const { return 0.0;}

    //! Tell if skin is NullSkin
    virtual bool
    operator !  () const { return false; }

    //! Some skins may have specific dimentions i.e. cell padding
    virtual
    int GetDimension    ( int dimID )   const { return 0; }

    
    virtual SPointer<Skin>
    CreateSubSkin       ( int skinID )  const { return SPointer<Skin>(); }

private:
    Pos  _direction_mask;
    Rect _clip_rect;

#ifndef NDEBUG
    public:
    int         _skin_id;
    static int  _skin_counter;
#endif
};

typedef enum __Orientation
{
    HORIZONTAL,
    VERTICAL,

} Orientation;

typedef enum __Dimension_ID
{
    LEFT_MARGIN,
    TOP_MARGIN,
    RIGHT_MARGIN,
    BOTTOM_MARGIN,
    VERTI_DIV,
    HORIZ_DIV

} Dimension_ID;

typedef enum __Alignment
{
    H_LEFT=0,
    H_CENTER=1,
    H_RIGHT=2,
    V_TOP=0<<8,
    V_CENTER=1<<8,
    V_BOTTOM=2<<8

} Alignment;

typedef enum __SubSkins
{
    HIGHLIGHT=0
       
} SubSkin_ID;


#ifdef NDEBUG
inline Skin::Skin(): _direction_mask(1,1) {}
#else
inline Skin::Skin(): _direction_mask(1,1), _skin_id(++_skin_counter) {}
#endif


};//namespace Useless

#endif//__INCLUDED_USELESS_SKIN_H__
