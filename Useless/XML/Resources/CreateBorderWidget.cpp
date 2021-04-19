#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateBorderWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string skin = get_attribute( attributes, "skin" );
    int w = to_integer( get_attribute( attributes, "width" ) );
    int h = to_integer( get_attribute( attributes, "height" ) );
    
    BorderWidget *ptr = new BorderWidget( w, h, QuerySkin( skin ) ); 
    InsertChildWidget( ptr, attributes, env);    
    return new AnyResource<BorderWidget*>( ptr );
}

}; // namespace Useless
