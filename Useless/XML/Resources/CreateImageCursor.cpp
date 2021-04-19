#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSystem.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/GUI/Display/ImageCursor.h"
#include "Useless/Util/str_conv.h"

namespace Useless {

Resource* CreateImageCursor( XMLIterator i, XMLCreatorEnv *env )
{
    std::string imageName = get_attribute( (*i)._attributes, "img");
    SPointer< IGraphics > image = MakeImage( imageName );
    Pos hotspot;
    std::string hotspotStr;
    if ( try_attribute( (*i)._attributes, "hotspot", &hotspotStr ))
    {
        hotspot.x = to_integer_vector( hotspotStr )[0];
        hotspot.y = to_integer_vector( hotspotStr )[1];
    }

    ImageCursor *p = new ImageCursor( *image, hotspot );
    
    return new AnyResource< SPointer< MouseCursor > >( p );
}

SPointer< MouseCursor > MakeMouseCursor( const std::string &id )
{
    while (! Resources::Instance().Exists("cursors", id ))
    {
        __Resources::NameIndex name_id( id );
        SPointer< MouseCursor > sp_cursor = new ImageCursor( *MakeImage( id ));
        Resources::Instance().Insert("cursors", name_id.first, new AnyResource< SPointer< MouseCursor > >( sp_cursor ));
    }
    
    SPointer< MouseCursor > *p_cursor;
    Resources::Instance().Query("cursors", id, p_cursor);
    return *p_cursor;
}

};//Useless
