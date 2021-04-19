#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateMapping( XMLIterator i, XMLCreatorEnv *env )
{
    std::string map_str = get_attribute( (*i)._attributes, "values" );
    to_integer_vector(map_str);

    return new AnyResource< std::vector<int> >( to_integer_vector(map_str) );
}

};//namespace Useless
