#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateImageWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    ImageWidget *ptr;
    std::string img;
    if ( try_attribute( attributes, "img", &img ))
    {
        int first=0, count;
        if ( try_attribute_i( attributes, "count", &count ))
        {
            try_attribute_i( attributes, "first", &first );

            for ( int i=0; i<count; ++i )
            {            
                ptr = new ImageWidget( *MakeImage( img + '[' + from_integer<char>(first+i) + ']'));
                InsertChildWidget( ptr, attributes, env);
            }
            return 0;
        }
        else
        {
            ptr = new ImageWidget( *MakeImage( img )); 
        }
    }
    else
    {
        std::string skin = get_attribute( attributes, "skin" );
        ptr = new ImageWidget( QuerySkin( skin ) ); 
    }
    
    InsertChildWidget( ptr, attributes, env);
    return new AnyResource<Widget*>( ptr );
}

}; // namespace Useless
