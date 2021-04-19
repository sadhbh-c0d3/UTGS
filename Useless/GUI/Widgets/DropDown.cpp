#include "UselessPch.h"

#include "Useless/GUI/Widgets/DropDown.h"
#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Widgets/EnumWidget.h"
#include "Useless/GUI/Logic/MultiItemLogic.h"
#include "Useless/Functional/SignalSwitch.h"
#include "Useless/Graphic/Device/Screen.h"

#include "Useless/GUI/Misc/DataTypes.h" // DT_Text in CTor used (compatibility W/A)

namespace Useless {

DropDown::DropDown( const CaptionStyle &style, bool menu_above )
    : ContentWidget( new DT_Text("..."
        , Place( style.alignment ))
        , style.skins
        , style.fonts
        , style.distance )
    , _p_list(0)
    , _placement( Place(style.alignment) )
    , _menu_above( menu_above )
    , _selected( -1 )
{}

DropDown::~DropDown()
{}

void DropDown::Tie2Signals( IWEnum *p_list )
{
    _p_list = p_list;
    Tie2Signal( OnClick[0], this, &DropDown::Drop );
    Tie2Signal( OnCatch[0], this, &DropDown::DragDrop );
    Tie2Signal( _p_list->Atom().OnVisible, this, &DropDown::MenuVisible );
    Enumerator().OnChange.TieUnary
        ( this,
          &DropDown::SetContent,
          PackFuncPtr2Holder
          ( PackFunctorR
            ( VoidRCall
              ( this,
                &DropDown::GetSelectedData,
                DataPointer(0) ),
              DataPointer(0) )
            )
    );
    Enumerator().OnConfirm.TieVoid( _p_list->AtomPtr(), &Widget::Hide );
    Tie2Signal( OnDrop[0], _p_list->AtomPtr(), &Widget::Hide );

    _p_list->Atom().Hide();

    Tie2Signal( OnResize, this, &DropDown::SetMenuWidth );
    Tie2Signal( OnClick[0], &Enumerator(), &Widget::FocusIntercept ); 
}

void DropDown::SetEnvironment( const WidgetEnvironment *enviro)
{
    if (!!_p_list)
    {
        if (enviro)
        {
            Workspace& work = enviro->GetWorkspace();
            if ( work.InWhichLayer(_p_list->AtomPtr())==Workspace::NO_LAYER )
            {
                work.AddMenu(_p_list->AtomPtr());
            }
        }

        else if ( GetEnvironment() )
        {
            Workspace& work = GetEnvironment()->GetWorkspace();

            if ( work.InWhichLayer(_p_list->AtomPtr())!=Workspace::NO_LAYER )
            {
                work.Remove( _p_list->AtomPtr() );
            }
        }
    }

    Widget::SetEnvironment( enviro );
}

void DropDown::Drop()
{
    Workspace& work = GetEnvironment()->GetWorkspace();
    
    int button_height = GetHeight();

    Pos pos = GetAbsolutePos( Pos( 0, button_height ) );
    
    int space_below = work.GetHeight() - pos.y - 2;
    int space_above = pos.y - button_height - 2;
    int minimal_size = button_height * 5;

    /*-- show menu, that selected item covers button */
    static bool amigaMenu = false;
    if ( amigaMenu )
    {
        SetMenuHeight( minimal_size );
        MultiItemLogic &mel = dynamic_cast< MultiItemLogic &>( Enumerator() );
        Rect ib = mel.GetItemBound( Enumerator().GetSelected() );
        Enumerator().OnActorMove.Send_1( ib );
        Widget *p = &mel;
        while ( p != _p_list->AtomPtr() )
        {
            ib += p->GetPosition();
            p = p->GetParent();
        }
        Pos offset(0, ib.GetY1() + (ib.GetH() + button_height)/2);
        work.ShowAtPosition( pos - offset, _p_list->AtomPtr() );
        return;
    }

    if ( (space_below > minimal_size && !_menu_above) || space_above <= minimal_size )
    {
        SetMenuHeight( space_below );
        work.ShowAtPosition( pos, _p_list->AtomPtr() );
    }
    else
    {
        SetMenuHeight( space_above );
        int new_height = _p_list->Atom().GetHeight();
        work.ShowAtPosition( pos-Pos(0,new_height+button_height), _p_list->AtomPtr() );
    }
}

void DropDown::DragDrop()
{
    Drop();
    static Pos where(0,0);
    static bool jumpToSelected = false;
    int previous = Enumerator().GetSelected();
    if ( jumpToSelected )
    {
        MultiItemLogic &mel = dynamic_cast< MultiItemLogic &>( Enumerator() );
        Rect ib = mel.GetItemBound( previous );
        Enumerator().OnActorMove.Send_1( ib );
        Widget *p = &mel;
        while ( p != _p_list->AtomPtr() )
        {
            ib += p->GetPosition();
            p = p->GetParent();
        }
        Enumerator().Accept( InputCursor( ib.GetPos(), Pos(0,0), NORMAL ));
        Enumerator().Accept( InputButton( 1, 1, NORMAL ));
        // change screen cursor position
        Pos scrPos = GetEnvironment()->GetScreen().GetCursorPos();
        Pos absPos = GetAbsolutePos( ib.GetPos() );
        GetEnvironment()->GetScreen().SetCursorPos( scrPos.x, absPos.y );
    }
    else
    {
        Enumerator().Accept( InputCursor( where, Pos(0,0), NORMAL ));
        Enumerator().Accept( InputButton( 1, 1, NORMAL ));
    }
}

void DropDown::MenuVisible( bool visible )
{
    if ( visible )
    {
        _selected = Enumerator().GetSelected();
        _p_list->Atom().InputIntercept();
    }
    else
    {
    }
}

void DropDown::SetMenuWidth( int w )
{
    _p_list->Atom().Resize( w, _p_list->Atom().GetHeight() );
}


};//namespace Useless

