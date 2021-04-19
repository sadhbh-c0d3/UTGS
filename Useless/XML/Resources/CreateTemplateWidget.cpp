#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"

namespace Useless {

Resource* CreateTemplateWidget( XMLIterator i, XMLCreatorEnv *env )
{
    const XMLParser::Node &node = *i;
    std::string class_name = get_attribute( node._attributes, "class" );
    
    XMLParser::Nodes nodes = i.SubTree();

    AnyResource< XMLParser::Nodes > * p_resource =
        new AnyResource< XMLParser::Nodes >( nodes );

    Resources::Instance().Insert( "classes", class_name, p_resource );

    return NULL;
}


};//namespace Useless
