#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateStyle.h"

namespace Useless {

Resource* CreateStyle( XMLIterator i, XMLCreatorEnv *env )
{
    AnyResource< XMLStyle > *p_res = new AnyResource< XMLStyle >();

    for ( i.StepInto(); !!i; ++i )
    {
        std::string name = (*i)._name;
        (*p_res->Get())[name] = (*i)._attributes;
    }

    return p_res;
}

void CascadeStyles
     (
          const XMLIterator &i,
          XMLCreatorEnv *env,
          XMLAttributes &attributes,
          XMLStyle &branch_style,
          XMLCreatorEnv &ch_env
      )
{ 
    XMLStyle *parent_style=0;
    const std::string &name = (*i)._name;
    const XMLParser::Node &node = (*i);

    XMLAttributes::iterator attr_i = attributes.end();
    insert( node._attributes.begin(), node._attributes.end(), attributes, attr_i );
    
    std::string style_name;
    if ( try_attribute( (*i)._attributes, "style", &style_name ) )
    {
        const XMLStyle &ref_style = QueryStyle( style_name );
        
        XMLStyle::iterator sty_i = branch_style.end();
        insert( ref_style.begin(), ref_style.end(), branch_style, sty_i );
    }

    if ( (*env).Query("parent_style", parent_style ) )
    {
        XMLStyle::iterator sty_i = branch_style.end();
        insert( parent_style->begin(), parent_style->end(), branch_style, sty_i );
    }

    XMLStyle::iterator st_it = branch_style.find(name);
    if ( st_it != branch_style.end() )
    {
        XMLAttributes::iterator attr_i = attributes.end();
        insert( (*st_it).second.begin(), (*st_it).second.end(), attributes, attr_i );
    }
    
    std::string inner_style_name;
    if ( try_attribute( (*i)._attributes, "inner_style", &inner_style_name ) )
    {
        const XMLStyle &ref_style = QueryStyle( inner_style_name );
        ch_env.Set("parent_style", &ref_style );
    }
    else if ( !branch_style.empty() )
    {
        int disable_style = false;
        try_attribute_i( (*i)._attributes, "disable_style", &disable_style );

        if ( !disable_style ) // apply Cascading Style Derivation for children
        {
            ch_env.Set("parent_style", &branch_style);
        }
    }
}

};//namespace Useless
