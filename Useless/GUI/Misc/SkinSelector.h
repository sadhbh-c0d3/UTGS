#ifndef __INLCUDED__USELESS__SKIN_SELECTOR_H__
#define __INLCUDED__USELESS__SKIN_SELECTOR_H__

#include "Useless/GUI/Misc/DisplayState.h"
#include "Useless/GUI/Skins/SkinMan.h"

namespace Useless {

class USELESS_API SkinSelector
{
public:
    SkinSelector    () {}
    SkinSelector    ( Widget *parent, const SkinMan &skin_manager );
    SkinSelector    ( Widget *parent, const SPointer<SkinMan> &sp );

    bool
    Select          ( DisplayState state );

    int
    GetWidth        () const;
    
    int
    GetHeight       () const;
    
    void
    SetSize         ( Dim2i s );
    
    void
    Paint           ( const Painter &painter, Pos p=Pos(), const Rect &border=Rect());
    
    int
    GetState        () const { return _state; }
    
    int
    GetSkinID       () const { return _available_state; }

    SkinMan &
    Get             () { return *_sp_manager; }
    
    const SkinMan &
    Get             () const { return *_sp_manager; }
    
    bool _gen_highlight; //generate highlight frame
    bool _gen_focussed;  //generate focussed frame

private:
    Widget             *_parent;
    SPointer< SkinMan > _sp_manager;
    DisplayState        _state;
    DisplayState        _available_state;

public:
    static bool _g_gen_highlight;
    static bool _g_gen_focussed;

};
//---------------------------------------------------------------

inline int
SkinSelector::GetWidth () const
{
    return _sp_manager->GetWidth( _available_state );
}

inline int
SkinSelector::GetHeight() const
{
    return _sp_manager->GetHeight( _available_state );
}

inline void
SkinSelector::SetSize( Dim2i s )
{
    //_sp_manager->SetSize( _available_state,s );
    _sp_manager->SafeResize( _available_state,s );
}

inline bool
SkinSelector::Select( DisplayState state )
{
    _state = state; bool exists;

    if (!_sp_manager->Exists(DisplayState::FOCUSSED))
    {
        state.Clr(DisplayState::FOCUSSED);
    }
    while ( !(exists=_sp_manager->Exists(state)) && state )
    {
        state = state.BG_substitute();
    }
    
    _available_state = state;
    
    if (!exists)
    {
        _available_state = -1;
    }
    else
    {
        _sp_manager->Cooperate( _available_state, _parent);
    }
    return exists;
}

};//namespace Useless
#endif//__INLCUDED__USELESS__SKIN_SELECTOR_H__
