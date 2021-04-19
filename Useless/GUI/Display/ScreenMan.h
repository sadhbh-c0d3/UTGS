#ifndef __INCLUDED_USELESS_SCREEN_MAN_H__
#define __INCLUDED_USELESS_SCREEN_MAN_H__

/*
 *    $Id: ScreenMan.h,v 1.19 2003/01/20 22:00:31 koolas Exp $
 *
 *    NAME
 *        ScreenMan
 *
 *    PURPOSE
 *        Manages and syncronizes Screen writes
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/Graphic/ClipList.h"
#include "Useless/GUI/Display/DisplayMan.h"
#include "Useless/GUI/Display/MouseCursor.h"
#include "Useless/Util/Storage/StupidVector.h"
#include "Useless/GUI/Display/ImageCursor.h"
#include "Useless/Util/CtorWrapper.h"

namespace Useless {

// REQUIRE:
    class GUIMaster;
    class Screen;

////////////////////////////////////////////////////////////
//  CLASS: ScreenMan
//
/*! \ingroup gDisplay
 *  Manages Screen buffer flipping and cursor.
 */
class USELESS_API ScreenMan : public DisplayMan
{
public:
    ScreenMan(Screen &screen, GUIMaster &gui_master);
    virtual ~ScreenMan();

    void Attach();
    void Detach();
    bool IsAttached() { return !_event_ties.Empty(); }

                 Screen& GetScreen   () { return *_screen; }
    virtual   OGraphics& GetPlane    ( const Rect &rect = Rect() );
    virtual ImageBuffer  GetImageRect( const Rect &area);
    virtual    RectList  GetClipRects() const;
    virtual    RectList  GetDirtyRegion()  const  { return GetRects(); }
    virtual        void  SetClipRects(const RectList& rect_list);
    virtual        bool  Intersect   (const Rect &rect);

                   void  SheduleCompleteRepaint();
                   void  AcceptDirtyRect       (const Rect &r);
                    int  GetDirtyCounter       () const { return 3; }

    typedef CtorWrapper< MouseCursor, ImageCursor, IGraphics > RefCursor;

                   void  UpdateCursor          (Pos p, Pos dp);
                   void  DrawCursor            ();
                   void  SetCursor             (const MouseCursor &mc);
     const MouseCursor&  GetCursor() const { return *_mouse_cursor; }
                   void  SetDefaultCursor      ();
                   void  CreateDefaultCursor   ( const RefCursor &image );
                   void  RemoveCursor();
                   void  SetCursorVisible(bool visible) { _cursor_visible=visible;}
                   bool  IsCursorVisible() const        { return _cursor_visible;}

                   void  UpdateWheel           (int z, int dz);
                   void  SetWheelDivider       (int divider);
                    Pos  WhereIsPointer        () const { return _cursor; }

                   void  Advance(int,int);
                   void  AcceptButtons(unsigned long status, unsigned long mask);

private:
    Screen    *_screen;
    GUIMaster *_gui_master;
    ClipList   _dirty_rects[3];
    int        _current_frame;
    bool       _cursor_visible;

          ClipList& GetRects(int frame);
    const ClipList& GetRects(int frame) const;
          RectList  GetRects() const;

    Pos _cursor;
    Pos _cursor_doubler;
    SPointer<MouseCursor> _mouse_cursor;
    SPointer<MouseCursor> _default_mouse_cursor;
    bool                  _mouse_buttons_status;
    
    int _wheel;
    int _wheel_divider;

    StupidVector< Signal::FuncID > _event_ties;
};

/*
---------------------------------------------------------------*/
inline ClipList& ScreenMan::GetRects(int frame)
{
    return _dirty_rects[ frame %3 ];
}

inline const ClipList& ScreenMan::GetRects(int frame) const
{
    return _dirty_rects[ frame %3 ];
}

inline RectList ScreenMan::GetRects() const
{
    RectList rects;
    _dirty_rects[ _current_frame %3 ].copy(rects);
    return rects;
}

};// namespace Useless


#endif //__INCLUDED_USELESS_SCREEN_MAN_H__
