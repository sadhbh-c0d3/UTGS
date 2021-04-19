#include "UselessPch.h"

#include "Useless/Functional/Signal.h"
#include "Useless/Functional/SignalSwitch.h"
#include "Useless/Graphic/Device/Screen.h"
#include "Useless/GUI/Display/ScreenMan.h"
#include "Useless/GUI/Display/ScreenShooter.h"
#include "Useless/GUI/GUIMaster.h"
#include "Useless/System/Timer.h"
#include "Useless/Util/Storage/MemBlock.h"


#include <stdio.h>

//#cannot include include "Threads/Lock.h" - signals don't need locking - bad concept
//typedef RW_Mutex Mutex;
//typedef WriteLock Lock;
namespace { struct s_Mutex {}; struct s_Lock { s_Lock(...){} }; };

#include "Useless/System/KeyCodes.h"

#include "Useless/Functional/DelayedExecutor.h"

#include "Useless/ErrorConfig.h"
#if !defined(s_GUI_DEBUG_LEVEL_NONEs_)
#   define   SCREENMAN_LOG(e) SMALL_LOG(e, "ScreenMan")
#   define   SCREENMAN_ERR(e) APP_ERROR(e, "ScreenMan")
#else
#   define   SCREENMAN_LOG(e)
#   define   SCREENMAN_ERR(e)
#endif

//#define s_DEBUG_DIRTYs_

namespace Useless {

USELESS_API bool G_EnableDebugF11 = true;
USELESS_API bool G_EnablePrintScr = true;

//#const int SCREENMANC1 = 131072;32768;
//#const int SCREENMANC2 = 128;
const int SCREENMANC1 = 16384;
const int SCREENMANC2 = 128;

static bool s_show_FPS;
static Pos  s_FPS_position;
static Pos  s_cursor_position;



// We do this to protect copyrights of an application with red-green cross.
// The magic password that shall be given as "protection" attribute of <gui> tag is: "scrnocross"
// See also "Useless/XML/Resources/CreateGUI.cpp" lines 80-84.
static std::string s_CopyrightProtectionString1;
static std::string s_CopyrightProtectionString2;

void _SetCopyrightProtectionString( const std::string &screenOption1, std::string screenOption2 )
{
    if ( screenOption1 == screenOption2 )
    {
        s_CopyrightProtectionString1 = screenOption1;
    }
}

void USELESS_API SetCopyrightProtectionString( const std::string &screenOption1, std::string screenOption2 )
{
    if ( screenOption1 == screenOption2 )
    {
        s_CopyrightProtectionString2 = screenOption2;
    }
    else
    {
        s_CopyrightProtectionString2 += 's'; s_CopyrightProtectionString2 += 'c'; s_CopyrightProtectionString2 += 'r'; s_CopyrightProtectionString2 += 'i';
        s_CopyrightProtectionString2 += 'n'; s_CopyrightProtectionString2 += 'v'; s_CopyrightProtectionString2 += 'a'; s_CopyrightProtectionString2 += 'l';
        s_CopyrightProtectionString2 += 'i'; s_CopyrightProtectionString2 += 'd';
    }
}

std::string USELESS_API GetCopyrightProtectionString()
{
    return s_CopyrightProtectionString1;
}


bool g_PaintWidgetBoundaries = false;

class DBGScreenMan
{
public:
    void OnActivate()    {  SCREENMAN_LOG(Error(" OnActivate {") ); }
    void OnActivateEnd() {  SCREENMAN_LOG(Error(" } OnActivate") ); }
    void OnPaint()       {  SCREENMAN_LOG(Error(" OnPaint {"   ) ); }
    void OnPaintEnd()    {  SCREENMAN_LOG(Error(" } OnPaint"   ) ); }
    void OnCursor( Pos p ) { s_cursor_position=p; }
    void TurnFPS() {  s_show_FPS=!s_show_FPS; s_FPS_position=s_cursor_position; }
    void TurnPaintBoundaries()
    {
        g_PaintWidgetBoundaries = !g_PaintWidgetBoundaries;
    }

