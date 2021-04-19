#ifndef __INCLUDED__DISPLAY_STATE_H__
#define __INCLUDED__DISPLAY_STATE_H__

#pragma warning(disable:4786)
#include <vector>

namespace Useless {

struct USELESS_API StateFlags
{
    StateFlags( int s=0L ): _state_flags(s) {}
    
    void Set( int flag ) { _state_flags |= flag; }
    void Clr( int flag ) { _state_flags &=~flag; }
    bool Get( int flag ) const { return !!(_state_flags & flag); }

    StateFlags CpSet( int flag ) const { return _state_flags | flag; }
    StateFlags CpClr( int flag ) const { return _state_flags &~flag; }

    operator int () const { return _state_flags; }
    
    int _state_flags;
};
//--------------------------------------------------------------------------------------


struct USELESS_API DisplayState : public StateFlags
{
    /* Fundamental display states */
    enum { NORMAL=0, HIGHLIGHTEN=1, SELECTED=2, DISABLED=4, FOCUSSED=8, USER=16 };
    
    /* Combined states*/
    enum { PRESSED = SELECTED|HIGHLIGHTEN, MARKED = SELECTED };

    DisplayState() {}
    DisplayState( int s ): StateFlags(s) {}
    DisplayState( StateFlags s ): StateFlags(s) {}

    bool IsNormal() const;
    
    int CpSetA( int flag ) { return _state_flags & ~(HIGHLIGHTEN|SELECTED|DISABLED) | flag; }
    int CpGetA() const { return _state_flags & (HIGHLIGHTEN|SELECTED|DISABLED); }

    DisplayState BG_substitute () const;
    DisplayState FG_substitute () const;
};

//--------------------------------------------------------------------------------------
inline bool DisplayState::IsNormal() const 
{
    return !Get(HIGHLIGHTEN) && !Get(SELECTED) && !Get(DISABLED) && !Get(FOCUSSED); 
}

//---------------------------------------------------------------------
struct ButtonImages : public std::vector<int>
{
    ButtonImages()
    {
        push_back( DisplayState::NORMAL );
        push_back( DisplayState::PRESSED );
        push_back( DisplayState::HIGHLIGHTEN );
        push_back( DisplayState::DISABLED );
    }
};

struct SwitchImages : public std::vector<int>
{
    SwitchImages()
    {
        push_back( DisplayState::NORMAL );
        push_back( DisplayState::SELECTED );
        push_back( DisplayState::HIGHLIGHTEN );
        push_back( DisplayState::SELECTED | DisplayState::HIGHLIGHTEN );
        push_back( DisplayState::DISABLED );
        push_back( DisplayState::DISABLED | DisplayState::SELECTED );
    }
};

struct RadioImages : public std::vector<int>
{
    RadioImages()
    {
        push_back( DisplayState::NORMAL );
        push_back( DisplayState::SELECTED );
        push_back( DisplayState::HIGHLIGHTEN );
        push_back( DisplayState::DISABLED );
        push_back( DisplayState::DISABLED | DisplayState::SELECTED );
    }
};


};//namespace Useless
#endif//__INCLUDED__DISPLAY_STATE_H__
