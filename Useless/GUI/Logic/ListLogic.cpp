#include "UselessPch.h"

#include "ListLogic.h"
#include "Useless/System/KeyCodes.h"

namespace Useless {

ListLogic::ListLogic(): _hiliten(-1), _selector(-1), _focussed(-1)
{
    Init();

    _follow_mode      = DONT_FOLLOW;
    _selector_selects = false;
    _allways_selected = false;
}


void ListLogic::Init()
{
    Tie2Signal( OnPress[0], this, &Widget::FocusIntercept );

    Tie2Signal( WhileMoving , this, &ListLogic::PosHilite );
    Tie2Signal( WhileDrag[0], this, &ListLogic::PosHilite );
    Tie2Signal( WhileDrag[0], this, &ListLogic::PosSelect );
    Tie2Signal( OnPress[0]  , this, &ListLogic::PosSelect );
    Tie2Signal( OnClick[0]  , this, &ListLogic::DoSelect  );
    Tie2Signal( OnRelease[0], this, &ListLogic::PosDeselect );
    
    for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
    {
          OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
        OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
    }

    OnFocus.TieUnary( this, &ListLogic::SetFocussed, true );
    OffFocus.TieUnary( this, &ListLogic::SetFocussed, false );

    Tie2Signal( OnKeyPress, this, &ListLogic::HandleKey);
    Tie2Signal( WhileKey,   this, &ListLogic::HandleKey);

    OnMoveOutside.TieUnary( this, &ListLogic::HiliteItem, -1);

    EnableKey(Keys::UP);
    EnableKey(Keys::DOWN);
    EnableKey(Keys::RET);
    EnableKey(Keys::SPACE);
    EnableKey(Keys::PGUP);
    EnableKey(Keys::PGDN);

    ActiveWidget::SetFocusAdaptive(true);
}

ListLogic::~ListLogic()
{
}

void ListLogic::HiliteItem( int item_to_hl )
{
    if ( item_to_hl != _hiliten )
    {
        SetState( _hiliten,   MkHL( GetState( _hiliten ), false ) );
        SetState( _hiliten = item_to_hl, MkHL( GetState( item_to_hl ), true  ) );

        if ( _follow_mode==FULL_FOLLOW )
        {
            SelectItem(item_to_hl);
        }
    }
}

void ListLogic::SelectItem( int item_to_sel )
{
    if ( ( item_to_sel>-1 || _follow_mode!=DONT_FOLLOW && _follow_mode!=FOCUS_FOLLOW ) &&
           item_to_sel!=_selector )
    {
        SetState( _selector,   MkSEL( GetState( _selector ), false ) );
        SetState( _selector = item_to_sel, MkSEL( GetState( item_to_sel ), true  ) );

        if (_selector_selects)
        {
            DoSelect();
        }
        else if ( _follow_mode!=FULL_FOLLOW && _follow_mode!=FOCUS_FOLLOW )
        {
            SetState( _focussed,   MkFOC( GetState( _focussed ), false ) );
            SetState( item_to_sel, MkFOC( GetState( item_to_sel ), true  ) );
        }
        _focussed = item_to_sel;
    }
}

void ListLogic::FocusItem( int item_to_sel )
{
    if ( !_selector_selects && _follow_mode!=FULL_FOLLOW && _follow_mode!=FOCUS_FOLLOW )
    {
        SetState( _focussed,   MkFOC( GetState( _focussed ), false ) );
        SetState( item_to_sel, MkFOC( GetState( item_to_sel ), true  ) );
    }
    _focussed = item_to_sel;
    if (item_to_sel>-1)
    {
        OnActorMove.Send_1( GetItemBound(item_to_sel) );
    }
}

void ListLogic::PosHilite( const Pos &p )
{
    HiliteItem( ItemAtPosition(p) );
}

void ListLogic::PosSelect( const Pos &p )
{
    int item = ItemAtPosition(p);
    SelectItem( item );
    FocusItem( item );
}

void ListLogic::SetFocussed( bool t )
{
    if ( _follow_mode==DONT_FOLLOW && NoButtons() )
    {
        if ( t )
        {
            FocusItem( ((int)varSelector>0)? (int)varSelector:0 );
        }
        else
        {
            FocusItem(-1);
        }
    }
}
    
void ListLogic::PosDeselect( const Pos &where )
{
    if ( !IsOver( where ) && !_allways_selected )
    {
        varSelector=-1;
    }
}

void ListLogic::DoSelect()
{
    if (!IsDisabled(_selector))
    { 
        if (!_allways_selected || _selector!=-1)
        {
            varSelector=_selector;
        }
        else
        {
            varSelector=0; 
        }
        SetFocussed( HasFocus() );
        OnConfirm.Send_1(varSelector);
    }
}

int ListLogic::GetSelected() const
{
    return varSelector;
}

void ListLogic::SetSelected(int s)
{
    if ( s>-1 ) 
    {
        SelectItem(s);
        DoSelect();
    }
    else
    {
        HideSelector();
        varSelector = s;
    }    
}

void ListLogic::HideSelector()
{
    Widget::SetDirty( GetItemBound(_selector) );
    SetState( _selector, MkSEL( GetState( _selector ), false ) );
    _selector=-1;
}

void ListLogic::EnableItem ( int n )
{
    SetState( n, MkDIS( GetState(n), false ) );
}

void ListLogic::DisableItem( int n )
{
    SetState( n, MkDIS( GetState(n), true ) );
}

inline bool ListLogic::IsDisabled( int n ) const
{
    return GetDIS( GetState(n) );
}

void ListLogic::SetUserFlagForItem( int n, int flagNo, bool value )
{
    int flag = USER << flagNo;
    SetState( n, Set( flag, GetState(n), value ));
}

bool ListLogic::GetUserFlagForItem( int n, int flagNo ) const
{
    int flag = USER << flagNo;
    return Get( flag, GetState(n) );
}

void ListLogic::OnItemRemove( int item_no )
{
    assert( item_no > -1 );

    if ( item_no <= _hiliten )
    {
        SetState( _hiliten, MkHL( GetState( _hiliten ), false ) ); ++_hiliten;
        SetState( _hiliten, MkHL( GetState( _hiliten ), true  ) );
    }
    if ( item_no < _selector )
    {
        SetState( _selector, MkSEL( GetState( _selector ), false ) ); --_selector;
        SetState( _selector, MkSEL( GetState( _selector ), true  ) );
    }

    if ( item_no < (int)varSelector )
    {
        varSelector.QuietUpdate( (int)varSelector -1 );
    }
    if ( item_no == (int)varSelector )
    {
        varSelector.QuietUpdate( -1 );
    }
}

void ListLogic::OnItemsClear()
{
    _hiliten = -1;
    _selector = -1;
    _focussed = -1;
    varSelector = -1;
}

void ListLogic::ScrollPage( bool dir )
{
    Rect area = GetParent()->GetClientRect( this );
    Rect ir;
    int  i = _focussed;
    int  n = GetNumItems();
    int  h = 0;
    
    if ( !dir )
    {
        while ( h < area.GetH() && i>-1 )
        {
            ir = GetItemBound( i-- );
            h += ir.GetH();
        } ++i;
    }
    else
    {
        while ( h < area.GetH() && i<n )
        {
            ir = GetItemBound( i++ );
            h += ir.GetH();
        } --i;
    }

    //SelectItem(i);
    //OnActorMove.Send_1( GetItemBound(i) );
    if (_selector_selects)
    {
        SelectItem(i);
        DoSelect(); 
    }
    FocusItem(i);
}

void ListLogic::EnableKeyboard( bool t )
{
    EnableKey(Keys::UP, t);
    EnableKey(Keys::DOWN, t);
    EnableKey(Keys::RET, t);
    EnableKey(Keys::SPACE, t);
    EnableKey(Keys::PGUP, t);
    EnableKey(Keys::PGDN, t);
}

void ListLogic::HandleKey(int key)
{
    bool handled = false;
    int  last_item = GetNumItems()-1;
    int  item_to_select = _focussed; //_selector;

    switch( key )
    {
        case Keys::UP:
            {
                if ( item_to_select==-1) 
                { 
                    item_to_select=last_item;
                }
                else
                {
                    if ( item_to_select > 0 )
                    {
                        --item_to_select;
                    }
                }
                handled = true;
            }
            break;

        case Keys::DOWN:
            {
                if ( item_to_select==-1) 
                { 
                    item_to_select=0;
                }
                else
                {
                    if ( item_to_select < last_item )
                    {
                        ++item_to_select;
                    }
                }
                handled = true;
            }
            break;

        case Keys::PGUP:
            ScrollPage(0);
            break;

        case Keys::PGDN:
            ScrollPage(1);
            break;

        case Keys::RET:
        case Keys::SPACE:
            SelectItem(item_to_select);
            DoSelect();
            break;
    }

    if (handled)
    {
        if ( _selector_selects || _follow_mode==FOCUS_FOLLOW || _follow_mode==FULL_FOLLOW )
        {
            SelectItem(item_to_select);
        }
        FocusItem(item_to_select);
    }
}

void ListLogic::SetFollowMode( Follow f )
{
    _follow_mode=f;
    if ( _follow_mode==FULL_FOLLOW || _follow_mode==FOCUS_FOLLOW )
    {
        SetState( _focussed, MkFOC( GetState( _focussed ), false ) );
    }
} 

void ListLogic::SelectorSelects( bool s )
{
    _selector_selects=s;
    if ( _selector_selects )
    {
        SetState( _focussed, MkFOC( GetState( _focussed ), false ) );
    }
} 


};//namespace Useless;
