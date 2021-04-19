#include "UselessPch.h"

#include "Useless/GUI/Display/ImageCursor.h"

namespace Useless {

ImageCursor::ImageCursor( const IGraphics &image, const Pos& hotspot )
    : _sp_image(image.Copy()), _hotspot(hotspot)
{}

void ImageCursor::Paint(const Pos &p, OGraphics &output)
{
    (*_sp_image).BlitTo( p.x-_hotspot.x, p.y-_hotspot.y, output );
}

};//namespace Useless
