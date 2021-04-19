#include "UselessPch.h"

#include "Useless/GUI/Widgets/ScrolledWidget.h"
#include "Useless/System/TypeInfo.h"
#include "Useless/System/KeyCodes.h"
#include "Useless/Functional/SignalSwitch.h"
#include "Useless/GUI/WidgetEnvironment.h"
#include "Useless/GUI/Workspace.h"

namespace Useless {

    ScrolledWidget::ScrolledWidget(int w, int h): BridgeWidget(w,h), 
    _x(0), _y(0), _x_size(w), _y_size(h), _x_full(w), _y_full(h), _no_drag_sliding(false),
    OnHAdvance(_x.OnUpdate), OnHSize(_x_size.OnUpdate), OnHRange(_x_full.OnUpdate),
    OnVAdvance(_y.OnUpdate), OnVSize(_y_size.OnUpdate), OnVRange(_y_full.OnUpdate)
    {
        for ( int i=0; i!=ActiveWidget::NUM_BUTTONS; ++i )
        {
            OnPress[i].TieUnary( this, &Widget::InputIntercept, (Widget*)0 );
            OnRelease[i].TieUnary( this, &Widget::InputRelease,   (Widget*)0 );
        }

        Tie2Signal( WhileDrag[0], this, &ScrolledWidget::IdleScroll );
        SetSpeed(DEFFAULT_SPEED);

        SignalSwitch<int>::Ptr swPage = new SignalSwitch<int>;
        swPage->TieBinary(Keys::PGUP, this, &ScrolledWidget::ScrollVisible, 0.0, -1.0 );
        swPage->TieBinary(Keys::PGDN, this, &ScrolledWidget::ScrollVisible, 0.0, 1.0 );
        swPage->TieBinary(Keys::LEFT, this, &ScrolledWidget::Scroll, -10, 0 );
        swPage->TieBinary(Keys::RIGHT,this, &ScrolledWidget::Scroll,  10, 0 );
        swPage->TieBinary(Keys::UP  , this, &ScrolledWidget::Scroll,  0, -10 );
        swPage->TieBinary(Keys::DOWN, this, &ScrolledWidget::Scroll,  0,  10 );
        Tie2Signal( OnKeyPress, swPage, &SignalSwitch<int>::Switch );
        Tie2Signal( WhileKey, swPage, &SignalSwitch<int>::Switch );

        EnableOnPathKey(Keys::PGUP);
        EnableOnPathKey(Keys::PGDN);
        EnableOnPathKey(Keys::LEFT);
        EnableOnPathKey(Keys::RIGHT);
        EnableOnPathKey(Keys::UP);
        EnableOnPathKey(Keys::DOWN);
    }

    void ScrolledWidget::Scroll(int dx, int dy)
    {
        if (dx)
        {
            int min_x = 0;
            int max_x = _x_full - _x_size;
            int x = _x + dx;

            _x = (x > max_x)? ( (max_x>0)? max_x : min_x ) : (x < min_x)? min_x : x;
        }

        if (dy)
        {
            int min_y = 0;
            int max_y = _y_full - _y_size;
            int y = _y + dy;

            _y = (y > max_y)? ( (max_y>0)? max_y : min_y ) : (y < min_y)? min_y : y;
        }

        if ( (dx || dy) && BridgeWidget::GetAttached() )
        {
            BridgeWidget::GetAttached()->SetPosition( Pos(-_x, -_y) );
        }
    }

    void ScrolledWidget::ScrollTo(int x, int y)
    {
        Scroll( x-_x, y-_y );
    }

    void ScrolledWidget::ScrollPrecentages(double ax, double ay)
    {
        double w = _x_full - _x_size;
        double h = _y_full - _y_size;
        ScrollTo( w*ax, h*ay);
    }

    void ScrolledWidget::ScrollVisible(double ax, double ay)
    {
        Scroll( _x_size*ax, _y_size*ay);
    }

    void ScrolledWidget::AttachWidget( Widget *widget, int x, int y)
    {
        if (x||y) 
            throw Error("ScrolledWidget::AttachWidget(): Cannot attach widget at other position than (0,0)");

        BridgeWidget::AttachWidget( widget, 0, 0 );
        _x=0;
        _y=0;
        _x_full = widget->GetWidth();
        _y_full = widget->GetHeight();
        _on_resize = Tie2Signal( widget->OnResize, this, &ScrolledWidget::UpdateSize );
        _on_actor_move = Tie2Signal( widget->OnActorMove, this, &ScrolledWidget::KeepActorVisible );
    }

