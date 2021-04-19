#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateImageDicer( XMLIterator i, XMLCreatorEnv *env )
{
    std::string img;
    std::string mapping;
    std::string image_ids;
    int trace_dots = 0;
    int columns = 0;
    int rows = 0;
    int count = 0;
    ImageDicer *dicer = 0;
    SPointer< SkinMan > smart;
    SPointer< ImageBase > image;
    std::vector< int > *pMap = 0;
    SkinSet *pParent = 0;

    if (! env->Query("parent_skinset", pParent))
    {
        pParent = 0;
    }

    try_attribute( (*i)._attributes, "image_ids", &image_ids );

    img = get_attribute( (*i)._attributes, "img" );
    if ( !try_attribute_i( (*i)._attributes, "trace_dots", &trace_dots ))
    {
        if (! try_attribute_i( (*i)._attributes, "rows", &rows ))
        {
            rows = 1;
        }
        if (! try_attribute_i( (*i)._attributes, "columns", &columns ))
        {
            columns = 1;
        }
    }

    if ( try_attribute( (*i)._attributes, "mapping", &mapping ))
    {
        Resources::Instance().Query( "mappings", mapping, pMap );
    }

    if (! try_attribute_i( (*i)._attributes, "count", &count ))
    {
        count = 1;
    }

    for ( int i=0; i < count; ++i )
    {
        int index = (count > 0 ? i : count-i-1);
        std::string indStr = from_integer< char >( index );
        std::string imgName = (count == 1 ? img : img + "[" + indStr + "]");
        image = MakeImage( imgName );
        
        if ( trace_dots )
        {
            dicer = new ImageDotTracer( *image );
        }
        else
        {
            dicer = new ImageDicer( *image, columns, rows );
        }
        smart = dicer;

        if ( !image_ids.empty() )
        {
            for ( int dI = 0, dN = dicer->Size(); dI < dN-1; ++dI )
            {
                IGraphics &graf = dicer->GetImage( dI );
                SPointer< ImageBase > p_image = &CopyOf< ImageBase >(
                        dynamic_cast< ImageBase & >( graf ));                
                Resources::Instance().Insert("images", std::string( image_ids ),
                            new AnyResource< SPointer< ImageBase > >( p_image ));
            }
        }

        if ( pParent )
        {
            for ( int dI = 0, dN = dicer->Size(); dI < dN-1; ++dI )
            {
                IGraphics &graf = dicer->GetImage( dI );
                pParent->Insert( ImageSkin( dynamic_cast< ImageBase & >( graf )) );
            }
            continue;
        }
        
        if ( pMap )
        {
            dicer->Remap( *pMap );
        }

        if ( 1 == count )
        {
            return new AnyResource< SPointer< SkinMan > >( smart );
        }
    }
    return 0;
}

};//namespace Useless
