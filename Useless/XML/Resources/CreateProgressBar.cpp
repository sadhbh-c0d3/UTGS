#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateProgressBar( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string skin_frame = get_attribute( attributes, "frame" );
    std::string skin_interior = get_attribute( attributes, "interior" );
    int size = to_integer( get_attribute( attributes, "size" ) );
    int range = to_integer( get_attribute( attributes, "range" ) );
    
    Orientation orientation = HORIZONTAL;
    std::string orient;
    if( try_attribute( attributes, "orientation", &orient ) )
    {
        if( orient == "vertical" )
            orientation = VERTICAL;
    }

    ProgressBar *ptr = new ProgressBar( range, size, QuerySkin( skin_frame ), 
                                           QuerySkin( skin_interior ), orientation ); 

    int advance;
    if( try_attribute_i( attributes, "advance", &advance ) )
    {
        ptr->SetAdvance( advance );
    }

    InsertChildWidget( ptr, attributes, env);    

    return new AnyResource<ProgressBar*>( ptr );
}

}; // namespace Useless
