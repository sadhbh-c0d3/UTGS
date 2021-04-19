#include "UselessPch.h"

#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Display/WidgetPainter.h"
#include "Useless/GUI/WidgetEnvironment.h"

#include "Useless/ErrorConfig.h"
#define   WIDGET_NOTICE1(e)  SMALL_LOG(e, "HubWidget")
#define   WIDGET_NOTICE2(e) MEDIUM_LOG(e, "HubWidget")
#define   WIDGET_NOTICE3(e)   HUGE_LOG(e, "HubWidget")
#define     WIDGET_ERROR(e)  APP_ERROR(e, "HubWidget")
#define   WIDGET_WARNING(e)  APP_ERROR(e, "HubWidget")

#include "Useless/GUI/DebugGUI.h"

namespace Useless {

HubWidget::HubWidget(int w, int h)
{
    SetDimensions( w, h );
}

HubWidget::~HubWidget()
{
    try { ReleaseAll(); }
    catch(...) { assert(0&&"~HubWidget(): ReleaseAll Failed !!!"); }
}

bool HubWidget::Insert(Widget *widget, int x, int y, int z)
{
    assert(widget&&"HubWidget::Insert(): NULL pointer passed");

    ZWidgetList::ConstIterator iter;
    if ( _z_indices.FindID(widget, &iter) ) { _z_indices.RemoveAt(iter); }

    widget->SetPosition( Pos(x,y) );
    _z_indices.Insert( z, widget );
    DBG_Insert;

    widget->SetParent( this );
    if ( !!GetEnvironment() )
    {
        widget->SetEnvironment( GetChildrenEnvironment() );
        widget->SetDirty();
    }
    return true;
}

bool HubWidget::Remove(Widget *widget)
{
    DBG_Remove;
    if( Retrieve(widget) ) 
    { 
        widget->SetEnvironment(0);
        if ( GetEnvironment() )
        {
            GetEnvironment()->GetTrashCan().Trash(widget); 
        }
        else delete widget;
        return true;
    }
    else
        return false;
}

bool HubWidget::Retrieve(Widget *widget)
{
    ZWidgetList::ConstIterator iter;

    if (_z_indices.FindID(widget, &iter) )
    {
        widget->SetDirty( widget->GetBoundingRect() );
        widget->SetParent(NULL);

        _z_indices.RemoveAt(iter);
        DBG_Retrieve;
        return true;
    }
    else
        return false;
}

void HubWidget::ReleaseAll()
{
    ZWidgetList::Iterator iter = _z_indices.Begin();
    for ( ;!!iter; ++iter )
    {
        try 
        { 
            delete iter.Value();
            int debug_int = 0;
        }
        catch(...) 
        { 
            WIDGET_ERROR( Error("HubWidget::ReleaseAll(): Could not delete widget %0x", iter.Value() ) ); 
        }
    }
    _z_indices.Clear();
}

void HubWidget::Resize(int w, int h)
{
    SetDimensions( w, h );
}

void HubWidget::BringOnTop( Widget *widget ) 
{
    if ( IsChild(widget) )
    {
        if ( !IsOnTop(widget) )
        {
            int hard_index = _z_indices.GetNumberByValue(widget);
            int num = _z_indices.Size();

            _z_indices.MoveElement( hard_index, num-1 );
            widget->SetDirty();
        }
    }
    else
    {
        WIDGET_ERROR( Error("BringOnTop(%0x): Not my child", widget) );
    }
}

void HubWidget::Lift( int num_layers, Widget *widget ) 
{
    if ( IsChild(widget) )
    {
        int hard_index = _z_indices.GetNumberByValue(widget);
        int num = _z_indices.Size();
        int dest  = ( hard_index+num_layers>num )? num-1 : ( hard_index+num_layers>0 )? hard_index+num_layers : 0;
        
        _z_indices.MoveElement( hard_index, dest );
        widget->SetDirty();
    }
    else
    {
        WIDGET_ERROR( Error("Lift(%0x): Not my child", widget) );
    }
}


bool HubWidget::IsOnTop( Widget *widget ) const
{
    return  WhoIsOnTop() == widget;
}

int HubWidget::GetNearestZ() const
{
    ZWidgetList::ConstIterator iter = _z_indices.ConstEnd();
    return (!!--iter)? iter.Key() : 0;
}

Widget* HubWidget::WhoIsOnTop() const
{
    ZWidgetList::ConstIterator iter = _z_indices.ConstEnd();
    return (!!--iter)? iter.Value() : NULL;
}

int HubWidget::GetChildZIndex(Widget *widget) const
{
    if ( IsChild(widget) )    
    {
        return *( _z_indices.FindID(widget) );
    }
    else
    {
        WIDGET_NOTICE1( Error("Lift(%0x): Not my child", widget) );
        return -1;
    }
}

void HubWidget::SetChildZIndex(int z, Widget *widget)
{
    ZWidgetList::ConstIterator iter;

    if ( _z_indices.FindID(widget, &iter) )
    {
        _z_indices.RemoveAt(iter);
        _z_indices.Insert(z, widget);
        widget->SetDirty();
    }
    else
    {
        WIDGET_NOTICE1( Error("Lift(%0x): Not my child", widget) );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


};// namespace Useless

#undef WIDGET_ERROR
#undef WIDGET_NOTICE3
#undef WIDGET_NOTICE2
#undef WIDGET_NOTICE1
