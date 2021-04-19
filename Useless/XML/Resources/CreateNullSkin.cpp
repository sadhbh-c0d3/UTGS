#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateNullSkin( XMLIterator i, XMLCreatorEnv *env )
{   
    int w = 0;
    try_attribute_i( (*i)._attributes, "width", &w );
    int h = 0;
    try_attribute_i( (*i)._attributes, "height", &h );

    SPointer<Skin> p_skin = new NullSkin( w, h );
    InsertChildSkin( *p_skin, env);    
    return new AnyResource< SPointer<Skin> >( p_skin );
}

}; // namespace Useless
