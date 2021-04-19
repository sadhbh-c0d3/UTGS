#include "UselessPch.h"

#include "Useless/GUI/Widgets/PopupWidget.h"
#include "Useless/GUI/Widgets/NullWidget.h"
#include "Useless/Functional/SignalSwitch.h"
#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Misc/Layout.h"

namespace Useless {

//Dimensions are treatened as follows:
// w - width of whole PopupWidget
// h - height of content frame
// titlebar height is obtained from title skin or from style
// buttontray height is obtained from tray skin
//

PopupWidget::PopupWidget( const PopupStyle &style, const Text &title )
    :Popup( style.w, style.h + style.button_tray_skin.GetHeight(),
            style.titlebar_style, title, style.title_height, style.modal )
{
    _p_frame = new FrameWidget( 0, 0, style.frame_skin );
    
    FrameStyle fs( 0, 0, style.button_tray_skin, 0,0,0,0 );
    _p_button_tray = new FrameWidget( fs );

    // Button tray is supposed to be oriented horizontally
    assert( style.button_tray_skin.GetDirMask().x != 0 && "ButtonTray" );
    int tray_height = style.button_tray_skin.GetHeight();

    _p_body_box = new VLayout();
    (*_p_body_box) << _p_frame;
    (*_p_body_box) << _p_button_tray,  MinSize_(tray_height), MaxSize_(tray_height);

    SetContent( (*_p_body_box).Release() );
    Hide();

    (*_p_body_box).Fill();

    // obsolete part
    OnResize.TieVoid( this, &PopupWidget::UpdateButtonTray );
    _weight_sum       = 0;
    _fixed_sum        = 0;
    _button_tray_size = _p_button_tray->GetHeight();
    _tray_alignment   = V_TOP;
}

PopupWidget::~PopupWidget()
{
    delete _p_body_box;
}

void PopupWidget::AddButton( ActiveWidget *p_button, int weight, bool closes_popup )
{
    AddBlock( p_button, weight );

    if (closes_popup) 
    {
        Tie2Signal( p_button->OnClick[0], this, &Widget::Hide ); 
        SetDefaultFocus( p_button );
    }
}

//void  PopupWidget::AddGap( int weight )
//{
//    _button_box << Gap_(weight);
//    _button_box.Fill();
//}
//
//void  PopupWidget::AddBlock( Widget *p_widget, int weight )
//{
//    bool fixed_size = weight<0;
//    weight = (weight<0)? - weight:weight;
//
//    _button_box << p_widget;
//
//    if ( fixed_size )
//    {
//        _button_box << MinSize_(weight), MaxSize_(weight);
//    }
//    else
//    {
//        _button_box << Weight_(weight);
//    }
//    _button_box.Fill();
//}

void  PopupWidget::AddGap( int weight )
{
    AddBlock( NullWidget::InstancePtr(), weight );
}

void  PopupWidget::AddBlock( Widget *p_widget, int weight )
{
    ButtonCell cell; cell.p_button=p_widget; cell.weight=weight;

    bool fixed_size = weight<0;
    weight = (weight<0)? - weight:weight;

    assert( weight>0       && "PopupWidget::AddButton(): Weight must be nonzero value" );
    assert( weight<(2<<15) && "PopupWidget::AddButton(): Weight too big" );

    if ( p_widget!=NullWidget::InstancePtr() )
    {
        _p_button_tray->Insert( p_widget );
    }

    if ( fixed_size )
        _fixed_sum +=weight;
    else
        _weight_sum+=weight;

    _button_cells.Insert(cell);
    UpdateButtonTray();
}

void PopupWidget::UpdateButtonTray()
{
    int icell, num_buttons = _button_cells.Size(), x=0;
    int width = GetWidth();
    int wg_width = width - _fixed_sum;
    for ( icell=0; icell!=num_buttons; ++icell )
    {
        ButtonCell cell = _button_cells[icell];
        // cell has fixed size (weight<0) or weightened size (otherwise)
        int cell_width = (cell.weight<0)? -cell.weight : ( cell.weight * wg_width )/_weight_sum;
        // widget height is not the subject to change
        int button_height = cell.p_button->GetHeight();
        // resize widget width to cell width
        cell.p_button->Resize( cell_width, button_height );
        // get new widget size after resizing
        int button_width = cell.p_button->GetWidth();
        // widget may fit whole cell, but center it in case if it don't
        Pos p;
        p.x = x + cell_width/2 - button_width/2;
        
        if ( _tray_alignment==V_CENTER )
        {
            p.y = (_button_tray_size - button_height)/2;
        }
        else if ( _tray_alignment==V_BOTTOM )
        {
            p.y = _button_tray_size - button_height;
        }

        cell.p_button->SetPosition(p);
        // go to the begining of the next cell
        x += cell_width;
    }
}

};//namespace Useless
