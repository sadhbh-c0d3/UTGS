#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateFacedFont( XMLIterator i, XMLCreatorEnv *env )
{
    std::string src, desc;
    AnyResource<Font> *p_font_res;
    
    desc = get_attribute( (*i)._attributes, "desc" );
    
    // SPECIAL DELTAS ------------------------------------
    int d;
    int Dx = ImageDotTracer::_Dx;
    int Dy = ImageDotTracer::_Dy;
    int Dw = ImageDotTracer::_Dw;
    int Dh = ImageDotTracer::_Dh;
    if ( try_attribute_i( (*i)._attributes, "Dx", &d))
    {
        ImageDotTracer::_Dx = d;
    }
    if ( try_attribute_i( (*i)._attributes, "Dy", &d))
    {
        ImageDotTracer::_Dy = d;
    }
    if ( try_attribute_i( (*i)._attributes, "Dw", &d))
    {
        ImageDotTracer::_Dw = d;
    }
    if ( try_attribute_i( (*i)._attributes, "Dh", &d))
    {
        ImageDotTracer::_Dh = d;
    }
    //----------------------------------------------------
    
    if ( try_attribute( (*i)._attributes, "src", &src) )
    {
        p_font_res = new AnyResource<Font>( src.c_str(), desc.c_str() );
    }
    else
    {
        src = get_attribute( (*i)._attributes, "img");
        ImageBase &img = QueryImage(src);
        p_font_res = new AnyResource<Font>( FacedFont(img, desc.c_str()));
    }

    InsertChildFont( *p_font_res->Get(), env );

    // RESET DELTAS ------------------
    ImageDotTracer::_Dx = Dx;
    ImageDotTracer::_Dy = Dy;
    ImageDotTracer::_Dw = Dw;
    ImageDotTracer::_Dh = Dh;
    //--------------------------------

    return p_font_res;
}

};//namespace Useless
