#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateBarSkins( XMLIterator i, XMLCreatorEnv *env )
{
    std::string src, cut_str, temp_str, map_name;

    // get source image
    src = get_attribute( (*i)._attributes, "img" );
    SPointer<ImageBase> sp_image = MakeImage( src );
    
    // get number of columns and rows
    int columns=1, rows=1;
    bool single_mode = ((*i)._name == "bar")? true : false;

    try_attribute_i( (*i)._attributes, "columns", &columns );
    try_attribute_i( (*i)._attributes, "rows", &rows );

    // detect parent skinset
    SkinSet *p_skins=0;
    bool use_parent_skinset = (*env).Query("parent_skinset", p_skins);

    // get cut points    
    cut_str = get_attribute( (*i)._attributes, "cut" );
    std::vector<int> cut=to_integer_vector(cut_str);
    if ( cut.size() != 3 )
    {
        throw Error("BarSkinner needs 3 cut points");
    }

    // get orientation flag
    Orientation orient = HORIZONTAL;
    if ( try_attribute( (*i)._attributes, "orientation", &temp_str ) )
    {
        if ( temp_str == "vertical" )
        {
            orient = VERTICAL;
        }
    }

    if ( single_mode && !use_parent_skinset)
    {
        return new AnyResource< SPointer<Skin> >(
            new BarSkinner(*sp_image, orient, cut[0], cut[1], cut[2]) );
    }
    
    // detect mapping override or parent mapping
    int first_index=0;
    if ( !try_attribute( (*i)._attributes, "mapping", &map_name ) )
    {
        const char *p_map_name;
        if ( !(*env).Query("skin_mapping", p_map_name) ||
             !(*env).Query("skin_index", first_index) )
        {
            throw Error("BarSkins needs mapping");
        }
        map_name = p_map_name;
    }

    // query mapping
    std::vector<int>* p_map;
    Resources::Instance().Query( "mappings", map_name, p_map);
    if ( (*p_map).size() < first_index + columns*rows )
    {
        throw Error("Too small mapping %s (tag bars)", map_name.c_str() );
    }


    // query or buid new skinset
    AnyResource< SPointer<SkinMan> > *p_skins_resource = 0;
    if ( !use_parent_skinset && !single_mode )
    {
        SPointer<SkinMan> sp_skin_man = p_skins = new SkinSet();
        p_skins_resource = new AnyResource< SPointer<SkinMan> >( sp_skin_man );
    }

    // build single skin (variant I)
    if ( single_mode )
    {
        AnyResource< SPointer<Skin> > *p_bs_res =
            new AnyResource< SPointer<Skin> >(
                new BarSkinner(*sp_image, orient, cut[0], cut[1], cut[2]) );

        if ( p_skins )
            p_skins->Insert( (*p_map)[first_index], *p_bs_res->Get() );

        (*env).Set("skin_index", ++first_index );

        return p_bs_res;
    }

    // build multiple skins (variant II)
    int idx=first_index;
    ImageBase &ref_image = *sp_image;
    int w = ref_image.GetWidth() / columns;
    int h = ref_image.GetHeight() / rows;

    for ( int j=0; j!=columns; ++j ) for ( int k=0; k!=rows; ++k )
    {
        Rect rc( j*w, k*h, w, h );
        int id = (*p_map)[idx];
        BarSkinner bar( ref_image, rc, orient, cut[0], cut[1], cut[2] );
        (*p_skins).Insert( id, bar );
        ++idx;
    }

    (*env).Set("skin_index", idx );

    return p_skins_resource;
}

};//namespace Useless
