#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/GUI/Widgets/AdvTableSystemWidget.h"

namespace Useless {

Resource* CreateTableView( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    
    const char *p_name = "create table view";
    BeginParentWidget( env, p_name, ch_env );

    CascadeStyles( i, env, attributes, branch_style, ch_env );

    TableSystemWidget *ptr;
    
    std::string s[2];
    bool a = try_attribute( (*i)._attributes, "header_skin", &( s[0] ));
    bool b = try_attribute( (*i)._attributes, "header_font", &( s[1] ));
    if ( a ^ b )
    {
        throw Error("CreateTableView: missing header_skin/font");
    }
    else if ( a && b )
    {
        ptr = new AdvTableSystemWidget
            ( & QuerySkinMan( s[0] ),
              & QueryFontSet( s[1] ),
              0, 0, 0 );
    }
    else
    {
        ptr = new TableSystemWidget(100, 100);
    }    

    ch_env.Set("parent_tablesystem", ptr );
    
    ChildsFromXML( i, &ch_env );

    InsertChildWidget( ptr, attributes, env);    

    return new AnyResource< TableSystemWidget *>( ptr );
}

};//Useless
