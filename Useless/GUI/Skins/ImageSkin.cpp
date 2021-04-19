#include "UselessPch.h"

#include "Useless/GUI/Skins/ImageSkin.h"

namespace Useless {

ImageSkin::ImageSkin( const RefImage &ref_image )
{
    _sp_image = dynamic_cast<ImageBase*>( ref_image.get().Copy().Release() );
    _size.x = _sp_image->GetWidth();
    _size.y = _sp_image->GetHeight();
}

void ImageSkin::Paint( const Painter& painter, const Pos& p )
{
    _sp_image->SetDestWidth( _size.x);
    _sp_image->SetDestHeight( _size.y);
    painter.ProjectImage( p.x, p.y, *_sp_image);
}

};//namespace Useless
