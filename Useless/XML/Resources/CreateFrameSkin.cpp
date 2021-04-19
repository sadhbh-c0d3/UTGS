#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateFrameSkin( XMLIterator i, XMLCreatorEnv *env )
{
    std::string src, temp;
    int left=0, right=0, top=0, bottom=0, no_interior=0;

    src = get_attribute( (*i)._attributes, "img" );
    SPointer<ImageBase> sp_image = MakeImage( src );

    try_attribute_i( (*i)._attributes, "left", &left);
    try_attribute_i( (*i)._attributes, "right", &right);
    try_attribute_i( (*i)._attributes, "top", &top);
    try_attribute_i( (*i)._attributes, "bottom", &bottom);
    try_attribute_i( (*i)._attributes, "nointerior", &no_interior);
    
    //Image image( src.c_str() );
    Rect inner( left, top, sp_image->GetWidth() - (right+left), sp_image->GetHeight() - (top+bottom) );

    SPointer<Skin> p_skin = new FrameSkin(*sp_image, inner, !!no_interior);

    SetupSkinAttributes( *p_skin, (*i)._attributes);
    InsertChildSkin( *p_skin, env);

    return new AnyResource< SPointer<Skin> >( p_skin );
}

};//namespace Useless
