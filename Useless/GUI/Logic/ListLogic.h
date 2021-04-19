#ifndef __INCLUDED__USELESS_LIST_LOGIC_H__
#define __INCLUDED__USELESS_LIST_LOGIC_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Logic/MultiItemLogic.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"
#include "Useless/Util/CPT.h"

namespace Useless {

class USELESS_API ListLogic : virtual public ActiveWidget, virtual public EnumWidget, virtual private MultiItemLogic
{
public:
    enum { NORMAL=0, HIGHLITEN=1, SELECTED=2, DISABLED=4, FOCUSSED=8, USER=16 };

             ListLogic();
    virtual ~ListLogic();
#   include "ListLogic.inl" // ParmList Ctor

    virtual  int GetSelected() const;
    virtual void SetSelected( int s);

    virtual void OnItemRemove( int item_no );
    virtual void OnItemsClear();

    void EnableItem ( int n );
    void DisableItem( int n );
    bool IsDisabled ( int n ) const;
    void SetUserFlagForItem( int n, int flagNo, bool value );
    bool GetUserFlagForItem( int n, int flagNo ) const;
    void ScrollPage ( bool dir ); // dn=0 up=1
    void EnableKeyboard( bool t );


private:
    /*
     *  selected item - varSelector
     *  hiliten       - _hiliten
     *  keyboard sel  - _selector
     */
     int  _hiliten;
     int  _selector;
     int  _focussed;

     void HiliteItem( int n );
     void SelectItem( int n );
     void FocusItem ( int n );
     void PosHilite ( const Pos &p );
     void PosSelect ( const Pos &p );
     void PosDeselect( const Pos &p );
     void DoSelect  ();
     void HandleKey ( int key );
     void SetFocussed( bool );
     void Init();

public:
      static  int Set( int dest_state, int src_state, bool t );
      static bool Get( int dest_state, int src_state );
      static  int MkHL  ( int state, bool t);
      static  int MkSEL ( int state, bool t);
      static  int MkDIS ( int state, bool t);
      static  int MkFOC ( int state, bool t);
      static bool GetHL ( int state );
      static bool GetSEL( int state );
      static bool GetDIS( int state );
      static bool GetFOC( int state );


/* Additional Features
---------------------------------------------------------------------------------------------------*/
public:
    typedef enum { 
                    DONT_FOLLOW,  //!< Selector won't follow anything
                    FULL_FOLLOW,  //!< Selector will follow mouse-move
                    DRAG_FOLLOW,  //!< Selector will follow mouse-drag
                    FOCUS_FOLLOW  //!< Selector will follow focus move
                 } Follow;

            //! tell selector image to follow
            void SetFollowMode( Follow f );
            //! tell to keep focussed item allways selected (follow focus)
            void SelectorSelects(bool s=true);
            //! prevent deselecting if selector image dissappears because of mouse-out event
            void AllwaysSelected(bool s=false) {_allways_selected=s;}
            //! Just hide selector image
            void HideSelector();

private:
     Follow    _follow_mode;
     bool _selector_selects;
     bool _allways_selected;     
};



/*
--------------------------------------------------*/

inline int ListLogic::Set( int dest_state, int src_state, bool t )
{
    return (t)? src_state|dest_state : src_state&~dest_state;
}

inline bool ListLogic::Get( int dest_state, int src_state )
{
    return src_state&dest_state;
}

inline int ListLogic::MkHL( int state, bool t )
{
    return Set( HIGHLITEN, state, t );
}

inline int ListLogic::MkSEL( int state, bool t )
{
    return Set( SELECTED, state, t );
}

inline int ListLogic::MkDIS( int state, bool t )
{
    return Set( DISABLED, state, t );
}

inline int ListLogic::MkFOC( int state, bool t )
{
    return Set( FOCUSSED, state, t );
}

inline bool ListLogic::GetHL( int state )
{
    return Get( HIGHLITEN, state );
}

inline bool ListLogic::GetSEL( int state )
{
    return Get( SELECTED, state );
}

inline bool ListLogic::GetDIS( int state )
{
    return Get( DISABLED, state );
}

inline bool ListLogic::GetFOC( int state )
{
    return Get( FOCUSSED, state );
}


};//namespace Useless

#endif//__INCLUDED__USELESS_LIST_LOGIC_H__
