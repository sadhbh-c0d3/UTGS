#include "UselessPch.h"

#include "TextField.h"
#include "Useless/Functional/DelayedExecutor.h"
#include "Useless/GUI/Skins/Skin.h" //included for enums

namespace Useless {

TextField::TextField( int w, int h, const FontSet &fonts, const SkinMan &skins , const Text &text )
    : _fonts(fonts), _sp_skins(&CopyOf<SkinMan>(skins)), TextEditLogic(text),
      _begin_marker(0), _end_marker(0), _text_shift(0), _status(NORMAL), _blink(1),
      _alignment(LEFT)
{
    const Font &font = GetFont(NORMAL);
    
    assert( font.GetWidth(BEGIN) );  _marker_width = font.GetWidth(BEGIN);
    //assert( !font.GetWidth(END) || font.GetWidth(END) == _marker_width );

    _letters = font.GetStats( GetText() );
    GetSkinMan().SetSizeAll( Dim2i(w,h) );
    
    int available_skin = ( GetSkinMan().Exists(FOCUSSED) )? FOCUSSED : NORMAL;

    _frame_left = GetSkinMan().GetDimension( available_skin, LEFT_MARGIN );
    _frame_right = GetSkinMan().GetDimension( available_skin, RIGHT_MARGIN );

    SetDimensions( w, h );
    
    Tie2Signal( OnCarMove, this, &TextField::ApplyMotion );
    Tie2Signal( OnTextChange, this, &TextField::ApplyChange );
    Tie2Signal( OnFocus, this, &TextField::Blink );
}

TextField::~TextField()
{
}

//-- Widget overloads
void TextField::Repaint( const WidgetPainter &a_painter )
{
    int w = GetWidth();
    int h = GetHeight();
    int text_y = ( h - GetFont(_status).GetHeight() )/2;
    int kern = GetFont(_status).GetKerning();
    Pos half_kern( (kern + _marker_width)/2, 0);
    int selection_w = _end_marker - _begin_marker;
    int odd_correcion = ( kern%2 != 0 )? 1 : 0;

    int origin = GetOrigin();

    int skin_id = GetSkinID(_status);
    if ( skin_id )
    {
        GetSkinMan().Cooperate( skin_id, this);
        GetSkinMan().Paint( skin_id, a_painter );
    }

    WidgetPainter painter( a_painter );
    if ( painter.Intersect( Rect(_frame_left,0,w-(_frame_left+_frame_right),h) ) )
    if ( selection_w )
    {
        int selection_h = GetFont(_status).GetHeight();
        Pos selector_pos( _begin_marker + origin - (kern)/2 - odd_correcion, text_y );
        Pos front_pos( origin, text_y );
        Pos sel_pos( _begin_marker + origin, text_y );
        Pos back_pos( _end_marker + origin, text_y );

        int first = 0;
        int begin = Begin();
        int end  = End();
        int last = GetText().length()-1;

        Text front_text = GetText()( 0, begin );
        Text sel_text   = GetText()( begin, end );
        Text back_text  = GetText()( end, last+1 );
        bool used_skin;

        if ( used_skin = GetSkinMan().Exists(SELECTED) ) // try to use selection skin
        {
            GetSkinMan().Cooperate( SELECTED, this);
            GetSkinMan().SafeResize( SELECTED, Dim2i( selection_w + odd_correcion, selection_h) );
            GetSkinMan().Paint( SELECTED, painter, selector_pos );
        }

        if ( _fonts.Exists(SELECTED) ) // try to use special font
        {
            GetFont(SELECTED).Paint( painter, BEGIN, sel_pos-half_kern );
            GetFont(SELECTED).Paint( painter, END, back_pos-half_kern );
            GetFont( _status).Paint( painter, front_text, front_pos );
            GetFont(SELECTED).Paint( painter, sel_text, sel_pos );
            GetFont( _status).Paint( painter, back_text, back_pos );
        }
        else
        {
            if ( !used_skin ) // try to use inverted font
            {
                sel_text = Text("\020[n]") + sel_text + Text("\020[/n]");
            }
            GetFont(_status).Paint( painter, front_text, front_pos );
            GetFont(_status).Paint( painter, sel_text, sel_pos );
            GetFont(_status).Paint( painter, back_text, back_pos );
        }
    }
    else
    {
        if ( HasFocus() )
        {
            Pos car_pos( _begin_marker + origin, text_y );
            GetFont(_status).Paint( painter, (_blink)? BEGIN : END, car_pos-half_kern );
        }

        Pos text_pos( origin, text_y );
        GetFont(_status).Paint( painter, GetText(), text_pos );
    }
}

void TextField::Blink()
{
    if ( HasFocus() )
    {
        _blink = !_blink;//-- alternate blink status
        Pos half_kern( (GetFont(_status).GetKerning() + _marker_width)/2, 0);
        Pos car_pos( _begin_marker + GetOrigin(), 0 );
        Rect car_rc( car_pos.x, car_pos.y, _marker_width, GetHeight() );
        SetDirty( car_rc - half_kern);//-- set dirty car area

        DelayedExecutor::TaskInfo timed;
        timed.p_func = PackFunctor( VoidCall(this, &TextField::Blink) );
        timed.delay = 0.5;
        OnTimer::Instance().Schedule(timed);//-- schedule next blink
    }
    else { _blink=1; }
}

void TextField::Resize ( int w, int h )
{
    GetSkinMan().SetSizeAll( Dim2i(w,h) );
    SetDimensions( w, h);
}

//-- TextEditLogic overloads
bool TextField::SetState( int id )
{
    if ( _fonts.Exists(id) || GetSkinID(id) ) 
    {
        _status=id; SetDirty(); return true; 
    }
    return false;
}

bool TextField::HasState( int id ) const
{
    if ( _fonts.Exists(id) ) { return true; }
    return false;
}

int TextField::GetState() const
{
    return _status;
}

void TextField::ApplyMotion( int car )
{
    int _w = GetWidth();
    int i,w, n = _letters.Size();
    int begin = Begin();
    int end = End();
    for ( i=0, w=0; i<begin && i<n; ++i)
    {
        w += _letters[i];
    }
    _begin_marker = w;
    for ( ; i<end && i<n; ++i)
    {
        w += _letters[i];
    }
    _end_marker = w;
    int actor = ( car==end )? _end_marker : _begin_marker;
    int effective_w = _w - (_frame_left+_frame_right) - 2*_marker_width;
    while ( actor-_text_shift > effective_w) { _text_shift += _w/4 ;}
    while ( actor-_text_shift < 0 ) { _text_shift = _text_shift<_w/4 ? 0 : _text_shift-_w/4; }

    Widget::SetDirty();
}

void TextField::ApplyChange()
{
    _letters = GetFont(_status).GetStats( GetText() );
    Widget::SetDirty();
}

int TextField::GetLetterAt( const Pos &pos ) const
{
    int origin = GetOrigin();
    int i=0, w=0, n=_letters.Size();
    for( ; ( i < n ) && ( w + _letters[i]/2 <= pos.x - origin ); ++i )
    {
        w += _letters[i];
    }
    return (i<0)? 0 : i;
}

int TextField::GetOrigin() const
{
    int marker_shift = _marker_width;//(_marker_width+1)/2;

    if ( _alignment == RIGHT && !_text_shift )
    {
        int w = GetWidth();
        int text_w = GetFont(_status).GetWidth( GetText() );
        int dx = w - text_w - _frame_right - marker_shift;

        if (dx > 0 )
            return w - text_w - _frame_right - marker_shift;
    }
    return _frame_left-_text_shift+marker_shift;
}

bool TextField::Validate( int &ch ) const
{
    if ( TextEditLogic::Validate( ch ))
    {
        return (GetFont( _status ).GetWidth( ch ) > 0);
    }
    else
    {
        return false;
    }
}

};//namespace Useless
