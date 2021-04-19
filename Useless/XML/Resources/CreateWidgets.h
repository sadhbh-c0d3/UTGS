#ifndef __INCLUDED_CREATE_WIDGETS_H__
#define __INCLUDED_CREATE_WIDGETS_H__


#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/GUI/Misc/Layout.h"
#include "Useless/GUI/Widgets/ActiveAreaWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/AdvListWidget.h"
#include "Useless/GUI/Widgets/AdvScrollbarWidget.h"
#include "Useless/GUI/Widgets/AdvTableSystemWidget.h"
#include "Useless/GUI/Widgets/AdvTextWidget.h"
#include "Useless/GUI/Widgets/CaptionWidget.h"
#include "Useless/GUI/Widgets/ContentWidget.h"
#include "Useless/GUI/Widgets/CreditsWidget.h"
#include "Useless/GUI/Widgets/DropDown.h"
#include "Useless/GUI/Widgets/DualTextWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"
#include "Useless/GUI/Widgets/FrameWidget.h"
#include "Useless/GUI/Widgets/GraphWidget.h"
#include "Useless/GUI/Widgets/ImageWidget.h"
#include "Useless/GUI/Widgets/ListWidget.h"
#include "Useless/GUI/Widgets/LoomWidget.h"
#include "Useless/GUI/Widgets/Popup.h"
#include "Useless/GUI/Widgets/ProgressBar.h"
#include "Useless/GUI/Widgets/RollerWidget.h"
#include "Useless/GUI/Widgets/ScrollbarWidget.h"
#include "Useless/GUI/Widgets/ScrolledWidget.h"
#include "Useless/GUI/Widgets/SizerWidget.h"
#include "Useless/GUI/Widgets/SliderWidget.h"
#include "Useless/GUI/Widgets/TableListedWidgets.h"
#include "Useless/GUI/Widgets/TableSystemWidget.h"
#include "Useless/GUI/Widgets/TextField.h"
#include "Useless/GUI/Widgets/TextWidget.h"