    void NextDebugFunction( Workspace *space )
    {
        int state = 0;
        state |= s_show_FPS << 0;
        state |= g_PaintWidgetBoundaries << 1;

        switch( state )
        {
        case 0:
            TurnFPS();
            break;
        case 1:
            TurnPaintBoundaries();
            break;
        case 3:
            TurnFPS();
            TurnPaintBoundaries();
            break;
        }
        space->SetDirty( Rect() );
    }
};

/* Just create ScreenMan (no signals) */
ScreenMan::ScreenMan(Screen &screen, GUIMaster &gui_master)
{
    _screen = &screen;
    _gui_master = &gui_master;

    _mouse_cursor=0;
    _current_frame=0;
    _cursor_visible=1;
    _wheel=0;
    _wheel_divider=60;
    _cursor=Pos( _screen->GetWidth()/2, _screen->GetHeight()/2 );
    screen.SetCursorPos( _cursor.x, _cursor.y);
    _cursor_doubler=_cursor;
    _mouse_buttons_status=false;
 
    s_CopyrightProtectionString1 += 's'; s_CopyrightProtectionString1 += 'c'; s_CopyrightProtectionString1 += 'r'; s_CopyrightProtectionString1 += 'n';
    s_CopyrightProtectionString1 += 'o'; s_CopyrightProtectionString1 += 'c'; s_CopyrightProtectionString1 += 'r'; s_CopyrightProtectionString1 += 'o';
    s_CopyrightProtectionString1 += 's'; s_CopyrightProtectionString1 += 's';

    SetCopyrightProtectionString( "s1", "s2" );
}

/*! Setup screen manager includes:
 *  + cursor and wheel defaults
 *  + plug to environmental signals
 *      + Window::OnIdle
 *      + Window::OnActivate
 *      + Window::OnPaint
 *      + Window::OnMouseMove/Btn/Wheel
 *      + Window::OnChar
 *      + Window::OnKeyPress/Release
 *
 */
void ScreenMan::Attach()
{
    if ( IsAttached() ) { Detach(); }

    Workspace& workspace = _gui_master->GetWorkspace(); assert(&workspace);
    
    _event_ties.Insert( Tie2Signal( Application::OnIdle, this, &ScreenMan::Advance ) );
    //OnTimer::Instance().Schedule( this, &ScreenMan::Advance, 0,0, 0,-1);

#ifndef NDEBUG
    _event_ties.Insert( Tie2Signal( _screen->OnActivate, (DBGScreenMan*)0, &DBGScreenMan::OnActivate ) );
    _event_ties.Insert( Tie2Signal(    _screen->OnPaint, (DBGScreenMan*)0, &DBGScreenMan::OnPaint ) );
#endif

    _event_ties.Insert( _screen->OnActivate.TieUnary( &workspace,      &Workspace::SetDirty, Rect() ) );
    _event_ties.Insert( _screen->OnPaint.TieUnary   ( &workspace,      &Workspace::SetDirty, Rect() ) );
    //_event_ties.Insert( _screen->OnPaint.TieVoid    ( _screen,         &Screen::Swap ) );
    _event_ties.Insert( _screen->OnPaint.TieBinary  ( this,            &ScreenMan::Advance, 0,0 ) );
    _event_ties.Insert( _screen->OnPaint.TieBinary  ( this,            &ScreenMan::Advance, 0,0 ) );
    _event_ties.Insert( _screen->OnPaint.TieUnary   ( &workspace,      &Workspace::SetDirty, Rect() ) );

#ifndef NDEBUG
    _event_ties.Insert( Tie2Signal( _screen->OnActivate, (DBGScreenMan*)0, &DBGScreenMan::OnActivateEnd ) );
    _event_ties.Insert( Tie2Signal(    _screen->OnPaint, (DBGScreenMan*)0, &DBGScreenMan::OnPaintEnd ) );
#endif
    _event_ties.Insert( Tie2Signal( _screen->OnIdle, &workspace, &Workspace::AcceptIdleCalls ) );
    _event_ties.Insert( Tie2Signal( _screen->OnResize, &workspace, &Workspace::Resize ));

    
    _event_ties.Insert( Tie2Signal( _screen->OnMouseMove,  this, &ScreenMan::UpdateCursor) );
    _event_ties.Insert( Tie2Signal( _screen->OnMouseWheel, this, &ScreenMan::UpdateWheel) );
    _event_ties.Insert( Tie2Signal( _screen->OnMouseBtn,   this, &ScreenMan::AcceptButtons) );

    _event_ties.Insert( Tie2Signal( _screen->OnMouseBtn, &workspace, &Workspace::AcceptInputButton) );
    _event_ties.Insert( Tie2Signal( _screen->OnDoubleClick, &workspace, &Workspace::AcceptDoubleClick) );
    _event_ties.Insert( Tie2Signal( _screen->OnChar,     &workspace, &Workspace::AcceptInputChar) );
    _event_ties.Insert(   _screen->OnKeyPress.TieBinary( &workspace, &Workspace::AcceptInputKey, _screen->OnKeyPress.GetVar1(), true) );
    _event_ties.Insert( _screen->OnKeyRelease.TieBinary( &workspace, &Workspace::AcceptInputKey, _screen->OnKeyRelease.GetVar1(), false) );

    _event_ties.Insert( Tie2Signal( _screen->OnMouseMove,(DBGScreenMan*)0, &DBGScreenMan::OnCursor ) );

    /* Screen Shots and FPS W/A
    --------------------------------*/
    SPointer<ScreenShooter> sp_screenshooter( new ScreenShooter(*_screen));
    SignalSwitch<int>::Ptr sp_ssswitch( new SignalSwitch<int>() );
    
    if ( G_EnablePrintScr )
    {
        sp_ssswitch->TieVoid( Keys::PRINT_SCR, sp_screenshooter, &ScreenShooter::Shot );
    }
    
    if ( G_EnableDebugF11 )
    {
        sp_ssswitch->TieUnary( Keys::F11, (DBGScreenMan*)0, &DBGScreenMan::NextDebugFunction, &workspace );
    }
    
    _event_ties.Insert( Tie2Signal( _screen->OnKeyRelease, sp_ssswitch, &SignalSwitch<int>::Switch ) );

    UpdateCursor( _screen->GetCursorPos(), Pos() );
    workspace.SetDirty( Rect() );
}

/* Detach from screen */
void ScreenMan::Detach()
{
    for ( int i=0; i!=_event_ties.Size(); ++i )
    {
        _event_ties[i].Untie();
    }
    _event_ties.Clear();
}

/* Detach from screen */
ScreenMan::~ScreenMan()
{
    Detach();
}

/* Schedule repainting whole screen during next Advance() */
void ScreenMan::SheduleCompleteRepaint()
{
    //GetRects(_current_frame).push_back( Rect(0, 0, _screen->GetWidth(), _screen->GetHeight() ) );
    GetRects(_current_frame) |= Rect(0, 0, _screen->GetWidth(), _screen->GetHeight() );
}

/*! Get sample of image data placed in given area of screen */
ImageBuffer ScreenMan::GetImageRect( const Rect &area)
{
    return ImageBuffer( _screen->GetBackBuffer(), area );
}

/*! Get clip-region's list-of-rectangles */
RectList ScreenMan::GetClipRects() const
{
    return _screen->GetClipper();
}

/*! Just setup clip-region*/
void ScreenMan::SetClipRects(const RectList& rect_list)
{
    if ( !rect_list.empty() )
        _screen->SetClipper( rect_list );
}

/*! Setup clip-region intersecting current one with given rectangle */
bool ScreenMan::Intersect( const Rect &crop )
{
    if ( !crop ) { return false; }

    RectList region = _screen->GetClipper( crop );

    if ( region.empty() )  { return false; }

    _screen->SetClipper( region );
    return true;
}

static s_Mutex _mutex;

/*! Notice dirty area, which will be repainted during next Advance() */
void ScreenMan::AcceptDirtyRect(const Rect &r)
{
    s_Lock guard(_mutex);
    GetRects(_current_frame)   |= r;
    GetRects(_current_frame+1) |= r;
}


/*! Advance to next frame by rendering current one */
void ScreenMan::Advance(int t, int dt)
{
    assert( IsAttached() );

    // Do this if we protect application with red-green cross
    if (( ((_current_frame - 1) % SCREENMANC1) == (SCREENMANC1-SCREENMANC2) || ((_current_frame - 1) % SCREENMANC1) == 0 ) && ( s_CopyrightProtectionString1 != s_CopyrightProtectionString2 ))
    {
        AcceptDirtyRect( Rect(0,0,_screen->GetWidth(),_screen->GetHeight() ));
    }

    ClipList &dirty = GetRects( _current_frame );
    bool anything_dirty = false;
    RectList optimized;

    {   s_Lock guard(_mutex);
        if ( !_screen->IsValid() ) 
        { return; }

        if ( !dirty.empty() )
        {
            dirty.copy(optimized);
            anything_dirty = true;

#           ifdef s_DEBUG_DIRTYs_
            _screen->SetClipper( Rect(0,0,_screen->GetWidth(),_screen->GetHeight()) );
            _screen->Clear();
#           endif

            _screen->SetClipper( optimized );
          ++_current_frame;
             dirty.clear();
        }
    }

    if ( anything_dirty )
    {
        _gui_master->GetWorkspace().Repaint();
        if (_cursor_visible)
        {   DrawCursor();  }

/*-----------------------------------------------------*/
        if ( s_show_FPS )
        {
            _screen->PrintFPS( s_FPS_position.x, s_FPS_position.y, dt );
            MemBlock fr(40);
            sprintf( fr.GetPtrC(), "Frame: %i", _current_frame );
            _screen->PrintText( s_FPS_position.x+100, s_FPS_position.y, fr.GetPtrC() );
            _gui_master->GetWorkspace().SetDirty( s_FPS_position+Rect(0,0,200,14) );
        }
/*-----------------------------------------------------*/
    
        // Do this if we protect application with red-green cross
        if ( (_current_frame % SCREENMANC1) >= (SCREENMANC1-SCREENMANC2) && ( s_CopyrightProtectionString1 != s_CopyrightProtectionString2 ))
        {
            _screen->SetClipper( optimized );
            Painter paint( *_screen );
            paint.PaintLine( Pos(16,16), Pos(_screen->GetWidth()-16,16), 0x80FF0000, 5 );
            paint.PaintLine( Pos(16,16), Pos(16,_screen->GetHeight()-16), 0x80FF0000, 5 );
            paint.PaintLine( Pos(16,16), Pos(_screen->GetWidth()-16,_screen->GetHeight()-16), 0x40FF0000, 3 );
            paint.PaintLine( Pos(_screen->GetWidth()-16,16), Pos(16,_screen->GetHeight()-16), 0x4000FF00, 3 );
            paint.PaintLine( Pos(_screen->GetWidth()-16,16), Pos(_screen->GetWidth()-16,_screen->GetHeight()-16), 0x8000FF00, 5 );
            paint.PaintLine( Pos(16,_screen->GetHeight()-16), Pos(_screen->GetWidth()-16,_screen->GetHeight()-16), 0x8000FF00, 5 );
        }
        _screen->Swap();
#       ifdef s_DEBUG_DIRTYs_
        Sleep(200);
#       endif
    }
}


void  ScreenMan::AcceptButtons(unsigned long status, unsigned long mask)
{
    if ( (!_mouse_buttons_status) && (mask & status) )
    {
        _mouse_buttons_status = true;
        _screen->InterceptCursor();
    }
    else if ( _mouse_buttons_status && (mask & (~status)) )
    {
        _mouse_buttons_status = false;
        _screen->ReleaseCursor();
    }
}

/* Called every OnMouseMove event */
void ScreenMan::UpdateCursor(Pos p, Pos dp)
{
    Point<double> zoom = _screen->GetZoomFactors();
    p.x = p.x/zoom.x; dp.x = dp.x/zoom.x;
    p.y = p.y/zoom.y; dp.y = dp.y/zoom.y;

    if (!!_mouse_cursor) /* Use MouseCursor for cursor display */
    {
        _gui_master->GetWorkspace().SetDirty( _mouse_cursor->GetRect()+_cursor );
       
       /* This part works only with relative positioning (with DirectInput enabled)*/
       /* Point<double> ddp = dp;
        ddp = ddp*_mouse_cursor->_speed + (ddp*ddp*ddp)*_mouse_cursor->_acceleration;
        dp = (ddp+0.4);

        (_cursor += dp).Enclose( _mouse_cursor->_paint_limit );

        if ( _mouse_cursor->_limitFlags & 2 )
        {
            if (_mouse_cursor->_move_limit)
            {
                Pos cdp = _cursor_doubler;
                (_cursor_doubler += dp).Enclose( _mouse_cursor->_move_limit );
                dp = _cursor_doubler - cdp;
            }
        }
        */

        /* Following code is based on absolute positioning (will work with tablets)*/ 
        _cursor = p;
        if ( _mouse_cursor->_limitFlags & 1 )
        {
            p.Enclose( _mouse_cursor->_paint_limit );
        }
        if (_cursor != p) 
        { 
            _screen->SetCursorPos( p.x, p.y); _cursor = p; 
        }
        _gui_master->GetWorkspace().SetDirty( _mouse_cursor->GetRect()+_cursor );
    }
    else /* No MouseCursor set, use default cross-hair then */
    {
        _gui_master->GetWorkspace().SetDirty( Rect(_cursor.x-16,_cursor.y-16,32,32) );
        _gui_master->GetWorkspace().SetDirty( Rect( p.x-16, p.y-16,32,32) );
        _cursor = p;
        _cursor_doubler = _cursor;
    }
    _gui_master->GetWorkspace().AcceptInputCursor(_cursor, dp);
}

/* Draw cursor on screen */
void ScreenMan::DrawCursor()
{
    if (!!_mouse_cursor) /* Let MouseCursor Repaint itself */
    {
        _mouse_cursor->Paint( Pos(_cursor.x, _cursor.y), *_screen );
    }
    else /* Draw cross-hair cursor */
    {
#       ifndef NDEBUG
        if ( g_PaintWidgetBoundaries )
        {
            _screen->Clear( 0xFFFFFF, Useless::Rect( _cursor.x-1, _cursor.y-16, 1, 8) );
            _screen->Clear( 0xFFFFFF, Useless::Rect( _cursor.x-16, _cursor.y-1, 8, 1) );
            _screen->Clear( 0xFFFFFF, Useless::Rect( _cursor.x-1, _cursor.y+8, 1, 8) );
            _screen->Clear( 0xFFFFFF, Useless::Rect( _cursor.x+8, _cursor.y-1, 8, 1) );
            _screen->Clear( 0xFFFFFF, Useless::Rect( _cursor.x-1, _cursor.y-1, 2, 2) );
        }
#       endif
    }
}

/*! Set MouseCursor shape and limits */
void ScreenMan::SetCursor(const MouseCursor &mc)
{
    Rect dirty;
    if(!! _mouse_cursor) { dirty |=  _mouse_cursor->GetRect()+_cursor; }
    dirty |= mc.GetRect()+_cursor;

    if(!! dirty) _gui_master->GetWorkspace().SetDirty(dirty);
    _mouse_cursor = mc.Copy();
}

/*! Releases MouseCursor */
void ScreenMan::RemoveCursor()
{
    _mouse_cursor = NULL;
}

/*! Sets default MouseCursor */
void ScreenMan::SetDefaultCursor()
{
    if (!_default_mouse_cursor) 
    { SCREENMAN_ERR(Error("No default cursor were created."
                          "Use CreateDefaultCursor() to make one")); }
    SetCursor( *_default_mouse_cursor );
}

/*! Creates default MouseCursor. User must call SetDefaultCursor() himself. */
void ScreenMan::CreateDefaultCursor( const RefCursor &image )
{
    _default_mouse_cursor = image.get().Copy(); //new ImageCursor( image, Pos(0,0) );
    _default_mouse_cursor->_limitFlags = 0;
    _default_mouse_cursor->_move_limit  = Rect( 0, 0, _screen->GetWidth(), _screen->GetHeight() );
    _default_mouse_cursor->_paint_limit = Rect( 0, 0, _screen->GetWidth(), _screen->GetHeight() );
    _default_mouse_cursor->_speed = 0.75f;
    _default_mouse_cursor->_acceleration = 100.0f;
}

/* Called every OnMouseWheel event */
void ScreenMan::UpdateWheel(int z, int dz)
{
     dz = dz/_wheel_divider + (dz*dz*dz)/(_wheel_divider*_wheel_divider*_wheel_divider);
     dz = -dz;
    _wheel += dz;
    _gui_master->GetWorkspace().AcceptInputWheel( _wheel, dz );
}

/*! Set wheel sensitivity. The bigger number, occures in more precise movement */
void ScreenMan::SetWheelDivider(int divider)
{
    _wheel_divider = divider;
}

OGraphics& ScreenMan::GetPlane( const Rect &rect )
{
    return *_screen;
}


};// namespace Useless
