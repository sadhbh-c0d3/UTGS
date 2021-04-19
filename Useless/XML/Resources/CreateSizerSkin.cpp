#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateSizerSkin( XMLIterator i, XMLCreatorEnv *env )
{   
    // get image
    std::string img;
    img = get_attribute( (*i)._attributes, "img" );
    SPointer<ImageBase> sp_image = MakeImage( img );

    // get parallel cut points    
    std::string cut_str = get_attribute( (*i)._attributes, "pcut" );
    std::vector<int> pcut=to_integer_vector(cut_str);
    if ( pcut.size() != 3 )
    {
        throw Error("SizerSkin needs 3 parallel cut points");
    }
    
    // get orthogonal cut points    
    cut_str = get_attribute( (*i)._attributes, "ocut" );
    std::vector<int> ocut=to_integer_vector(cut_str);
    if ( ocut.size() != 2 )
    {
        throw Error("SizerSkinn needs 2 orthogonal cut points");
    }

    // get boundary rect
    cut_str = "0,0,0,0";
    try_attribute( (*i)._attributes, "boundry", &cut_str );
    std::vector<int> boundry=to_integer_vector(cut_str);

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

    Orientation orientation = HORIZONTAL;
    std::string orient;
    if ( try_attribute( (*i)._attributes, "orientation", &orient ) && orient == "vertical" )
        orientation = VERTICAL;

    int no_interrior = 0;
    try_attribute_i( (*i)._attributes, "no_interrior", &no_interrior );

    SPointer<Skin> p_skin = new SizerSkin( 
                                           *sp_image, 
                                           Rect( boundry[0], boundry[1], boundry[2], boundry[3] ),
                                           orientation,
                                           pcut[0], pcut[1], pcut[2],
                                           ocut[0], ocut[1],
                                           no_interrior,
                                           Rect( hlo[0], hlo[0], hlo[0], hlo[0] ),
                                           Rect( hli[0], hli[0], hli[0], hli[0] )
                                         );
    InsertChildSkin( *p_skin, env);    
    return new AnyResource< SPointer<Skin> >( p_skin );
}

}; // namespace Useless
