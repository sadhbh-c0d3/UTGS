#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateImageSkin( XMLIterator i, XMLCreatorEnv *env )
{   
    std::string src;
    src = get_attribute( (*i)._attributes, "img" );
    
    // detect parent skinset
    SkinSet *p_skinSet = 0;
    SPointer<SkinMan> sp_skinSet;
    SkinSet *p_skins=0;
    bool use_parent_skinset = (*env).Query("parent_skinset", p_skinSet);


    if ( (*i)._name == "images" )
    {
        int count = to_integer< char >( get_attribute( (*i)._attributes, "count" ));
        int first = 0;        
        try_attribute_i( (*i)._attributes, "first", &first );
        
        if ( !use_parent_skinset )
        {
            p_skinSet = new SkinSet;
            sp_skinSet = p_skinSet;
        }

        if ( count > 0 ) for ( int j=0; j < count; ++j )
        {
            SPointer<ImageBase> sp_image = MakeImage( src + '[' + from_integer< char >(first+j) + ']' );
            p_skinSet->Insert( ImageSkin( *sp_image ));
        }
        else for ( int j=(-count); j > 0; --j )
        {
            SPointer<ImageBase> sp_image = MakeImage( src + '[' + from_integer< char >(first+j-1) + ']' );
            p_skinSet->Insert( ImageSkin( *sp_image ));
        }

        if ( !use_parent_skinset )
        {
            return new AnyResource< SPointer<SkinMan> >( sp_skinSet );
        }
        else
        {
            return 0;
        }
    }
    else
    {
        SPointer<ImageBase> sp_image = MakeImage( src );
        SPointer<Skin> p_skin = new ImageSkin( *sp_image );

        SetupSkinAttributes( *p_skin, (*i)._attributes);
        InsertChildSkin( *p_skin, env);    
        return new AnyResource< SPointer<Skin> >( p_skin );
    }
}

}; // namespace Useless
