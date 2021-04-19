#ifndef __INCLUDED_USELESS_FRAME_SKIN_H__
#define __INCLUDED_USELESS_FRAME_SKIN_H__

/*
 *    $Id: FrameSkin.h,v 1.10 2003/01/20 22:00:31 koolas Exp $
 *
 *    NAME
 *        FrameSkin
 *
 *    PURPOSE
 *        Creates image-set for FrameWidget
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Skins/Skin.h"
#include "Useless/GUI/Skins/ImageDicer.h"

namespace Useless {

/*! \ingroup Skins
 *  \a FrameWidget skin.
 */
class USELESS_API FrameSkin : virtual public Skin, virtual protected ImageDicer
{
public:
    FrameSkin(): _width(0), _height(0) {}
    FrameSkin( const RefImage& ref_image, const Rect &inner, bool no_interior=false );
    virtual ~FrameSkin() {}

    //obsolete
    FrameSkin( const RefImage& ref_image, const Rect &outer, const Rect &inner, bool no_interior=false );

    DEFINE_CYCLIC_VISITABLE(Skin);

    enum Images { HEAD=1, BODY=2, TAIL=3, TOP =0<<4, MIDDLE=1<<4, BOTTOM=2<<4 };

    ImageDicer::SetColorKey;
    ImageDicer::SetNoColorKey;

    virtual  void  Paint( const Painter& painter, const Pos& = Pos() );
    virtual   int  GetWidth ()             const { return _width;  }
    virtual   int  GetHeight()             const { return _height; }
    virtual   Pos  GetDirection()          const { return Pos(1,1); }
    virtual  void  SetSize  ( const Dim2i& d)    { _width=d.x; _height=d.y; }

    virtual   float       GetFrameRate() const { return GetSource().GetFrameRate();}
    virtual   bool        IsSolid()      const { return GetSource().IsSolid();}
    virtual  bool         IsColorKeyed() const { return GetSource().HasColorKey(); }
    virtual  NormalPixel GetColorKey()   const { return GetSource().GetColorKey(); }

    virtual int GetDimension( int dimID ) const;


    void PaintBackdrop( const Painter& painter, const Pos& = Pos() );
    void PaintFrame   ( const Painter& painter, const Pos& = Pos() );
    void ThinkFrame   ( PointList& points, RectList& rects, const Pos& = Pos() );
    void ThinkBackdrop( PointList& points, RectList& rects, const Pos& = Pos() );

           IGraphics&  GetSource()       { return ImageDicer::GetImage(SOURCE); }
     const IGraphics&  GetSource() const { return ImageDicer::GetImage(SOURCE); }


protected:
    int _width;
    int _height;

    PointList            _point_cache;
    RectList             _rect_cache;

    Rect                 _rLU;
    Rect                 _rRU;
    Rect                 _rRD;
    Rect                 _rLD;

private:
    void FrameDice( Rect outer, Rect inner=Rect(), bool no_interior=false);
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,FrameSkin);

#endif//__INCLUDED_USELESS_FRAME_SKIN_H__
