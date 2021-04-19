#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateSkinSet( XMLIterator i, XMLCreatorEnv *env )
{
    const char *p_cat, *p_name;
    if ( !(*env).Query("category", p_cat) || strcmp( p_cat, "skins" )!=0 )
    {
        throw Error("CreateSkinSet(): cannot create skinset in %s context (must be skins)", p_cat);
    }

    XMLCreatorEnv ch_env;
    ch_env.Set( "category", p_cat );

    if ( (*env).Query("name", p_name ) )
    {
        ch_env.Set( "branch", p_name );
    }

    SPointer<SkinMan> sp_skin_man;
    SkinSet *p_skin_set;
    sp_skin_man = p_skin_set = new SkinSet();
    AnyResource< SPointer<SkinMan> > *p_skins_resource = new AnyResource< SPointer<SkinMan> >( sp_skin_man );
    ch_env.Set("parent_skinset", p_skin_set );

    std::string map_name;
    if ( try_attribute( (*i)._attributes, "mapping", &map_name ) )
    {
        ch_env.Set("skin_mapping", map_name.c_str() );
        ch_env.Set("skin_index", 0);
    }

    // create children elements
    ChildsFromXML( i, &ch_env );

    return p_skins_resource;
}

bool InsertChildSkin( const Skin &skin, XMLCreatorEnv *env )
{
    // detect parent skinset
    SkinSet *p_skins=0;
    if ( (*env).Query("parent_skinset", p_skins) )
    {
        int index=0;
        const char *p_map_name;
        if ( !(*env).Query("skin_mapping", p_map_name) ||
             !(*env).Query("skin_index", index) )
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
        p_skins->Insert( (*p_map)[index], skin );
        (*env).Set("skin_index", ++index );

        return true;
    }
    else
        return false;
}

void SetupSkinAttributes( Skin &skin, const XMLAttributes &attributes)
{
    std::string stretch;
    if ( try_attribute( attributes, "stretch", &stretch) )
    {
        if ( stretch=="NONE")
        {
            skin.SetDirMask( Pos(0,0));
        }
        else if ( stretch=="VERTICAL")
        {
            skin.SetDirMask( Pos(0,1));
        }
        else if ( stretch=="HORIZONTAL")
        {
            skin.SetDirMask( Pos(1,0));
        }
        else {
            skin.SetDirMask( Pos(1,1));
        }
    }
    
    std::string dims_str;
    if ( try_attribute( attributes, "dimensions", &dims_str))
    {
        std::vector<int> dims = to_integer_vector( dims_str);
        Dim2i d( dims[0], dims[1]);
        skin.SetSize( d);
    }
}

Resource* ResolveSkinLink( XMLIterator i, XMLCreatorEnv *env )
{
    std::string id;
    if ( (*i)._name == "copy" )
    {
        id = get_attribute( (*i)._attributes, "skin" );
        SPointer< Skin > spSkin = &CopyOf< Skin >( QuerySkin( id ));
        SetupSkinAttributes( *spSkin, (*i)._attributes );
        return new AnyResource< SPointer< Skin > >( spSkin );
    }
    else
    {
        id = get_attribute( (*i)._attributes, "id" );
        SPointer< Skin > spSkin = &CopyOf< Skin >( QuerySkin( id ));
        SetupSkinAttributes( *spSkin, (*i)._attributes );
        InsertChildSkin( (*spSkin), env );
    }
    return NULL;
}

};//namespace Useless
