#include "UselessPch.h"

#include "Useless/GUI/Widgets/CaptionWidget.h"

namespace Useless {

#pragma warning (disable:4355)

CaptionWidget::CaptionWidget( const Text    &caption, 
                              const SkinMan &skins, 
                              const FontSet &fonts,
                              unsigned int  alignment,
                                       Pos  distance ):
    _caption(caption),
    _skins( this, skins),
    _fonts( fonts),
    _alignment(alignment),
    _distance(distance),
    _line_spacing(1)
{
    UpdateCaption(0,0,true);
}

CaptionWidget::CaptionWidget( const Text &caption, const CaptionStyle &style ):
    _caption(caption),
    _skins( this, style.skins), 
    _fonts(style.fonts),
    _alignment(style.alignment),
    _distance( style.distance ),
    _line_spacing(1)
{
    UpdateCaption(0,0,true);
}

CaptionWidget::~CaptionWidget()
{
}

void CaptionWidget::SetCaption(const Text &caption)
{
    _caption = caption;
    UpdateCaption( GetWidth(), GetHeight() );
}

Text CaptionWidget::GetCaption() const
{
    return _caption;
}


void CaptionWidget::Repaint( const WidgetPainter &painter )
{
    int skin_height = _skins.GetHeight();
    Pos p = Pos(_skin_pos, GetHeight()/2 - skin_height/2 );

    _skins.Paint( painter, p, GetBoundingRect() );

    Pos t_o = Pos(0,0) * GetDP().Get( DisplayState::SELECTED );
    if ( _alignment > INNER_RIGHT ) { t_o=Pos(); }

    int text_height = _fn_painter.GetHeight();
    Pos cpos( _caption_pos, (GetHeight() - text_height)/2+_distance.y );
    _fn_painter.Paint( painter, cpos + t_o, _line_spacing, _fp_alignment );
}

void CaptionWidget::Resize( int w, int h )
{
    _fn_painter.SetWidth(w);
    UpdateCaption( w, h);
}

int CaptionWidget::GetState () const
{
    assert( _skins.GetState() == _fonts.GetState() );
    return _skins.GetState();
}

bool CaptionWidget::SetState (int state)
{
    _skins.Select( state );
    _fonts.Select( state );
    UpdateCaption( GetWidth(), GetHeight() );
    return true;
}

bool CaptionWidget::HasState (int state) const
{
    return _skins.Get().Exists(state) || _fonts.Get().Exists(state);
}

void CaptionWidget::UpdateCaption( int width, int height, bool update_dimentions )
{
    _fn_painter.SetFont( _fonts.Get().GetFont( _fonts.GetFontID() ) );
    _fn_painter.SetText( _caption );
    
    int text_width  = _fn_painter.GetWidth();
    int text_height = _fn_painter.GetHeight();
    int skin_width  = _skins.GetWidth();
    int skin_height = _skins.GetHeight();
    
    if (skin_width <= 0)  { skin_width = text_width;}
    if (skin_height <= 0) { skin_height = text_height;}

    if (update_dimentions)
    {
        text_width  = _fn_painter.GetStatus().GetWidth();
        text_height = _fn_painter.GetStatus().GetHeight();

        if( _alignment==INNER || _alignment==INNER_LEFT || _alignment==INNER_RIGHT )
        {
            width  = skin_width;
        }
        else
        {
            width = text_width + skin_width + _distance.x + _distance.x/2;
        }
        height = skin_height;
    }

    switch(_alignment)
    {
        case INNER:
            skin_width = width;
            _skin_pos = 0;
            _caption_pos = width/2 + _distance.x;
            _fp_alignment = FontPainter::CENTER;
            text_width = width - ( _distance.x + _distance.x/2 );
            break;

        case INNER_LEFT:
            skin_width = width;
            _skin_pos = 0;
            _caption_pos = _distance.x;
            _fp_alignment = FontPainter::LEFT;
            text_width = width - ( _distance.x + _distance.x/2 );
            break;

        case INNER_RIGHT:
            skin_width = width;
            _skin_pos = 0;
            _caption_pos = width - _distance.x;
            _fp_alignment = FontPainter::RIGHT;
            text_width = width - ( _distance.x + _distance.x/2 );
            break;

        case OUTER_LEFT:
            _skin_pos = width - skin_width - _distance.x/2;
            _caption_pos = _skin_pos - _distance.x;
            _fp_alignment = FontPainter::RIGHT;
            text_width = width - ( skin_width + _distance.x + _distance.x/2 );
            break;

        case OUTER_RIGHT:
            _skin_pos = _distance.x/2;
            _caption_pos = skin_width + _distance.x + _distance.x/2;
            _fp_alignment = FontPainter::LEFT;
            text_width = width - ( skin_width + _distance.x + _distance.x/2 );
            break;
    };

    _fn_painter.SetWidth( text_width );
    _skins.SetSize( Dim2i( skin_width, height)  ); 
    SetDimensions( width, height );
}


};//namespace Useless
