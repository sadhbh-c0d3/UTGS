#include "UselessPch.h"

#include "ImageWidget.h"

namespace Useless {

ImageWidget::ImageWidget( const RefSkin &ref_skin )
{
    _sp_skin = &CopyOf<Skin>( ref_skin.get() );
    SetDimensions( GetSkin().GetWidth(), GetSkin().GetHeight() );
}

ImageWidget::~ImageWidget()
{}   

void ImageWidget::Repaint(const WidgetPainter& painter)
{
    GetSkin().Paint( painter );
}

void ImageWidget::Resize( int w, int h )    
{
    GetSkin().SafeResize( Dim2i(w,h) );
    SetDimensions( GetSkin().GetWidth(), GetSkin().GetHeight() );
}

void ImageWidget::SetSkin( const RefSkin &ref_skin)
{
    _sp_skin = &CopyOf<Skin>( ref_skin.get() );
    //SetDimensions( GetSkin().GetWidth(), GetSkin().GetHeight() );
    GetSkin().SafeResize( Dim2i( GetWidth(), GetHeight() ));
    SetDirty();
}


};//namespace Useless
