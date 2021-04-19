#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateFonts.h"

namespace Useless {

Resource* CreateFontSet( XMLIterator i, XMLCreatorEnv *env )
{
    const char *p_cat, *p_name;
    if ( !(*env).Query("category", p_cat) || strcmp( p_cat, "fonts" )!=0 )
    {
        throw Error("CreateFontSet(): cannot create fontset in %s context (must be fonts)", p_cat);
    }

    XMLCreatorEnv ch_env;
    ch_env.Set( "category", p_cat );

    if ( (*env).Query("name", p_name ) )
    {
        ch_env.Set( "branch", p_name );
    }


    AnyResource<FontSet> *p_fonts_resource = new AnyResource<FontSet>();
    ch_env.Set("parent_fontset", p_fonts_resource->Get() );

    std::string map_name;
    if ( try_attribute( (*i)._attributes, "mapping", &map_name ) )
    {
        ch_env.Set("font_mapping", map_name.c_str() );
        ch_env.Set("font_index", 0);
    }

    // create children elements
    ChildsFromXML( i, &ch_env );

    return p_fonts_resource;
}

bool InsertChildFont( const Font &font, XMLCreatorEnv *env )
{
    // detect parent fontset
    FontSet *p_fonts=0;
    if ( (*env).Query("parent_fontset", p_fonts) )
    {
        int index=0;
        const char *p_map_name;
        if ( !(*env).Query("font_mapping", p_map_name) ||
             !(*env).Query("font_index", index) )
        {
            throw Error("Parent exists without mapping (add mapping attribute to parent)");
        }
        // query mapping
        std::vector<int>* p_map;
        Resources::Instance().Query( "mappings", p_map_name, p_map);
        if ( (*p_map).size() <= index )
        {
            throw Error("Too small mapping %s (tag frame inside skinset)", p_map_name );
        }
        p_fonts->Insert( (*p_map)[index], font );
        (*env).Set("font_index", ++index );
        return true;
    }
    else
        return false;
}


Resource* ResolveFontLink( XMLIterator i, XMLCreatorEnv *env )
{
    std::string id = get_attribute( (*i)._attributes, "id" );
    Font& font = QueryFont( id );
    InsertChildFont( font, env );
    return NULL;
}

};//namespace Useless