    Widget* ScrolledWidget::DetachWidget()
    {
        _on_resize.Untie();
        _on_actor_move.Untie();
        return BridgeWidget::DetachWidget();
    }

    void ScrolledWidget::Resize(int w, int h)
    {
        _x_size = w;
        _y_size = h;

        if (Widget *child = BridgeWidget::GetAttached())
        {
            int cw = child->GetWidth();
            int ch = child->GetHeight();
            bool stretch_x = cw < w;
            bool stretch_y = ch < h;
            cw = (stretch_x)? w : cw;
            ch = (stretch_y)? h : ch;

            if ( stretch_x || stretch_y )
            {
                child->Resize( cw, ch );
            }
        }

        Update();
        BridgeWidget::Resize(w,h);
    }

    bool ScrolledWidget::Accept( const Request& request)
    {
        if (_no_drag_sliding)
        {
            bool r = BridgeWidget::Accept( request );
            if (!r) r |= ActiveWidget::Accept( request );
            return  r ;
        }

        if ( const InputCursor* cur_req = dynamic_cast<const InputCursor*>(&request) )
        {
            return AcceptInputCursor( *cur_req );
        }
        else if ( dynamic_cast<const InputKeyboard*>(&request) )
        {
            if ( !BridgeWidget::Accept( request ) )
                if ( !ActiveWidget::Accept( request ) )
                    return false;
            return true;
        }
        else
        {
            bool handled = false;
            handled |= ActiveWidget::Accept( request );
            handled |= BridgeWidget::Accept( request );
            return handled;
        }
    }

    bool ScrolledWidget::AcceptInputCursor( const InputCursor &mc )
    {
        Pos  pos = mc.Position();
        Pos dpos = mc.Move();
        int cond = mc.Cond();

        if (!BridgeWidget::GetAttached()) return false;

        bool handled = false;
        handled |= ActiveWidget::AcceptInputCursor( InputCursor(pos, dpos, cond) );

        Widget *ch = BridgeWidget::GetAttached();

        int do_max_x=0, do_max_y=0;
        if ( const WidgetEnvironment *penv = GetEnvironment() )
        {
            const Workspace &work = penv->GetWorkspace();
            Rect screen_area( 0,0, work.GetWidth(), work.GetHeight() );
            Pos  screen_pos = GetAbsolutePos(pos);
            int borderPixels = 5;
            if ( screen_area.GetX1() + borderPixels > screen_pos.x )  {
                do_max_x = -1;
            }
            else if ( screen_area.GetX2() - borderPixels < screen_pos.x )  {
                do_max_x =  1;
            }
            if ( screen_area.GetY1() + borderPixels > screen_pos.y )  {
                do_max_y = -1;
            }
            else if ( screen_area.GetY2() - borderPixels < screen_pos.y )  {
                do_max_y =  1;
            }

        }

        int dist_x = (pos.x<0 ? pos.x: (pos.x<_x_size ? 0: pos.x-_x_size ));
        int dist_y = (pos.y<0 ? pos.y: (pos.y<_y_size ? 0: pos.y-_y_size ));

        if ( dist_x || dist_y || do_max_x || do_max_y )
        {
            int max_dist = 150;
            double absx = ( do_max_x ? max_dist : (dist_x<0 ? -dist_x : dist_x ));
            double absy = ( do_max_y ? max_dist : (dist_y<0 ? -dist_y : dist_y ));
            double abs  = ( absx > absy ? absx : absy );

            _x_vel = ( do_max_x ? do_max_x : (dist_x<0 ? -1 : (dist_x>0 ? 1 : 0 ) ));
            _y_vel = ( do_max_y ? do_max_y : (dist_y<0 ? -1 : (dist_y>0 ? 1 : 0 ) ));

            double f = _speed*abs;

            if (f>0 && f<=100) 
            {
                SetRepeatRate( 1000/f, 0 );
                if (abs==absx) { if (absy) _y_vel *= abs/absy; }
                else           { if (absx) _x_vel *= abs/absx; }
            }
            else if (f>100)
            {
                SetRepeatRate( 20, 0);
                _x_vel *= _speed*absx/50.0f;
                _y_vel *= _speed*absy/50.0f;
            }
        }
        else
        {
            _x_vel = 0;
            _y_vel = 0;
        }


        if ( !GetClientRect(ch).IsInside(pos.x,pos.y) )
        {
            handled |= BridgeWidget::GetAttached()->Accept( InputCursor( pos+Pos( _x, _y), dpos, Widget::OFFSIDE ) );
        }
        else
            handled |= BridgeWidget::GetAttached()->Accept( InputCursor( pos+Pos( _x, _y), dpos, cond ) );

        return handled;
    }


