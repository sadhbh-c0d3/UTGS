#ifndef __INCLUDED_USELESS_MOUSE_CURSOR_H__
#define __INCLUDED_USELESS_MOUSE_CURSOR_H__

#include "Useless/Graphic/Planes/OGraphics.h"

namespace Useless {

/*! \ingroup gDisplay
 *  Base for setting mouse cursor shape and behavior.
 */
class USELESS_API MouseCursor
{
public:
    MouseCursor(): _speed(1.0f), _acceleration(0.02f), _limitFlags(0) {}
    virtual ~MouseCursor() {}

    virtual void Paint(const Pos& p, OGraphics& output) = 0;
    virtual Rect GetRect() const = 0;
    virtual MouseCursor* Copy() const = 0;

    int          _limitFlags;
    Rect         _paint_limit;
    Rect         _move_limit;
    float        _speed;
    float        _acceleration;
};


class Widget;

/*! Holds MouseCursor copy and when asked,
 *  it translates limits from parent Widget coords.
 */
template<class MC> struct MCurHolder
{
    MCurHolder(const MC &m, Useless::Widget *parent): mc(m), _mc(m), _parent(parent) {}

    operator MC& ()
    {
        mc._paint_limit= _parent->GetAbsoluteRect(_mc._paint_limit);
        mc._move_limit = _parent->GetAbsoluteRect(_mc._move_limit);
        return mc; 
    }

    MC               mc;
    MC              _mc;
    Useless::Widget   *_parent;
};

template<class MC> MCurHolder<MC> PackMouseCursor(const MC &m, Useless::Widget *p) 
{
    return MCurHolder<MC>(m,p); 
}


};//namespace Useless


#endif//__INCLUDED_USELESS_MOUSE_CURSOR_H__

