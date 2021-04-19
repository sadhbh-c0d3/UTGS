#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateHints.h"

#include "Useless/GUI/Widgets/ListMenu.h"
#include "Useless/GUI/Widgets/TableMenu.h"
#include "Useless/GUI/Logic/RadioLogic.h"

namespace Useless {

Resource *
CreateDropDown( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    const char *p_name = "create dropdown";
    BeginParentWidget( env, p_name, ch_env );
    
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    // skins & fonts for caption
    std::string skinName = get_attribute( attributes, "title_skin");
    std::string fontName = get_attribute( attributes, "title_font");
    
    SkinMan &skins = QuerySkinMan( skinName );
    FontSet &fonts = QueryFontSet( fontName );

    // caption position
    int titleAlign = 0;    
    try_attribute_i( attributes, "title_align", &titleAlign );
    
    std::vector<int> pos = to_integer_vector( "0,0" );
    std::string position;   
    if ( try_attribute( attributes, "title_distance", &position ) )
    {
        pos = to_integer_vector( position );
        if ( pos.size() != 2 ) { throw Error("pos attribute must have x and y coordinates"); }
    }
    
    CaptionStyle style( skins, fonts, titleAlign, Pos(pos[0], pos[1]) );

    
    // temporary hub for child widget
    SPointer<HubWidget> ptr = new HubWidget( 100, 100 );
    ch_env["parent_hub"] = ptr.get();

    // create child element
    ChildsFromXML( i, &ch_env );
   
    if( ptr->ChildrenCount() != 1 )
        throw Error("DropDown widget %s must have one and only content.", (*i)._name.c_str());

    Widget *content = ptr->FirstChild();
    ptr->Retrieve( content );

    if ( IWList *p_items = dynamic_cast< IWList *>( content ))
    {
        typedef DropButton< ListMenu, RadioLogic > ListMenuButton;
        
        ListMenu *lm = new ListMenuButton( style, p_items );
        p_items->Items().SetFollowMode( ListLogic::FULL_FOLLOW );
        p_items->Items().AllwaysSelected( 1 );
        
        InsertChildWidget( lm, attributes, env );
        SetHint( lm, attributes ); 
        return new AnyResource< Widget *>( lm );
    }
    else if ( IWTableList *p_table = dynamic_cast< IWTableList *>( content ))
    {
        Share< DataConnector > dataCon =
            dynamic_cast< DataTableWidget &>( p_table->Table() )
                .GetDataConnector();
        
        typedef DropButton< TableMenu, RadioLogic > TableMenuButton;
        TableMenu *tm = new TableMenuButton( style, p_table, dataCon.GetTarget() );
        
        dynamic_cast< ListLogic &>( p_table->Enumerator() )
            .SetFollowMode( ListLogic::FULL_FOLLOW );
        dynamic_cast< ListLogic &>( p_table->Enumerator() )
            .AllwaysSelected( 1 );
        
        InsertChildWidget( tm, attributes, env );
        SetHint( tm, attributes ); 
        return new AnyResource< Widget *>( tm );
    }

    return NULL;
}


};//Useless
