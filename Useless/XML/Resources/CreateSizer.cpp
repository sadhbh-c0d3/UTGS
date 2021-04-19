#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/GUI/Skins/NullSkin.h"

namespace Useless {

Resource* CreateSizerWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    const char *p_name = "create sizer widget";
    BeginParentWidget( env, p_name, ch_env );

    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    int left=-1, right=-1, top=-1, bottom=-1, divider = -1, vertical=false;

    try_attribute_i( attributes, "left", &left);
    try_attribute_i( attributes, "right", &right);
    try_attribute_i( attributes, "top", &top);
    try_attribute_i( attributes, "bottom", &bottom);
    try_attribute_i( attributes, "divider", &divider);

    std::string skin_name;
    ArraySkin *p_skin;
    NullSkin null_skin;

    if ( try_attribute( attributes, "skin", &skin_name ) )
    {
        p_skin = &QueryArraySkin(skin_name);
        if (left==-1)   left = p_skin->GetDimension( LEFT_MARGIN );
        if (right==-1)  right = p_skin->GetDimension( RIGHT_MARGIN );
        if (top==-1)    top = p_skin->GetDimension( TOP_MARGIN );
        if (bottom==-1) bottom = p_skin->GetDimension( BOTTOM_MARGIN );
        if (divider==-1)
            divider = p_skin->GetDimension( (vertical)?  HORIZ_DIV : VERTI_DIV);
    }
    else
    {
        p_skin = &null_skin;
    }

    if ( try_attribute_i( attributes, "vertical", &vertical) )
    {
        (*p_skin).SetDirMask( Pos(0,1) );
    }

    SizerStyle style( *p_skin, left, top, right, bottom, divider );

    SizerWidget * ptr = new SizerWidget( style );
    
    ch_env.Set("parent_sizer", ptr);

    // create children elements
    ChildsFromXML( i, &ch_env );

    InsertChildWidget( ptr, attributes, env);

    return new AnyResource<SizerWidget*>( ptr );
}

};//namespace Useless
