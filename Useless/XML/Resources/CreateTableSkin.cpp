#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateTableSkin( XMLIterator i, XMLCreatorEnv *env )
{   
    // get image
    std::string img;
    img = get_attribute( (*i)._attributes, "img" );
    SPointer<ImageBase> sp_image = MakeImage( img );

    // get parallel cut points    
    std::string cut_str = get_attribute( (*i)._attributes, "xcut" );
    std::vector<int> xcut=to_integer_vector(cut_str);
    if ( xcut.size() != 3 )
    {
        throw Error("TableSkin needs 3 x-cut points");
    }
    
    // get orthogonal cut points    
    cut_str = get_attribute( (*i)._attributes, "ycut" );
    std::vector<int> ycut=to_integer_vector(cut_str);
    if ( ycut.size() != 3 )
    {
        throw Error("TableSkin needs 3 y-cut points");
    }

    // get boundary rect
    cut_str = "0,0,0,0";
    std::vector<int> boundry;

    if (try_attribute( (*i)._attributes, "boundry", &cut_str ))
    {
        boundry = to_integer_vector(cut_str);
    }
    else
    {
        boundry .push_back(0);
        boundry .push_back(0);
        boundry .push_back(sp_image->GetWidth());
        boundry .push_back(sp_image->GetHeight());
    }

    // get inner hilite rect
    cut_str = "0,0,0,0";
    try_attribute( (*i)._attributes, "hilite_inner", &cut_str );
    std::vector<int> hli=to_integer_vector(cut_str);

    // get outer hilite rect
    cut_str = "0,0,0,0";
    try_attribute( (*i)._attributes, "hilite_outer", &cut_str );
    std::vector<int> hlo=to_integer_vector(cut_str);

    if ( boundry.size() != 4 || 
         hlo.size() != 4 || 
         hli.size() != 4 )
    {
        throw Error("Rect needs 4 points");
    }

    int no_interrior = 0;
    try_attribute_i( (*i)._attributes, "no_interrior", &no_interrior );

    SPointer<Skin> p_skin = new TableSkin( 
                                           *sp_image, 
                                           Rect( boundry[0], boundry[1], boundry[2], boundry[3] ),
                                           xcut[0], xcut[1], xcut[2],
                                           ycut[0], ycut[1], ycut[2],
                                           no_interrior,
                                           Rect( hlo[0], hlo[0], hlo[0], hlo[0] ),
                                           Rect( hli[0], hli[0], hli[0], hli[0] )
                                         );
    InsertChildSkin( *p_skin, env);    
    return new AnyResource< SPointer<Skin> >( p_skin );
}

}; // namespace Useless