namespace Useless {

/* Resource creation methods
-----------------------------------------------------------------------------*/
USELESS_RESOURCES_API Resource* CreateActiveArea      ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateButtonWidget    ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateCredits         ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateDropDown        ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateFrameWidget     ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateGraphWidget     ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateHLText          ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateImageWidget     ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateLayoutGap       ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateListWidget      ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateLoomWidget      ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreatePopup           ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateProgressBar     ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateRollerWidget    ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateScoreRoller     ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateScrollbar       ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateScrolledWidget  ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateSizerWidget     ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateSlider          ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateTableView       ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateTableCells      ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateTemplateWidget  ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateTextField       ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateTextWidget      ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* CreateWidgetTableWidget   ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* InstantiateTemplateWidget ( XMLIterator i, XMLCreatorEnv *env );
USELESS_RESOURCES_API Resource* ResolveParentWidget   ( XMLIterator i, XMLCreatorEnv *env );

/* Resource creation registry
-----------------------------------------------------------------------------*/
static RegisterXMLCreator reg_create_active_area_widget(
        "widgets::activearea", CreateActiveArea );

static RegisterXMLCreator reg_create_active_text_widget(
        "widgets::activetext", CreateTextWidget );

static RegisterXMLCreator reg_create_button_widget(
        "widgets::button", CreateButtonWidget );

static RegisterXMLCreator reg_create_credits(
        "widgets::credits", CreateCredits );

static RegisterXMLCreator reg_create_dragger_widget(
        "widgets::dragger", CreateButtonWidget );

static RegisterXMLCreator reg_create_drop_down(
        "widgets::dropdown", CreateDropDown );

static RegisterXMLCreator reg_create_frame_widget(
        "widgets::frame", CreateFrameWidget );

static RegisterXMLCreator reg_create_gap(
        "widgets::gap", CreateLayoutGap );

static RegisterXMLCreator reg_create_graph_widget(
        "widgets::graph", CreateGraphWidget );

static RegisterXMLCreator reg_create_hilite(
        "widgets::hilite", CreateButtonWidget );

static RegisterXMLCreator reg_create_hltext_widget(
        "widgets::hltext", CreateHLText );

static RegisterXMLCreator reg_create_hub_widget(
        "widgets::hub", CreateFrameWidget );

static RegisterXMLCreator reg_create_image_widget(
        "widgets::image", CreateImageWidget );

static RegisterXMLCreator reg_create_list_widget(
        "widgets::list", CreateListWidget );

static RegisterXMLCreator reg_create_loom_widget(
        "widgets::loom", CreateLoomWidget );

static RegisterXMLCreator reg_create_mark(
        "widgets::mark", CreateButtonWidget );

static RegisterXMLCreator reg_create_multifont_widget(
        "widgets::multifont", CreateHLText );

static RegisterXMLCreator reg_create_popup(
        "widgets::popup", CreatePopup );

static RegisterXMLCreator reg_create_progress_widget(
        "widgets::progressbar", CreateProgressBar );

static RegisterXMLCreator reg_create_roller_widget(
        "widgets::roller", CreateRollerWidget );

static RegisterXMLCreator reg_create_radio(
        "widgets::radio", CreateButtonWidget );

static RegisterXMLCreator reg_create_score_roller(
        "widgets::scoreroller", CreateScoreRoller );

static RegisterXMLCreator reg_create_scrollbar(
        "widgets::scrollbar", CreateScrollbar );

static RegisterXMLCreator reg_create_scrolled(
        "widgets::scrolled", CreateScrolledWidget );

static RegisterXMLCreator reg_create_sizer_widget(
        "widgets::sizer", CreateSizerWidget );

static RegisterXMLCreator reg_create_slider(
        "widgets::slider", CreateSlider );

static RegisterXMLCreator reg_create_switch(
        "widgets::switch", CreateButtonWidget );

static RegisterXMLCreator reg_create_tab(
        "widgets::tab", CreateButtonWidget );

static RegisterXMLCreator reg_create_table_listed_widgets(
        "widgets::table", CreateWidgetTableWidget );

static RegisterXMLCreator reg_create_table_list(
        "widgets::tablelist", CreateWidgetTableWidget );

static RegisterXMLCreator reg_create_table_view_widget(
        "widgets::tableview", CreateTableView );

static RegisterXMLCreator reg_create_table_row(
        "widgets::row", CreateTableCells );

static RegisterXMLCreator reg_create_table_col(
        "widgets::col", CreateTableCells );

static RegisterXMLCreator reg_create_template_widget(
        "widgets::template", CreateTemplateWidget );

static RegisterXMLCreator reg_create_text_widget(
        "widgets::text", CreateTextWidget );

static RegisterXMLCreator reg_create_text_field(
        "widgets::textfield", CreateTextField );

static RegisterXMLCreator reg_create_trigger(
        "widgets::trigger", CreateButtonWidget );

static RegisterXMLCreator reg_instantiate_template_widget(
        "widgets::instance", InstantiateTemplateWidget );

static RegisterXMLCreator reg_resolve_parent_widget(
        "widgets::parent", ResolveParentWidget );


/* Convertion traits
-----------------------------------------------------------------------------*/
template<>
struct Converters< Widget *>
    : DynamicConverter< Widget, ActiveWidget >
    , DynamicConverter< Widget, EnumWidget >
    , DynamicConverter< Widget, CaptionWidget >
    , DynamicConverter< Widget, ContentWidget >
    , DynamicConverter< Widget, FontChanger >
    , DynamicConverter< Widget, BaseTextWidget >
    , DynamicConverter< Widget, BaseDualText >
    , DynamicConverter< Widget, AdvTextBase >
    , DynamicConverter< Widget, ActiveText >
    , DynamicConverter< Widget, ActiveAreaWidget >
{};

template<>
struct Converters< HubWidget *>
    : DynamicConverter< HubWidget, Widget >
{};

template<>
struct Converters< SizerWidget *>
    : DynamicConverter< SizerWidget, Widget >
{};

template<>
struct Converters< ListWidget *>
    : DynamicConverter< ListWidget, Widget >
    , DynamicConverter< ListWidget, EnumWidget >
    , DynamicConverter< ListWidget, ActiveWidget >
{};

template<>
struct Converters< AdvListWidget *>
    : DynamicConverter< AdvListWidget, Widget >
    , DynamicConverterTo< ListWidget >
    , DynamicConverterTo< EnumWidget >
    , DynamicConverterTo< ActiveWidget >
    , DynamicConverterTo< ScrolledWidget >
{
    virtual ListWidget * Apply( void *ptr, DynamicConverterTo< ListWidget >::Tag )
    {
        return (& UnrawPtr< AdvListWidget *>(ptr)->Items() );
    }
    
    virtual EnumWidget * Apply( void *ptr, DynamicConverterTo< EnumWidget >::Tag )
    {
        return (& UnrawPtr< AdvListWidget *>(ptr)->Enumerator() );
    }
    
    virtual ActiveWidget * Apply( void *ptr, DynamicConverterTo< ActiveWidget >::Tag )
    {
        return (& UnrawPtr< AdvListWidget *>(ptr)->Control() );
    }
    
    virtual ScrolledWidget * Apply( void *ptr, DynamicConverterTo< ScrolledWidget >::Tag )
    {
        return (& UnrawPtr< AdvListWidget *>(ptr)->Scrollee() );
    }
};

template<>
struct Converters< LoomWidget *>
    : DynamicConverter< LoomWidget, Widget >
{};

template<>
struct Converters< Popup *>
    : DynamicConverter< Popup, Widget >
    , DynamicConverter< Popup, ActiveWidget >
    , DynamicConverter< Popup, FormWidget >
{};

template<>
struct Converters< ProgressBar *>
    : DynamicConverter< ProgressBar, Widget >
{};

template<>
struct Converters< ScrolledWidget *>
    : DynamicConverter< ScrolledWidget, Widget >
    , DynamicConverter< ScrolledWidget, ActiveWidget >
{};

template<>
struct Converters< SliderWidget *>
    : DynamicConverter< SliderWidget, Widget >
    , DynamicConverter< SliderWidget, EnumWidget >
    , DynamicConverter< SliderWidget, ActiveWidget >
{};

template<>
struct Converters< ScrollbarWidget *>
    : DynamicConverter< ScrollbarWidget, Widget >
    , DynamicConverter< ScrollbarWidget, ActiveWidget >
{};

template<>
struct Converters< AdvScrollbarWidget *>
    : DynamicConverter< AdvScrollbarWidget, Widget >
    , DynamicConverter< AdvScrollbarWidget, ActiveWidget >
{};

template<>
struct Converters< WidgetTableWidget *>
    : DynamicConverter< WidgetTableWidget, Widget >
    , DynamicConverter< WidgetTableWidget, EnumWidget >
    , DynamicConverter< WidgetTableWidget, ActiveWidget >
    , DynamicConverter< WidgetTableWidget, DataTableWidget >
    , DynamicConverter< WidgetTableWidget, TableList >
{};

template<>
struct Converters< TableSystemWidget *>
    : DynamicConverter< TableSystemWidget, Widget >
    , DynamicConverter< TableSystemWidget, AdvTableSystemWidget >
    , DynamicConverterTo< EnumWidget >
{
    virtual EnumWidget * Apply( void *ptr, DynamicConverterTo< EnumWidget >::Tag )
    {
        return ( & UnrawPtr< TableSystemWidget * >(ptr)->Enumerator() );
    }
};

template<>
struct Converters< TextField *>
    : DynamicConverter< TextField, Widget >
    , DynamicConverter< TextField, ActiveWidget >
{};




/*! Using static method: WidgetResource<>::Query()
 *  is the common way to query widget resources.
 */
template< class _WidgetType >
struct WidgetResource
{
    static _WidgetType * Query( const std::string &id )
    {
        Resource *res = Resources::Instance().QueryResource("widgets", id );
        if ( 0 == res )
        {
            throw Error("Widget resource does not exist\n"
                    "name: %s"
                    , id.c_str());
        }
        _WidgetType *wid = PointerResourceConvertion< _WidgetType >::Apply( res );
        
        if ( 0 == wid ) // try other way
        {
            Widget *pw = PointerResourceConvertion< Widget >::Apply( res );
            wid = dynamic_cast< _WidgetType *>( pw );
            C_warnf( WidgetResource, 0, (0 != wid),
                    ("Widget resource does not convert to:", typeid(_WidgetType).name(),
                     "\nUsed dynamic_cast instead."
                     ));
        }
        
        if ( 0 == wid )
        {
            throw Error("Widget resource does not convert to: %s\n"
                    "name: %s\n"
                    "type: %s"
                    , typeid(_WidgetType).name()
                    , id.c_str()
                    , res->GetTypeName() );
        }
        return wid;
    }
};


/* Querying methods
-----------------------------------------------------------------------------*/

inline LayoutBase &     QueryLayout( const std::string &id )
{
    LayoutBase **p;
    Resources::Instance().Query("layouts", id, p);
    return **p;
}


inline Widget *         QueryWidget( const std::string &id )
{
    return WidgetResource< Widget >::Query( id );
}

inline HubWidget *      QueryHubWidget( const std::string &id )
{
    return WidgetResource< HubWidget >::Query( id );
}

inline SizerWidget *    QuerySizerWidget( const std::string &id )
{
    return WidgetResource< SizerWidget >::Query( id );
}

inline ActiveWidget *   QueryActiveWidget( const std::string &id )
{
    return WidgetResource< ActiveWidget >::Query( id );
}

inline CaptionWidget *  QueryCaptionWidget( const std::string &id )
{
    return WidgetResource< CaptionWidget >::Query( id );
}

inline ContentWidget *  QueryContentWidget( const std::string &id )
{
    return WidgetResource< ContentWidget >::Query( id );
}

inline EnumWidget *     QueryEnumWidget( const std::string &id )
{
    return WidgetResource< EnumWidget >::Query( id );
}

inline CreditsWidget *  QueryCredits( const std::string &id )
{
    return WidgetResource< CreditsWidget >::Query( id );
}

inline ImageWidget *    QueryImageWidget( const std::string &id )
{
    return WidgetResource< ImageWidget >::Query( id );
}

inline GraphWidget *    QueryGraphWidget( const std::string &id )
{
    return WidgetResource< GraphWidget >::Query( id );
}

inline ListWidget *     QueryListWidget( const std::string &id )
{
    return WidgetResource< ListWidget >::Query( id );
}

inline AdvListWidget *  QueryAdvListWidget( const std::string &id )
{
    return WidgetResource< AdvListWidget >::Query( id );
}

inline LoomWidget *     QueryLoomWidget( const std::string &id )
{
    return WidgetResource< LoomWidget >::Query( id );
}

inline Popup *          QueryPopup( const std::string &id )
{
    return WidgetResource< Popup >::Query( id );
}

inline ProgressBar *    QueryProgressBar( const std::string &id )
{
    return WidgetResource< ProgressBar >::Query( id );
}

inline ScrolledWidget * QueryScrolledWidget( const std::string &id )
{
    return WidgetResource< ScrolledWidget >::Query( id );
}

inline SliderWidget *   QuerySlider( const std::string &id )
{
    return WidgetResource< SliderWidget >::Query( id );
}

inline ScrollbarWidget *    QueryScrollbar( const std::string &id )
{
    return WidgetResource< ScrollbarWidget >::Query( id );
}

inline AdvScrollbarWidget * QueryAdvScrollbar( const std::string &id )
{
    return WidgetResource< AdvScrollbarWidget >::Query( id );
}

inline WidgetTableWidget *  QueryWidgetTableWidget( const std::string &id )
{
    return WidgetResource< WidgetTableWidget >::Query( id );
}

inline TableListedWidgets * QueryTableListedWidget( const std::string &id )
{
    return WidgetResource< TableListedWidgets >::Query( id );
}

inline TextField *      QueryTextField( const std::string &id )
{
    return WidgetResource< TextField >::Query( id );
}

inline FontChanger *    QueryFontChanger( const std::string &id )
{
    return WidgetResource< FontChanger >::Query( id );
}

inline BaseTextWidget * QueryTextWidget( const std::string &id )
{
    return WidgetResource< BaseTextWidget >::Query( id );
}

inline BaseDualText *   QueryDualTextWidget( const std::string &id )
{
    return WidgetResource< BaseDualText >::Query( id );
}

inline AdvTextBase *    QueryAdvTextWidget( const std::string &id )
{
    return WidgetResource< AdvTextBase >::Query( id );
}



/* Utility methods
-----------------------------------------------------------------------------*/
void InsertChildWidget  ( Widget *p_child
                        , const XMLAttributes &attributes, XMLCreatorEnv *env );

void ApplyWidgetSize    ( Widget *ptr
                        , const XMLAttributes &attributes );

void ApplyWidgetAttr    ( Widget *ptr
                        , const XMLAttributes &attributes );

void BeginParentWidget  ( XMLCreatorEnv *env
                        , const char* &name
                        , XMLCreatorEnv &ch_env );


ScrollbarStyle *
MakeScrollbarStyle      ( XMLIterator i
                        , XMLCreatorEnv *env );

AdvScrollbarStyle *
MakeAdvScrollbarStyle   ( XMLIterator i
                        , XMLCreatorEnv *env );

};//Useless

#endif//__INCLUDED_CREATE_WIDGETS_H__
