#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateSystem.h"
#include "Useless/XML/Resources/CreateText.h"

namespace Useless {

Resource* CreatePopup( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    const char *p_name = "create popup";
    BeginParentWidget( env, p_name, ch_env );

    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    int modal = 0;
    try_attribute_i( attributes, "modal", &modal );
  
    //temporary hub
    SPointer<HubWidget> ptr = new HubWidget( 100, 100 );
    ch_env["parent_hub"] = ptr.get();

    // create children elements
    ChildsFromXML( i, &ch_env );

    Widget* p_content = ptr->FirstChild();

    if( ptr->ChildrenCount( ) > 1 )
        throw Error("Popup %s has more than 1 content.", (*i)._name.c_str());    
    else if( !ptr->Retrieve( p_content ) )
            throw Error("There is no content in popup %s.", (*i)._name.c_str());
    
    Popup* p_popup;  
    
    std::string title = "...";
    try_attribute( attributes, "title", &title );

    int titlebarHeight = 0;    
    try_attribute_i( attributes, "titlebar_height", &titlebarHeight );

    int titleAlign = 0;    
    try_attribute_i( attributes, "title_align", &titleAlign );

    std::vector<int> pos = to_integer_vector( "0,0" );
    std::string position;   
    if ( try_attribute( attributes, "title_distance", &position ) )
    {
        pos = to_integer_vector( position );
        if ( pos.size() != 2 ) { throw Error("pos attribute must have x and y coordinates"); }
    }

    std::string titleSkinSet = get_attribute( attributes, "titlebar_skin");
    std::string titleFontSet = get_attribute( attributes, "titlebar_font");
    
    // Nullary skins are not supported by Popup
    SkinMan *skins;
    FontSet *fonts;
    //SkinSet  skins0;
    //FontSet  fonts0;

    
    if (true /*try_attribute( attributes, "titlebar_skin", &titleSkinSet)*/)
    {
        skins = &QuerySkinMan(titleSkinSet);
    }
    //else { skins = &skins0;
    //}
    if (true /*try_attribute( attributes, "titlebar_font", &titleFontSet)*/)
    {
        fonts = &QueryFontSet(titleFontSet);
    }
    //else { fonts = &fonts0;
    //}

    CaptionStyle style( *skins, *fonts, titleAlign, Pos(pos[0], pos[1]) );
    
    p_popup = new Popup( p_content, style, MakeText( title ), titlebarHeight,modal);    
    
    std::string owner;
    
    if( try_attribute(attributes, "owner", &owner) )
    {
        pos = to_integer_vector( "0,0" );
        std::string position;   
        if ( try_attribute( attributes, "pos", &position ) )
        {
            pos = to_integer_vector( position );
            if ( pos.size() != 2 ) { throw Error("pos attribute must have x and y coordinates"); }
        }

        QueryWorkspace( owner ).AddPopup( p_popup, Pos(pos[0], pos[1]) );
    }
    
    ApplyWidgetSize( p_popup, attributes );
    ApplyWidgetAttr( p_popup, attributes );
    
    return new AnyResource<Popup*>( p_popup );
}

};//namespace Useless