    void ScrolledWidget::IdleScroll()
    {
        if (_no_drag_sliding) return;

        Scroll( _x_vel, _y_vel );

        if ( _x_vel || _y_vel )
        {
            Pos pos = ActiveWidget::GetCursorPos();
            int px = pos.x<0 ? 2 : ( pos.x>_x_size ? _x_size -2 : pos.x );
            int py = pos.y<0 ? 2 : ( pos.y>_y_size ? _y_size -2 : pos.y );
            BridgeWidget::GetAttached()->Accept( InputCursor( Pos( _x+px, _y+py), Pos(), Widget::NORMAL ) );
        }
    }

    void ScrolledWidget::UpdateSize(int w, int h)
    {
        _x_full = BridgeWidget::GetAttached()->GetWidth();
        _y_full = BridgeWidget::GetAttached()->GetHeight();
        Update();
    }

    void ScrolledWidget::Update()
    {
        int min_x = 0;
        int max_x = _x_full - _x_size;
        int x = ((int)_x > max_x)? ( (max_x>0)? max_x : min_x ) : ((int)_x < min_x)? min_x : (int)_x;
        int dx = x-_x;

        int min_y = 0;
        int max_y = _y_full - _y_size;
        int y = ((int)_y > max_y)? ( (max_y>0)? max_y : min_y ) : ((int)_y < min_y)? min_y : (int)_y;
        int dy = y-_y;

        if ( dx || dy ) { Scroll(dx,dy); }
    }

    void ScrolledWidget::KeepActorVisible( const Rect &absolute_actor_bound)
    {
        Widget *child = BridgeWidget::GetAttached();
        assert(child&&"ScrolledWidget::KeepActorVisible(): Nothing attached");
        Rect child_area = GetClientRect(child);
        Rect actor_bound = absolute_actor_bound + child->GetPosition();
        int lt = 0;
        int rt = 0;
        int up = 0;
        int dn = 0;
        bool fit_x = actor_bound.GetW() < child_area.GetW();
        bool fit_y = actor_bound.GetH() < child_area.GetH();
        if ( fit_x )
        {
            if ( actor_bound.GetX1() < child_area.GetX1() )
            {
                lt = child_area.GetX1() - actor_bound.GetX1();
            }
            if ( actor_bound.GetX2() > child_area.GetX2() )
            {
                rt = actor_bound.GetX2() - child_area.GetX2();
            }
        }
        else
        {
            if ( actor_bound.GetX2() < child_area.GetX1() )
            {
                lt = child_area.GetX2() - actor_bound.GetX2();
            }
            if ( actor_bound.GetX1() > child_area.GetX2() )
            {
                rt = actor_bound.GetX1() - child_area.GetX1();
            }
        }
        if ( fit_y )
        {
            if ( actor_bound.GetY1() < child_area.GetY1() )
            {
                up = child_area.GetY1() - actor_bound.GetY1();
            }
            if ( actor_bound.GetY2() > child_area.GetY2() )
            {
                dn = actor_bound.GetY2() - child_area.GetY2();
            }
        }
        else
        {
            if ( actor_bound.GetY2() < child_area.GetY1() )
            {
                up = child_area.GetY2() - actor_bound.GetY2();
            }
            if ( actor_bound.GetY1() > child_area.GetY2() )
            {
                dn = actor_bound.GetY1() - child_area.GetY1();
            }
        }
        int scroll_x = lt ? -lt : rt;
        int scroll_y = up ? -up : dn;
        Scroll( scroll_x, scroll_y);
    }


};//namespace Useless
