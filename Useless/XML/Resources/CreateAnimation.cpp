#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

#include "Useless/GUI/Skins/ImageDotTracer.h"

namespace Useless {

Resource* CreateAnimation( XMLIterator i, XMLCreatorEnv *env )
{
    const XMLNode &node = *i;
    std::string img;
    int columns = 1;
    int rows = 1;
    int trace_dots = 0;
    try_attribute_i( node._attributes, "columns", &columns );
    try_attribute_i( node._attributes, "rows", &rows );
    try_attribute_i( node._attributes, "trace_dots", &trace_dots);
    
    img = get_attribute( node._attributes, "img" );
    SPointer<ImageBase> sp_image = MakeImage( img);
    Animation *p_anim;

    if ( trace_dots)
    {
        ImageDotTracer frames( *sp_image);
        p_anim = new Animation( frames);
    }
    else
    {
        C_assert( columns>0 && rows>0);
        C_warning( columns>1 || rows>1);
        
        ImageDicer frames( *sp_image, columns, rows);
        p_anim = new Animation( frames);
    }
    
    double holdTime = 0.0;
    if ( try_attribute_r( node._attributes, "hold_time", &holdTime))
    {
        int heldFrame = -1;
        try_attribute_i( node._attributes, "held_frame", &heldFrame);
        p_anim->SetHoldFrame( holdTime, heldFrame);
    }
    
    int cascadeGroup;
    if ( try_attribute_i( node._attributes, "cascade_group", &cascadeGroup))
    {
        p_anim->SetCascadeGroup( cascadeGroup);
    }
    
    int pingPong;
    if ( try_attribute_i( node._attributes, "ping_pong", &pingPong))
    {
        p_anim->SetPingPong( pingPong);
    }
    
    double frameRate;
    if ( try_attribute_r( node._attributes, "frame_rate", &frameRate))
    {
        p_anim->SetFrameRate( frameRate);
    }

    return new AnyResource< SPointer<ImageBase> >( p_anim);
}

};//namespace Useless
