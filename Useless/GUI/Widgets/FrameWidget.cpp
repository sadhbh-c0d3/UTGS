#include "UselessPch.h"

#include "Useless/GUI/Widgets/FrameWidget.h"

#include "Useless/GUI/Skins/FontSkin.h"
#include "Useless/GUI/Text/Font.h"

namespace Useless {

FrameWidget::FrameWidget(int w, int h, const Skin &skin ): 
             HubWidget(w,h), _sp_skin(&CopyOf<Skin>(skin))
{
    _frame_left    = skin.GetDimension( LEFT_MARGIN );
    _frame_top     = skin.GetDimension( TOP_MARGIN );
    _frame_right   = skin.GetDimension( RIGHT_MARGIN );
    _frame_bottom  = skin.GetDimension( BOTTOM_MARGIN );

    _sp_skin->SetSize( Dim2i(w,h) );
}

FrameWidget::FrameWidget(const FrameStyle &style):
             HubWidget(style.w,style.h), _sp_skin(&CopyOf<Skin>(style.skin))

{
    if (style.frame_left>-1)   _frame_left   = style.frame_left;
    else _frame_left = style.skin.GetDimension( LEFT_MARGIN );

    if (style.frame_top>-1)    _frame_top    = style.frame_top;
    else _frame_top = style.skin.GetDimension( TOP_MARGIN );

    if (style.frame_right>-1)  _frame_right  = style.frame_right;
    else _frame_right = style.skin.GetDimension( RIGHT_MARGIN );

    if (style.frame_bottom>-1) _frame_bottom = style.frame_bottom;
    else _frame_bottom  = style.skin.GetDimension( BOTTOM_MARGIN );

    _sp_skin->SetSize( Dim2i(style.w,style.h) );
}

FrameWidget::~FrameWidget()
{}

void FrameWidget::Resize(int w, int h)
{
    _sp_skin->SetSize( Dim2i(w,h) );
    HubWidget::Resize(w,h);
}

void FrameWidget::RepaintAction(const WidgetPainter &painter)
{
    _sp_skin->Paint( painter );
}

Rect FrameWidget::GetClientRect( const Widget *sender ) const
{
    Rect r( 0,0, GetWidth(),GetHeight() );
    if ( IsChild(sender) )
    {
        return (r-Pos(_frame_right,_frame_bottom) ) & (r+Pos(_frame_left,_frame_top) );
    }
    else
    {
        return r;
    }
}

};//namespace Useless
