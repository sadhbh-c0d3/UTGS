#include "UselessPch.h"

#include "SkinSelector.h"

namespace Useless {

bool SkinSelector::_g_gen_highlight = false;
bool SkinSelector::_g_gen_focussed = true;

SkinSelector::SkinSelector   ( Widget *parent,
        const SkinMan &skin_manager )
    :_parent(parent),
    _sp_manager( &CopyOf<SkinMan>(skin_manager)),
    _available_state(DisplayState::NORMAL)
{
    C_verify(_parent);
    

    _gen_highlight = _g_gen_highlight && !_sp_manager->Exists( DisplayState::HIGHLIGHTEN );
    _gen_focussed  = _g_gen_focussed && !_sp_manager->Exists( DisplayState::FOCUSSED );
    
    if ( _sp_manager->Exists(_available_state))
        _sp_manager->Cooperate( _available_state, _parent);
    //else
    //    C_warning( _sp_manager->Exists(_available_state) );
}

SkinSelector::SkinSelector( Widget *parent,
        const SPointer<SkinMan> &sp )
    :_parent(parent),
    _sp_manager(sp),
    _available_state(DisplayState::NORMAL)
{
    C_verify(_parent);

    _gen_highlight = _g_gen_highlight && !_sp_manager->Exists( DisplayState::HIGHLIGHTEN );
    _gen_focussed  = _g_gen_focussed && !_sp_manager->Exists( DisplayState::FOCUSSED );

    if ( _sp_manager->Exists(_available_state))
        _sp_manager->Cooperate( _available_state, _parent);
    //else
    //    C_warning( _sp_manager->Exists(_available_state) );
}


void SkinSelector::Paint( const Painter &painter, Pos p, const Rect &border )
{
    if ( _available_state > -1 )
    {
        _sp_manager->Paint( _available_state, painter, p );
    }
    else if ( _gen_highlight && _state.Get( DisplayState::HIGHLIGHTEN ) && !!border )
    {
        painter.PaintBoundary( border, 0xE0E0E0, 0xE0E0E0, 0x000000, 0x000000 );
    }

    if ( _gen_focussed && _state.Get( DisplayState::FOCUSSED ) && !!border )
    {
        painter.PaintBoundary( border, 0x000000, 0x000000, 0xE0E0E0, 0xE0E0E0 );
    }
}


};//namespace Useless
