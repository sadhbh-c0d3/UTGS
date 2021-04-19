#include "UselessPch.h"

#include "Useless/GUI/Widgets/SmallHint.h"

namespace Useless {

SmallHint::SmallHint( const Font &font, const Skin &skin, const Text &text, int alignment )
    :AdvTextWidget( font, text, 1, alignment ), _sp_skin(&CopyOf<Skin>(skin))
{
    SetDimensions( skin.GetWidth(), skin.GetHeight() );
}

SmallHint::~SmallHint()
{
}

void SmallHint::Repaint(const WidgetPainter &painter)
{
    int lt = _sp_skin->GetDimension( LEFT_MARGIN );
    int tp = _sp_skin->GetDimension( TOP_MARGIN );
    _sp_skin->Paint( painter );
    WidgetPainter text_painter( painter, Pos(lt, tp) );
    AdvTextWidget::Repaint(text_painter);
}

void SmallHint::Resize( int w, int h )
{
    ((Skin&)*_sp_skin).SafeResize( Dim2i(w,h) );
    w = _sp_skin->GetWidth();
    h = _sp_skin->GetHeight();
    int dx = _sp_skin->GetDimension( RIGHT_MARGIN ) - _sp_skin->GetDimension( LEFT_MARGIN );
    int dy = _sp_skin->GetDimension( BOTTOM_MARGIN ) - _sp_skin->GetDimension( TOP_MARGIN );
    AdvTextWidget::Resize( w-dx, h-dy );
    SetDimensions( w, h);
}

void SmallHint::FitText()
{
    int mx = _sp_skin->GetDimension( RIGHT_MARGIN ) + _sp_skin->GetDimension( LEFT_MARGIN );
    int my = _sp_skin->GetDimension( BOTTOM_MARGIN ) + _sp_skin->GetDimension( TOP_MARGIN );

    Status s = GetStatus();
    int text_width = s.GetWidth();
    int text_height = s.GetHeight();

    Resize( text_width + mx, text_height + my );
}

};//namespace Useless
