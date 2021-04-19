#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateBarWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string skin = get_attribute( attributes, "skin" );
    int size = to_integer( get_attribute( attributes, "size" ) );

    BarWidget *ptr = new BarWidget( QuerySkin( skin ), size ); 
    InsertChildWidget( ptr, attributes, env);    
    return new AnyResource<BarWidget*>( ptr );
}

}; // namespace Useless
