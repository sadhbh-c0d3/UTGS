#include "UselessPch.h"

#include "Useless/GUI/Widgets/ContentWidget.h"
#include "Useless/GUI/Misc/DT_Painter.h"
#include "Useless/GUI/Misc/DT_Inspector.h"
#include "Useless/GUI/Misc/DT_StateChanger.h"
#include "Useless/GUI/Skins/Skin.h"
#include "Useless/Util/Math.h"

namespace Useless {

#pragma warning (disable:4355)

ContentWidget::ContentWidget( const InternalPointer< DataObject > &content,
                              const SkinMan &skins,
                              const FontSet &fonts,
                              const Pos &distance )
   :_content( content ),
    _skins( this, skins ),
    _fonts( fonts ),
    _distance( distance )
{
    UpdateContent();
}


ContentWidget::~ContentWidget()
{
}

void ContentWidget::SetContent( InternalPointer< DataObject > content )
{
    _content = content;
    UpdateContent();
}

InternalPointer< DataObject > ContentWidget::GetContent() const
{
    return _content;
}


void ContentWidget::Repaint( const WidgetPainter &painter )
{
    _skins.Paint( painter, Pos(), GetBoundingRect() );

    // use data painter to paint data object
    Font font_a = _fonts.Get().GetFont( _fonts.GetFontID() );
    Font font_b = font_a; // W/A
    DT_Painter dt_painter( painter, font_a, font_b, _content_area );
    (*_content).AcceptVisitor( dt_painter );
}

void ContentWidget::Resize( int w, int h )
{
    SetDimensions( w, h );
    _skins.SetSize( Dim2i(w, h) );
    UpdateContent();
}

int ContentWidget::GetState() const
{
    assert( _skins.GetState() == _fonts.GetState() );
    return _skins.GetState();
}

bool ContentWidget::SetState (int state)
{
    _skins.Select( state );
    _fonts.Select( state );
    _skins.SetSize( Dim2i( GetWidth(), GetHeight() ));
    DT_StateChanger state_changer(state);
    (*_content).AcceptVisitor( state_changer);
    UpdateContent();
    return true;
}

bool ContentWidget::HasState( int state ) const
{
    return _skins.Get().Exists(state) || _fonts.Get().Exists(state);
}

void ContentWidget::UpdateContent()
{
    int id = _skins.GetSkinID();
    int lt = _skins.Get().GetDimension( id, LEFT_MARGIN );
    int rt = _skins.Get().GetDimension( id, RIGHT_MARGIN );
    int tp = _skins.Get().GetDimension( id, TOP_MARGIN );
    int bt = _skins.Get().GetDimension( id, BOTTOM_MARGIN );
    
    if ( GetWidth() && GetHeight() )
    {
        _content_area = Rect( lt, tp,
              SimpleMax( 0, GetWidth()-(rt+lt)),
              SimpleMax( 0, GetHeight()-(tp+bt))
            );
    }

    Font font_a = _fonts.Get().GetFont( _fonts.GetFontID() );
    Font font_b = font_a; // W/A
    DT_Inspector dt_inspector( font_a, font_b, _content_area );
    (*_content).AcceptVisitor( dt_inspector );
    int content_h = dt_inspector._filled_area.GetH();

    int inner_h = GetHeight()-(tp+bt);

    if ( content_h <= inner_h )
    {
        int y = ( inner_h - content_h + 1) /2;
       _content_area = Rect( lt, tp + y,
               SimpleMax( 0, GetWidth()-(rt+lt)),
               content_h
            );
       //_content_area &= _content_area + _distance;
    }
    // @TODO: ContentWidget: content and marings...
    _content_area += _distance;
    SetDirty();
}

};//namespace Useless
