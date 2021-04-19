#ifndef __INCLUDED_USELESS_OFFSCREEN_MAN_H__
#define __INCLUDED_USELESS_OFFSCREEN_MAN_H__

/*
 *    $Id: OffscreenMan.h,v 1.9 2002/09/13 16:22:11 koolas Exp $
 *
 *    NAME
 *        OffscreenMan
 *
 *    PURPOSE
 *        Manages Offscreen writes
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/GUI/Display/DisplayMan.h"
#include "Useless/Graphic/Planes/ImageBuffer.h"
#include "Useless/Functional/Functor.h"

namespace Useless {

// REQUIRE:
    class Screen;

////////////////////////////////////////////////////////////
//  CLASS: OffscreenMan
//
/*! \ingroup gDisplay
 *  Manages Image Buffer
 */
class USELESS_API OffscreenMan : public DisplayMan
{
public:
    OffscreenMan(int w, int h, Screen &screen);
    OffscreenMan(const ImageBuffer &image_plane);
    virtual ~OffscreenMan();
    
            ImageBuffer& GetImage    ()                            { return _image_plane; }
    virtual   OGraphics& GetPlane    ( const Rect &rect = Rect() ) { return _image_plane; }
    virtual ImageBuffer  GetImageRect( const Rect &area);
    virtual    RectList  GetClipRects() const;
    virtual    RectList  GetDirtyRegion() const                    { return _dirty_rects; }
    virtual        void  SetClipRects(const RectList& rect_list);
    virtual        bool  Intersect   (const Rect &rect);

            void AcceptDirtyRect(const Rect &r);
            void Repaint();

    FVoid::Ptr   pRepaint;

private:
    int _w,_h;
    ImageBuffer _image_plane;
    RectList _dirty_rects;
};

};// namespace Useless


#endif //__INCLUDED_USELESS_OFFSCREEN_MAN_H__
