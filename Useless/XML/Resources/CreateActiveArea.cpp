#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateActiveArea( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    ActiveAreaWidget *ptr = new ActiveAreaWidget( 100, 100 );
    InsertChildWidget( ptr, attributes, env);    
    return new AnyResource< Widget *>( ptr );
}

}; // namespace Useless
