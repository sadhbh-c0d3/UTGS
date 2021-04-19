#include "UselessPch.h"

#include "Useless/GUI/Widgets/AdvScrollbarWidget.h"
#include "Useless/Functional/EquationHolder.h"

namespace Useless {

AdvScrollbarWidget::AdvScrollbarWidget( const ScrollbarStyle &sb_style,
                                        const SkinMan &skins_button_up,
                                        const SkinMan &skins_button_down,
                                                  int  button_scroll_speed
                                       )
{
    _p_scrollbar = new ScrollbarWidget(sb_style);
    _p_button_up = new ButtonWidget(skins_button_up);
    _p_button_down = new ButtonWidget(skins_button_down);
    _button_scroll_speed = button_scroll_speed;
    Init();
}

AdvScrollbarWidget::AdvScrollbarWidget( const AdvScrollbarStyle &style )
{
    _p_scrollbar = new ScrollbarWidget(style);
    _p_button_up = new ButtonWidget(style.skins_button_up);
    _p_button_down = new ButtonWidget(style.skins_button_down);
    _button_scroll_speed = style.button_scroll_speed;
    Init();
}

void AdvScrollbarWidget::Init()
{
    _p_button_up->OnPress[0].TieUnary( _p_scrollbar, &ScrollbarWidget::Scroll,
                       /* Make this event dependant on -_button_scroll_speed
                       -----------------------------------------------------*/
                       EqPackSign<int>( PackVariable2Holder(_button_scroll_speed) )
                       );

    _p_button_down->OnPress[0].TieUnary( _p_scrollbar, &ScrollbarWidget::Scroll,
                       /* Make this event dependant on +_button_scroll_speed
                       -----------------------------------------------------*/
                       PackVariable2Holder(_button_scroll_speed)
                       );

    _p_button_up->WhileHold[0].TieUnary( _p_scrollbar, &ScrollbarWidget::Scroll,
                       /* Make this event dependant on -_button_scroll_speed
                       -----------------------------------------------------*/
                       EqPackSign<int>( PackVariable2Holder(_button_scroll_speed) )
                       );

    _p_button_down->WhileHold[0].TieUnary( _p_scrollbar, &ScrollbarWidget::Scroll,
                       /* Make this event dependant on +_button_scroll_speed
                       -----------------------------------------------------*/
                       PackVariable2Holder(_button_scroll_speed)
                       );

    if ( _p_scrollbar->IsHorizontal() )
    {
        int sb_size = _p_scrollbar->GetWidth();
        int up_size = _p_button_up->GetWidth();
        int dn_size = _p_button_down->GetWidth();

        int btn1_y = ( _p_scrollbar->GetHeight() - _p_button_up->GetHeight() )/2;
        int btn2_y = ( _p_scrollbar->GetHeight() - _p_button_down->GetHeight() )/2;

        HubWidget::Insert( _p_scrollbar );
        HubWidget::Insert( _p_button_up  , sb_size, btn1_y );
        HubWidget::Insert( _p_button_down, sb_size + up_size, btn2_y );
        HubWidget::Resize( sb_size + up_size + dn_size, _p_scrollbar->GetHeight() );
    }
    else
    {
        int sb_size = _p_scrollbar->GetHeight();
        int up_size = _p_button_up->GetHeight();
        int dn_size = _p_button_down->GetHeight();

        int btn1_x = ( _p_scrollbar->GetWidth() - _p_button_up->GetWidth() )/2;
        int btn2_x = ( _p_scrollbar->GetWidth() - _p_button_down->GetWidth() )/2;

        HubWidget::Insert( _p_scrollbar );
        HubWidget::Insert( _p_button_up  , btn1_x, sb_size );
        HubWidget::Insert( _p_button_down, btn2_x, sb_size + up_size );
        HubWidget::Resize( _p_scrollbar->GetWidth(), sb_size + up_size + dn_size );
    }
}

AdvScrollbarWidget::~AdvScrollbarWidget()
{
}


void AdvScrollbarWidget::Resize(int w, int h)
{
    if ( Scroller().IsHorizontal() )
    {
        int min_sb  = _p_scrollbar->GetPagerSizeLimit();
        int sb_size = _p_scrollbar->GetWidth();
        int up_size = _p_button_up->GetWidth();
        int dn_size = _p_button_down->GetWidth();
        int buttons = up_size + dn_size;

        int btn1_y = ( _p_scrollbar->GetHeight() - _p_button_up->GetHeight() )/2;
        int btn2_y = ( _p_scrollbar->GetHeight() - _p_button_down->GetHeight() )/2;

        //assert( w >= buttons );

        if ( w - buttons >= 0 /*=min_sb*/ )
        {
            sb_size = w - buttons;

            _p_scrollbar->Resize( sb_size, _p_scrollbar->GetHeight() );
            _p_button_up->SetPosition( Pos( sb_size, btn1_y) );
            _p_button_down->SetPosition( Pos( sb_size+up_size, btn2_y) );

            _p_scrollbar->Show();
            h = _p_scrollbar->GetHeight();
        }
        else
        {
            _p_button_up->SetPosition( Pos( 0, 0) );
            _p_button_down->SetPosition( Pos( w/2 /*up_size*/, 0) );
            _p_scrollbar->Hide();
        }
    }
    else
    {
        int min_sb  = _p_scrollbar->GetPagerSizeLimit();
        int sb_size = _p_scrollbar->GetHeight();
        int up_size = _p_button_up->GetHeight();
        int dn_size = _p_button_down->GetHeight();
        int buttons = up_size + dn_size;

        int btn1_x = ( _p_scrollbar->GetWidth() - _p_button_up->GetWidth() )/2;
        int btn2_x = ( _p_scrollbar->GetWidth() - _p_button_down->GetWidth() )/2;

        //assert( h >= buttons );

        if ( h - buttons >= 0 /*=min_sb*/ )
        {
            sb_size = h - buttons;

            _p_scrollbar->Resize( _p_scrollbar->GetWidth(), sb_size );
            _p_button_up->SetPosition( Pos( btn1_x, sb_size) );
            _p_button_down->SetPosition( Pos( btn2_x, sb_size+up_size) );

            _p_scrollbar->Show();
            w = _p_scrollbar->GetWidth();
        }
        else
        {
            _p_button_up->SetPosition( Pos( 0, 0) );
            _p_button_down->SetPosition( Pos( 0, h/2 /*up_size*/) );
            _p_scrollbar->Hide();
        }
    }

    HubWidget::Resize(w,h);
}

void AdvScrollbarWidget::SetRepeatRate( long int period, long int delay )
{
    _p_button_up->SetRepeatRate( period, delay );
    _p_button_down->SetRepeatRate( period, delay );
}


};//namespace Useless
