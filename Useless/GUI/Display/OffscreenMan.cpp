#include "UselessPch.h"

#include "Useless/Graphic/Device/Screen.h"
#include "Useless/GUI/Display/OffscreenMan.h"

namespace Useless {

OffscreenMan::OffscreenMan(int w, int h, Screen &screen): _image_plane(w,h), _w(w), _h(h)
{
    _image_plane.Create(screen);
    _dirty_rects.push_back( Rect(0, 0, _w, _h) );
}

OffscreenMan::OffscreenMan(const ImageBuffer &image_plane): 
    _image_plane(image_plane), _w(image_plane.GetWidth()), _h(image_plane.GetHeight())
{
    _dirty_rects.push_back( Rect(0, 0, _w, _h) );
}

OffscreenMan::~OffscreenMan()
{
}

RectList OffscreenMan::GetClipRects() const
{
    return _image_plane.GetClipper();
}

void OffscreenMan::SetClipRects( const RectList& rect_list )
{
    if ( !rect_list.empty() )
        _image_plane.SetClipper( rect_list );
}

ImageBuffer OffscreenMan::GetImageRect( const Rect &area)
{
    return ImageBuffer( _image_plane, area );
}

bool OffscreenMan::Intersect( const Rect &crop )
{
    if ( !crop ) { return false; }

    RectList region = _image_plane.GetClipper( crop );

    if ( region.empty() )  { return false; }

    _image_plane.SetClipper( region );
    return true;
}


void OffscreenMan::AcceptDirtyRect(const Rect &r)
{
    _dirty_rects.push_back(r);
}

void OffscreenMan::Repaint()
{
    if( !_dirty_rects.empty() )
    {
        _image_plane.SetClipper( _dirty_rects );
        if ( pRepaint() ) (*pRepaint)();
        _dirty_rects.clear();
    }
}

};// namespace Useless
