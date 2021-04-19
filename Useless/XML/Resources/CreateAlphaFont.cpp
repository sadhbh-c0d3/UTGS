#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateFonts.h"

namespace Useless {

Resource* CreateAlphaFont( XMLIterator i, XMLCreatorEnv *env )
{
    std::string src, desc;
    int color;
    desc = get_attribute( (*i)._attributes, "desc" );
    Resource *result = 0;
    
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

    if ( try_attribute( (*i)._attributes, "src", &src ))
    {
        AlphaFont af( src.c_str(), desc.c_str() );
        if ( try_attribute_h( (*i)._attributes, "color", &color ) )
        {
            af.SetColor( color );
        }
        AnyResource<Font> *p_font_res = new AnyResource<Font>( af );
        InsertChildFont( *p_font_res->Get(), env );
        result = p_font_res;
    }
    else if ( try_attribute( (*i)._attributes, "img", &src ))
    {
        ImageBase &img = QueryImage(src);
        AlphaFont af( FacedFont( img, desc.c_str() ));
        if ( try_attribute_h( (*i)._attributes, "color", &color ) )
        {
            af.SetColor( color );
        }
        AnyResource<Font> *p_font_res = new AnyResource<Font>( af );
        InsertChildFont( *p_font_res->Get(), env );
        result = p_font_res;
    }
    else
    {
        throw Error("AlphaFont needs 'src' or 'img' attribute defined");
    }
    
    // RESET DELTAS ------------------
    ImageDotTracer::_Dx = Dx;
    ImageDotTracer::_Dy = Dy;
    ImageDotTracer::_Dw = Dw;
    ImageDotTracer::_Dh = Dh;
    //--------------------------------
    return result;
}

};//namespace Useless
