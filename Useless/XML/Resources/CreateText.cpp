#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateText.h"
#include "Useless/GUI/Text/VerticalText.h"

namespace Useless {

Resource* CreateText( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );
    Text theValue;
    std::string link;
    if ( try_attribute( attributes, "link", &link ))
    {
        theValue = MakeText( link );
    }
    else
    {
        theValue = to_chars((*i)._value);
    }
    
    if ( (*i)._name == "dt_text" )
    {
        int placement = 0;
        try_attribute_i( attributes, "placement", &placement );
        SPointer<DT_Text> text = new DT_Text( theValue, placement );
        return new AnyResource<SPointer<DT_Text> >(text);
    }
    else 
        if ( (*i)._name == "text" )
        {
            SPointer<Text> text;
            int vertical = 0;
            if ( try_attribute_i( attributes, "vertical", &vertical ) && vertical == 1 )
                text = new VerticalText( theValue );
            else
                text = new Text( theValue );   

            return new AnyResource<SPointer<Text> >( text );
        }
        else 
            throw Error("Unsupported tag: %s in texts section.", (*i)._name.c_str());
}

Resource* ResolveParentGroup( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
  
    const char *p_name = "resolve parent group";
    BeginParentGroup( env, p_name, ch_env );
    
    ChildsFromXML( i, &ch_env );

    return NULL;
}

void BeginParentGroup(
      XMLCreatorEnv *env,
      const char*   &name,
      XMLCreatorEnv &ch_env )
{
    const char *p_cat;
    if ( !(*env).Query("category", p_cat) || strcmp( p_cat, "texts" )!=0 )
    {
        throw Error("Cannot %s in %s context (must be texts)", name, p_cat);
    }

    ch_env.Set( "category", p_cat ); //not needed - we have copy of parent

    if ( (*env).Query("name", name ) )
    {
        ch_env.Set( "branch", name ); // override branch name
    }
}

Text MakeText( const Text &input )
{
    __Resources::NameIndex nameIndex;
    nameIndex.second = 0;

    for ( Text::const_iterator iter = input.begin();
            iter != input.end(); ++iter )
    {
        char c = *iter;
        if ( c < 32 )
        {
            return input;
        }
        nameIndex.first += c;
    }

    __Resources::ResourceMap::iterator
       found = Resources::Instance().Find( "texts", nameIndex );
    
    if ( found == Resources::Instance().End() ) // No translation available
    {
        return input;
    }

    Resource *r = (*found).second;

    SPointer< Text > *t = r->DynaCast< SPointer< Text > >();

    return **t;
}

};//namespace Useless
