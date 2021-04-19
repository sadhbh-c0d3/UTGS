#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"

namespace Useless {

Resource* CreateLayoutGap( XMLIterator i, XMLCreatorEnv *env )
{
    LayoutBase *p_parent_layout=0;
    SizerWidget *p_parent_sizer=0;

    std::string size = "0";
    try_attribute( (*i)._attributes, "size", &size );
    int s;
    int u = size.find("px",0);
    if ( u!= -1 ) { s = -to_integer(size.substr(0,u)); }
    else { s = to_integer(size); }

    if ( (*env).Query("parent_layout", p_parent_layout) )
    {
        Gap_ g(s);
        (*p_parent_layout) << g;
    }
    else if ( (*env).Query("parent_sizer", p_parent_sizer) )
    {
        (*p_parent_sizer).AddCell( (s>0)? s:-s, (s>0)? false:true );
    }
    else
    {
        throw Error("LayoutGap can only be created inside layouted widget");
    }
    return NULL;
}

};//namespace Useless
