#ifndef __INCLUDED__USELESS__FONT_SELECTOR_H__
#define __INCLUDED__USELESS__FONT_SELECTOR_H__

#include "Useless/GUI/Misc/DisplayState.h"
#include "Useless/GUI/Text/FontSet.h"

namespace Useless {

class USELESS_API FontSelector
{
public:
    FontSelector( const FontSet &font_manager );

    bool Select ( DisplayState state );

    int  GetWidth ( const Text &text ) const;
    int  GetHeight() const;
    
    void Paint( const Painter &painter, const Text &text, Pos p=Pos() );
    
    int  GetState()  const { return _state; }
    int  GetFontID() const { return _available_state; }

    FontSet &Get() { return _manager; }
    const FontSet &Get() const { return _manager; }

private:
    FontSet      _manager;
    DisplayState _state;
    int          _available_state;
};
//---------------------------------------------------------------

inline FontSelector::FontSelector( const FontSet &font_manager )
    : _manager(font_manager), _available_state(DisplayState::NORMAL)
{
    //C_warning( _manager.Exists(_available_state) );
}

inline int  FontSelector::GetWidth ( const Text &text ) const
{
    return _manager.GetWidth( _available_state, text );
}
inline int  FontSelector::GetHeight() const
{
    return _manager.GetHeight( _available_state );
}

inline bool FontSelector::Select( DisplayState state )
{
    _state = state; bool exists;
    if (!_manager.Exists(DisplayState::FOCUSSED)) { state.Clr(DisplayState::FOCUSSED); }
    while ( !(exists=_manager.Exists(state)) && state )
    {
        state = state.FG_substitute();
    }
    _available_state = state;
    return exists;
}


};//namespace Useless
#endif//__INCLUDED__USELESS__FONT_SELECTOR_H__
