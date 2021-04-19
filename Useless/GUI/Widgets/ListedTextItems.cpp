#include "UselessPch.h"

#include "ListedTextItems.h"

namespace Useless {

#pragma warning(disable:4355)

ListedTextItems::ListedTextItems( const FontSet &fonts, const SkinMan &skins, int shift ):
    _skins( this, skins), _fonts(fonts), _shift(shift)
{
    _entry_height = _fonts.GetHeight() + 2;
    _selector_size = _skins.Get().GetMaxSize();
    _entry_height = (_entry_height>_selector_size.y)? _entry_height:_selector_size.y;
}

ListedTextItems::~ListedTextItems()
{
}

int ListedTextItems::GetState( int item_no ) const
{
    if ( item_no >=0 && item_no<GetNumItems() )  { return GetItem(item_no)._state;  }
    else                                         { return 0; }
}

bool ListedTextItems::SetState( int item_no, int state_no )
{
    if ( item_no >=0 && item_no<GetNumItems() )  
    { 
        GetItem(item_no)._state=state_no;  
        SetDirty();
        return true;
    }
    else return false;
}

bool ListedTextItems::HasState( int item_no, int state_no )
{
    if ( item_no >=0 && item_no<GetNumItems() )
    {
        return _skins.Get().Exists(state_no) || _fonts.Get().Exists(state_no);
    }
    return false;
}

int ListedTextItems::ItemAtPosition( const Pos &pos) const
{
    if ( GetBoundingRect().IsInside( pos.x, pos.y ) ) 
    { 
        return pos.y/_entry_height; 
    }
    return -1;
}

void ListedTextItems::Repaint( const WidgetPainter &painter )
{
    int entry_h = GetEntryHeight();
    int font_h = GetFontHeight();
    int shy = ( entry_h - font_h +1)/2;

    if (!_items.Empty())
    {
        int shsel = (_shift<0)? (-_shift) : 0;
        int shtxt = (_shift<0)? 0: _shift;
        int i,n,y;
        bool be_always_dirty = false;
        for( i=0, n=_items.Size(), y=0; i<n; ++i )
        {
            __Item k = _items[i];
 
            _skins.Select(k._state);
            _skins.Paint( painter, Pos( shsel,y ), Rect(0,0,GetWidth(),entry_h)+Pos( shsel,y ) );

            _fonts.Select(k._state);
            _fonts.Paint( painter, k._caption, Pos(shtxt, y+shy) );
            
            be_always_dirty |= IsAlwaysDirty();

            y += entry_h;
        }
        
        BeAlwaysDirty( be_always_dirty);
    }
}

void ListedTextItems::PaintArea( const Painter &painter, const Rect &area, int stateID)
{
    if ( _skins.Select(stateID) )
    {
        int cur_w = _skins.GetWidth();
        int cur_h = _skins.GetHeight();
        _skins.SetSize( Dim2i( area.GetW(), area.GetH()));
        _skins.Paint( painter, area.GetPos());
        _skins.SetSize( Dim2i( cur_w, cur_h));
    }
}

void ListedTextItems::Insert(const Text& text)
{
    __Item item;
    item._caption = text;
    item._state = 0;
    _items.Insert(item);
    CheckSize(text);
}

void ListedTextItems::SetText( int item_no, const Text &text )
{
    if ( item_no >=0 && item_no<GetNumItems() )  
    { 
        GetItem(item_no)._caption=text; 
        CheckSize(text);
    }
}

void ListedTextItems::CheckSize(const Text &text)
{
    _fonts.Select(DisplayState::NORMAL);
    int w = _fonts.GetWidth(text);
    int _w = GetWidth();

    w += 2*( (_shift<0)? -_shift:_shift );

    /* Hug items with selector
    ---------------------------*/
    if ( w>_w ) 
    {
        _w = w;
        SetSelectorSize( w, _entry_height);
    }
    SetDimensions( _w, GetH() );
}


void ListedTextItems::Remove(int id)
{
    if (!_items.Empty() && id>-1 && id<_items.Size() )
    {
        _items.Remove(id);
        OnItemRemove(id);
        SetDimensions( GetWidth(), GetH() );
    }
}

void ListedTextItems::ClearEntries()
{
    Rect oldrect = GetBoundingRect();
    _items.Clear();
    OnItemsClear();
    SetDimensions( GetWidth(), GetH() );
}

int ListedTextItems::GetItemNo( const Text &text) const
{
    int i,n;
    for ( i=0, n=_items.Size(); i<n && _items[i]._caption!=text; ++i ) {}
    
    return (i<n)? i : -1;
}

void ListedTextItems::Remove(const Text& text)
{
    Remove( GetItemNo(text) );
}

void ListedTextItems::SetText( const Text &prev_text, const Text &new_text )
{
    SetText( GetItemNo(prev_text), new_text );
}

int ListedTextItems::GetFontHeight () const
{
    return _fonts.Get().GetFont(DisplayState::NORMAL).GetHeight();
}

int ListedTextItems::GetEntryHeight() const
{
    return _entry_height;
}

void ListedTextItems::SetEntryHeight( int h)
{
    _entry_height = h;
    Widget::SetDirty();
}

int ListedTextItems::GetDefaultEntryHeight() const
{
    return GetFontHeight() + 2;
}

Dim2i ListedTextItems::GetSelectorSize() const
{
    return (!!_selector_size)? _selector_size : _skins.Get().GetMaxSize();
}

void ListedTextItems::SetSelectorSize( int w, int h)
{
    int _w = GetWidth();
    if ( h==-1 ) { h=_entry_height; }

    _skins.Get().SetSizeAll( Dim2i(w,h) );
    
    _selector_size = _skins.Get().GetMaxSize();

    if (           _w  < _selector_size.x )          { _w = _selector_size.x; }
    if ( _entry_height < _selector_size.y ) { _entry_height=_selector_size.y; }

    SetDimensions( _w, GetH() );
}

Rect ListedTextItems::GetItemBound( int item_no  ) const
{
    if ( item_no >=0 && item_no<GetNumItems() )  
    {
        return Rect( 0, _entry_height * item_no, GetWidth(), _entry_height );  
    }
    else return Rect(); 
}

void ListedTextItems::SetupSkin( int flags )
{
    _skins._gen_focussed = flags & GEN_FOCUSSED;
    _skins._gen_highlight = flags & GEN_HIGHLIGHT;
}

};//namespace Useless 
